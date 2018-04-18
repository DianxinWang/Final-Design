#include "msg.h"
#include <QDateTime>


Msg::Msg(MsgThread *parentThread, QObject *parent) :
     m_parentThread(parentThread),
     QObject(parent),
     m_hid(RH_VID, RH_PID, this),
     m_rhtrace(4),
     m_rhforce(4)
{
    qDebug()<<"MsgConstructor ID"<<QThread::currentThreadId();
    m_rawdata = new unsigned char[65];
    m_packet = new unsigned char[65];
    ctx = new mathpresso::Context;
    exp = new mathpresso::Expression;
    ctx->addBuiltIns();
    ctx->addVariable("t", 0 * sizeof(double));
}


void Msg::starhid()
{
    if(m_hid.handle == NULL)
        {//qDebug()<< "77";
        m_hid.open(RH_VID, RH_PID);}
    if(m_hid.handle == NULL) return;
    //qDebug()<< "80";
    connect(this, &Msg::msgReceived, this, &Msg::processMsg);
    connect(this, &Msg::startReceiv, this, &Msg::getRegularRawMsg);
    emit(startReceiv());
}

void Msg::getRegularRawMsg()
{
    //clear previous data buffer    
    qDebug()<<"Receiving ID"<<QThread::currentThreadId();
    int state;
    m_isCanRun = true;
    for(;;)
    {
        state = m_hid.read_wait(m_rawdata,0);
        if(state == -1) return;
        else if(state == 0) break;
    }

    for(;;)
    {
        state = m_hid.read(m_rawdata);
        if(state == -1) return;
        else if(state > 0) emit(msgReceived());
        {
            QMutexLocker locker(&m_lock);
            if(!m_isCanRun)//check if keeping running
            {
                return;
            }
        }
    }
}

void Msg::stopRegularRawMsg()
{
    QMutexLocker locker(&m_lock);
    m_isCanRun = false;
}

void Msg::processMsg()
{
    qDebug()<<"processMsg ID"<<QThread::currentThreadId();
    if(Msg::isPacketValid())
    {
        if(m_rawdata[1] == PC)
        {
           unsigned short int *data = (unsigned short int *)&m_rawdata[4];
            switch(m_rawdata[2])
            {
            case RH_TRACE_FORCE_MSG:
                for(int i = 0; i < 4; i++)
                {
                    m_rhtrace[i] = data[2*i];
                    m_rhforce[i] = data[1+2*i];
                }
                qDebug() << m_rhtrace[0]<<" "<<m_rhtrace[1]<<" "<<m_rhtrace[2]<<" "<<m_rhtrace[3];
                emit(m_parentThread->traceDraw(m_rhtrace));
                emit(m_parentThread->forceDraw(m_rhforce));
                break;
            case DG_FORCE_BENCH_MSG:
                break;
            }
        }
    }
}

void Msg::startTest(QString expr, int interval)
{
    QByteArray ba = expr.toLatin1(); // Convert to char*
    mathpresso::Error err = exp->compile(*ctx, ba.data(), mathpresso::kNoOptions);
    if (err != mathpresso::kErrorOk) {
      //printf("Expression Error: %u\n", err);
      //Todo Trigger a signal to show msg.
      return;
    }
    m_times = 0;
    m_timerID = this->startTimer(interval);
}

void Msg::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerID){
        m_times++;
        uint16_t motion = exp->evaluate(&m_times);
        sendMotion(motion, motion, motion, motion);
        if(m_times == TESTTIME)
        {
            killTimer(m_timerID);
        }
    }
}

void Msg::sendMotion(uint16_t motion1, uint16_t motion2, uint16_t motion3, uint16_t motion4)
{
    uint16_t motion[4] = {motion1, motion2, motion3, motion4};
    buildMotionCTRLMsg(motion, m_packet);
    m_hid.write(m_packet);
}

void Msg::sendPID(float p, float i, float d)
{
    float pid[12] = {p,i,d,p,i,d,p,i,d,p,i,d};
    buildPIDPareCTRLMsg((uint16_t *)pid, m_packet);
    m_hid.write(m_packet);
}

void Msg::sendInterval(int interval)
{
    uint16_t intv = (uint16_t)(interval & 0xffff);
    buildIntervalCTRLMsg(&intv, m_packet);
    m_hid.write(m_packet);  
}

void Msg::sendEnable(int enable1, int enable2, int enable3, int enable4)
{
    uchar enable[4] = {(uchar)(enable1 & 0xff), (uchar)(enable2 & 0xff),
                       (uchar)(enable3 & 0xff), (uchar)(enable4 & 0xff)};
    buildMotorStatusCTRLMsg((uint16_t *)enable, m_packet);
    m_hid.write(m_packet);
}

void Msg::buildMotionCTRLMsg(uint16_t *motion, unsigned char *packet)
{
    Msg::buildCMDMsg(MotionCTRL, motion, packet);
}

void Msg::buildPIDPareCTRLMsg(uint16_t *PIDpara, unsigned char *packet)
{
    Msg::buildCMDMsg(PIDParaCTRL, PIDpara, packet);
}

void Msg::buildMotorStatusCTRLMsg(uint16_t *status, unsigned char *packet)
{
    Msg::buildCMDMsg(MotorStatusCTRL, status, packet);
}

void Msg::buildIntervalCTRLMsg(uint16_t *interval, unsigned char *packet)
{
    Msg::buildCMDMsg(IntervalCTRL, interval, packet);
}

void Msg::buildCMDMsg(CMDType type, uint16_t *data_ptr, unsigned char *packet)
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
        memcpy(&in_data[1], data_ptr, 48);
        size = 49;
        break;
    case MotorStatusCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 4);
        size = 4;
        break;
    case IntervalCTRL:
        in_data[0] = type;
        memcpy(&in_data[1], data_ptr, 2);
        size = 3;
        break;
    }
    Msg::buildMsg((unsigned char *)in_data, RH, CMD, size, packet);
}

void Msg::buildMsg(unsigned char *data_ptr, PacketDestination dest, PacketType type, uint8_t data_size, unsigned char *packet)
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

int Msg::isPacketValid()
{
    return (
        m_rawdata[0] == PACKET_HEADER &&
        m_rawdata[3] <= DATA_SIZE_MAX &&
        m_rawdata[1] == (m_rawdata[m_rawdata[3]+5] ^ 0xFF) &&
        m_rawdata[2] == (m_rawdata[m_rawdata[3]+4] ^ 0xFF) &&
        m_rawdata[m_rawdata[3]+6] == PACKET_FOOTER);
}
