#include "rhmsg.h"

Q_DECLARE_METATYPE(QAbstractSeries *)
RHMsg::RHMsg(RHMsgThread *parentThread, QObject *parent) :
     m_parentThread(parentThread),
     QObject(parent),
     m_hid(RH_VID, RH_PID, this),
     m_rhsetTrace(4),
     m_points(12),
     m_force(4),
     m_trace(4),
     m_setForce(4),
     m_setTrace(4)
{
    connect(this, &RHMsg::msgReceived, this, &RHMsg::processMsg,Qt::DirectConnection);
    qDebug()<<"RHMsg::RHMsg ID"<<QThread::currentThreadId();
    ctx = new mathpresso::Context;
    exp = new mathpresso::Expression;
    ctx->addBuiltIns();
    ctx->addVariable("t", 0 * sizeof(double));
    qRegisterMetaType<QAbstractSeries*>();
}

RHMsg::~RHMsg()
{
    //emit stoptimer();
    qDebug()<<"RHMsg::~RHMsg ID"<<QThread::currentThreadId();
    stophid();
    delete ctx;
    delete exp;
    future.waitForFinished();
}

//tested
bool RHMsg::starthid()
{
    qDebug()<<"RHMsg::starhid ID"<<QThread::currentThreadId();
    if(m_hid.handle == NULL)
    {
        m_hid.open(RH_VID, RH_PID);
        if(m_hid.handle == NULL) return false;
    }
    if(!future.isRunning())
        future = QtConcurrent::run(this, &RHMsg::getRawMsg);
    sendPID(1, 0.022, 0.06, 0.002);
    sendPID(3, 0.026, 0.06, 0.0027);
    return true;
}
//tested
void RHMsg::stophid()
{    
    QMutexLocker locker(&m_lockhidrun);
    m_isCanReceive = false;
    if(!future.isRunning())
        future.waitForFinished();
}
//tested
void RHMsg::getRawMsg()
{
    qDebug()<<"Receiving ID"<<QThread::currentThreadId();
    int state;
    static packetType rawdata = new unsigned char[65];
    m_isCanReceive = true;
    for(;;)
    {
        //state returns the number of bytes it reads
        //return -1 when it fails to connect
        state = m_hid.read_wait(rawdata,0);
        if(state == -1) {
            m_hid.close();
            emit m_parentThread->hiddisconnected();
            return;
        }
        else if(state == 0) break;
    }
    emit m_parentThread->hidconnected();
    for(;;)
    {
        state = m_hid.read(rawdata);
        if(state == -1) {
            m_hid.close();
            emit m_parentThread->hiddisconnected();
            return;
        }
        else if(state > 0) emit msgReceived(rawdata);
        //This is to stop hid connection
        {
            QMutexLocker locker(&m_lockhidrun);
            if(!m_isCanReceive)
            {
                return;
            }
        }
    }
}
//tested
void RHMsg::processMsg(packetType rawdata)
{
    if(RHMsg::isPacketValid(rawdata))
    {
        if(rawdata[1] == PC) //Todo: rawdata[1]
        {
           unsigned short int *data = (unsigned short int *)&rawdata[4];
            switch(rawdata[2])
            {
            case RH_TRACE_FORCE_MSG:
                RHMsg::processRHTraceForceMsg(data);
                break;
            case RH_Status:
                RHMsg::processRHStatusMSG(data);
                break;
            }
        }
    }
}

void RHMsg::processRHStatusMSG(quint16 *data)
{
    float *pidfloat =(float *) data;
    QVector<QString> pidprevious;
    for(int i = 0; i < 12; i++)
    {
        pidprevious << QString::number(pidfloat[i]);
    }
    quint16 interv;
    memcpy(&interv, data+24, 2);
    pidprevious << QString::number(interv);
    m_parentThread->updatePreviousPID(pidprevious);
}

void RHMsg::processRHTraceForceMsg(quint16 *data)
{
    static int times = 0;
    if(data[0] > 60000) data[0] = 0;
    data[0] = 10*data[0]/8;
    for(int i = 0; i < 4; i++)
    {
        m_trace[i] = data[i];
        m_force[i] = data[i+4];
        if(data[i]>60000)
            m_trace[i] = 0;
    }
    if (++times < RH_RESOLUTION) return;
    times = 0;
    QVector<QVector<QPointF>> points(12);

    if (m_points[0].count() < RH_RANGE) {
        points = m_points;
    } else {
        for (int i = 1; i < m_points[0].count(); i++)
        {
            for (int j = 0; j < 12; j++)
            {
                int temp;
                if( m_points[j].at(i).y()>60000)  temp = 0;
                else temp =  m_points[j].at(i).y();
                 points[j].append(QPointF(i - 1, temp));
            }
        }
    }

    qint64 size = points[0].count();
    for (int j = 0; j <4; j++)
    {
        points[j].append(QPointF(size, data[j]));
        points[j+4].append(QPointF(size, data[j+4]));
        points[j+8].append(QPointF(size, m_rhsetTrace[j]));        
    }
    m_points = points;
    emit m_parentThread->updateSeries();
    emit startRHCTRL();
}

void RHMsg::update(QAbstractSeries *series, int num)
{
    if (series) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(m_points.at(num));
    }
}

//tested
void RHMsg::startTest(QString expr)
{
    disconnect(this, &RHMsg::msgReceived, this, &RHMsg::testPID);
    QByteArray ba = expr.toLatin1();
    {
        QMutexLocker locker(&m_locktestrun);
        mathpresso::Error err = exp->compile(*ctx, ba.data(), mathpresso::kNoOptions);
        if (err != mathpresso::kErrorOk) {
            //printf("Expression Error: %u\n", err);
            //Todo Trigger a signal to show msg.
            return;

        }
        m_t = 0;
    }
    connect(this, &RHMsg::msgReceived, this, &RHMsg::testPID);
}

void RHMsg::stopTest()
{
    disconnect(this, &RHMsg::msgReceived, this, &RHMsg::testPID);
}
//tested
void RHMsg::testPID()
{
    uint16_t motion;
    {
        QMutexLocker locker(&m_locktestrun);
        m_t++;
        motion = exp->evaluate(&m_t);
    }
    sendMotion(motion, motion, motion, motion);
}
//tested
void RHMsg::sendMotion(uint16_t motion1, uint16_t motion2, uint16_t motion3, uint16_t motion4)
{
    quint8 packet[65];
    m_rhsetTrace = {motion1, motion2, motion3, motion4};
    uint16_t motion[4] = {8*motion1/10, motion2, motion3, motion4};
    buildMotionCTRLMsg(motion, packet);
    m_hid.write(packet);
}
//tested
void RHMsg::sendPID(quint8 index,float p, float i, float d)
{
    quint8 packet[65];
    float pid[] = {p,i,d};
    qDebug()<<pid[0]<<" "<<pid[1]<<" "<<pid[2]<<" "<< index;
    quint8 data[13] = {index};
    memcpy(data+1, pid, 12);
    buildPIDPareCTRLMsg((uint16_t *)data, packet);
    m_hid.write(packet);
}
//tested
void RHMsg::sendInterval(int interval)
{
    quint8 packet[65];
    uint16_t intv = (uint16_t)(interval & 0xffff);
    buildIntervalCTRLMsg(&intv, packet);
    m_hid.write(packet);
}
//tested
void RHMsg::sendEnable(int enable1, int enable2, int enable3, int enable4)
{
    quint8 packet[65];
    uchar enable[4] = {(uchar)(enable1 & 0xff), (uchar)(enable2 & 0xff),
                       (uchar)(enable3 & 0xff), (uchar)(enable4 & 0xff)};
    buildMotorStatusCTRLMsg((uint16_t *)enable, packet);
    m_hid.write(packet);
}
//tested
void RHMsg::sendInteLimit(float intelimit)
{
    quint8 packet[65];
    buildInteLimitCTRLMsg((uint16_t *)&intelimit, packet);
    m_hid.write(packet);
}

void RHMsg::sendRHStatusRequest()
{
    quint8 packet[65];
    buildRH_StatusRETMsg(packet);
    m_hid.write(packet);
}
//tested
void RHMsg::buildMotionCTRLMsg(uint16_t *motion, packetType packet)
{
    RHMsg::buildCMDMsg(MotionCTRL, motion, packet);
}
//tested
void RHMsg::buildPIDPareCTRLMsg(uint16_t *PIDpara, packetType packet)
{
    RHMsg::buildCMDMsg(PIDParaCTRL, PIDpara, packet);
}
//tested
void RHMsg::buildMotorStatusCTRLMsg(uint16_t *status, packetType packet)
{
    RHMsg::buildCMDMsg(MotorStatusCTRL, status, packet);
}
//tested
void RHMsg::buildIntervalCTRLMsg(uint16_t *interval, packetType packet)
{
    RHMsg::buildCMDMsg(IntervalCTRL, interval, packet);
}
//tested
void RHMsg::buildInteLimitCTRLMsg(uint16_t *intelimit, packetType packet)
{
    RHMsg::buildCMDMsg(InteLimitCTRL, intelimit, packet);
}

void RHMsg::buildRH_StatusRETMsg(packetType packet)
{
    RHMsg::buildCMDMsg(RH_StatusRET, NULL, packet);
}
//tested
void RHMsg::buildCMDMsg(CMDType type, uint16_t *data_ptr, packetType packet)
{
    int size = 0;
    unsigned char in_data[49] = {0};
    switch(type){
    case MotionCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 8);
        size = 9;
        break;
    case PIDParaCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 13);
        size = 14;
        break;
    case MotorStatusCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 4);
        size = 5;
        break;
    case IntervalCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 2);
        size = 3;
        break;
    case InteLimitCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 4);
        size = 5;
        break;
     case RH_StatusRET:
        in_data[0] = type;
        size = 1;
        break;
    }
    RHMsg::buildMsg((packetType)in_data, RH, CMD, size, packet);
}
//tested
void RHMsg::buildMsg(packetType data_ptr, PacketDestination dest, PacketType type, uint8_t data_size, packetType packet)
{
    memset(packet, 0, 65);
    if(data_size > DATA_SIZE_MAX)
    {
        //Todo return a log msg to PC
    }
    uint8_t pos = 1;
    packet[pos++] = PACKET_HEADER;
    packet[pos++] = dest;
    packet[pos++] = type;
    packet[pos++] = data_size;
    memcpy(packet+pos, data_ptr, data_size);
    pos += data_size;
    packet[pos++] = type ^ 0xFF;
    packet[pos++] = dest ^ 0xFF;
    packet[pos++]   = PACKET_FOOTER;
}
//tested
bool RHMsg::isPacketValid(packetType packet)
{
    return (
        packet[0] == PACKET_HEADER &&
        packet[3] <= DATA_SIZE_MAX &&
        packet[1] == (packet[packet[3]+5] ^ 0xFF) &&
        packet[2] == (packet[packet[3]+4] ^ 0xFF) &&
        packet[packet[3]+6] == PACKET_FOOTER);
}
//not used now
bool RHMsg::isConnected()
{
    if(m_hid.handle == NULL)
        return false;
    else
        return true;
}

void RHMsg::motionDesign()
{
    bool dgforcestate[4] = {0};
    bool rhforcestate[4] = {0};
    QMutexLocker locker(&m_lockrh);
    for(int i = 0; i < 4; i++)
    {
        if (m_setForce[i] < FORCE_THREASHOLD) dgforcestate[i] = false;
        else dgforcestate[i] = false;
        if (m_force[i] < FORCE_THREASHOLD) rhforcestate[i] = false;
        else rhforcestate[i] = false;
    }
    quint16 rhSetTrace[4] = {0};
    for(int i = 0; i < 4; i++)
    {
        if(dgforcestate[i])
        {
            if(rhforcestate[i])
            {
                rhSetTrace[i] += 0.01*(m_force[i] - m_setForce[i]);
            } else {
                rhSetTrace[i] = 0;
            }
        } else {
            if(rhforcestate[i])
            {
                rhSetTrace[i] -= 0.01*(m_force[i] - m_setForce[i]);

            } else {
                if(abs(m_setTrace[i]-m_trace[i])<500)
                    rhSetTrace[i] = m_rhsetTrace[i];
                else
                    rhSetTrace[i] = m_setTrace[i];
            }
        }
        if(rhSetTrace[i] > RH_ENCOUNTER_MAX) rhSetTrace[i] = RH_ENCOUNTER_MAX;
        else if(rhSetTrace[i] <500) rhSetTrace[i] = 0;
    }
    sendMotion(rhSetTrace[0],rhSetTrace[1],rhSetTrace[2],rhSetTrace[3]);
}
