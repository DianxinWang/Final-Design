#include "dgmsg.h"
Q_DECLARE_METATYPE(RHMsg *)
Q_DECLARE_METATYPE(QAbstractSeries *)

DGMsg::DGMsg(DGMsgThread *parentThread,RHMsg *rhmsg, QObject *parent) :
    m_parentThread(parentThread),
    QObject(parent),
    m_rhmsg(rhmsg),
    m_hid(DG_VID, DG_PID, this),
    m_points(DG_POINTS_NUM),
    m_calibmax(5),
    m_calibmin(5),
    m_isCalib(false)
{
    qDebug()<<"DGMsg::DGMsg ID"<<QThread::currentThreadId();
    connect(this, &DGMsg::msgReceived, this, &DGMsg::processMsg,Qt::DirectConnection);
    connect(this, &DGMsg::sendout, &m_unity, &socket::send);
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<RHMsg*>();
    for(int i = 0; i < 5; i++){
        m_calibmin[i] = 0;
        m_calibmax[i] = 10000;
    }
}

DGMsg::~DGMsg()
{
    qDebug()<<"DGMsg::~DGMsg ID"<<QThread::currentThreadId();
    stophid();
    future.waitForFinished();
}
bool DGMsg::starthid()
{
    qDebug()<<"DGMsg::starhid ID"<<QThread::currentThreadId();
    if(m_hid.handle == NULL)
    {
        m_hid.open(DG_VID, DG_PID);
        if(m_hid.handle == NULL) return false;
    }
    if(!future.isRunning())
        future = QtConcurrent::run(this, &DGMsg::getRawMsg);
    return true;
}
//tested
void DGMsg::stophid()
{
    QMutexLocker locker(&m_lockhidrun);
    m_isCanReceive = false;
}


void DGMsg::getRawMsg()
{
    int state;
    static packetType rawdata = new unsigned char[65];
    m_isCanReceive = true;
    for(;;)
    {
        state = m_hid.read_wait(rawdata,0);
        if(state == -1) {
            m_hid.close();
            emit m_parentThread->hiddisconnected();
            return;
        }
        else if(state == 0) break;
    }
    for(;;)
    {
        state = m_hid.read(rawdata);
        if(state == -1) {
            m_hid.close();
            emit m_parentThread->hiddisconnected();
            return;
        }
        else if(state > 0) emit msgReceived(rawdata);
        {
            QMutexLocker locker(&m_lockhidrun);
            if(!m_isCanReceive)
            {
                return;
            }
        }
        qDebug()<<state;
    }
}

void DGMsg::processMsg(packetType rawdata)
{
    if(DGMsg::isPacketValid(rawdata))
    {
        if(rawdata[1] == PC) //Todo: rawdata[1]
        {
            quint16 *data = (quint16 *)&rawdata[4];
            switch(rawdata[2])
            {
            case DG_EMG_FORCE_BENCH_MSG:
                processDGEMGTraceMsg(data);
                break;
            }
        }
    }    
    qDebug()<<1;
}

bool DGMsg::isPacketValid(packetType packet)
{
    return (
        packet[0] == PACKET_HEADER &&
        packet[3] <= DATA_SIZE_MAX &&
        packet[1] == (packet[packet[3]+5] ^ 0xFF) &&
        packet[2] == (packet[packet[3]+4] ^ 0xFF) &&
        packet[packet[3]+6] == PACKET_FOOTER);
}
//todo
void DGMsg::processDGEMGTraceMsg(quint16 *data)
{
    QMutexLocker locker(&m_lockCalib);
    if(m_isCalib)
    {
        if(calibrateDG(data))
            m_isCalib = false;
    } else {
        plot(data);
    }

}
//not used now
bool DGMsg::isConnected()
{
    if(m_hid.handle == NULL)
        return false;
    else
        return true;
}
/* m_points 0-3 EMG
 *          4-8 DG_angle
 *          9-13 DG_force
 *          14-17 RH_trace
 *          18-21 RH_force
 */
void DGMsg::plot(quint16 *data)
{
    QVector<QVector<QPointF>> points(DG_POINTS_NUM);

    //preprocess date into angle data
    static float points_angle[5];
    quint16 *benddata = &data[16];
    quint16 *force = &data[21];
    static float angle[5];
    getAngleData(angle, benddata);
    QMutexLocker locker(&m_rhmsg->m_lockrh);
    emit sendout((quint8 *)angle, force);
    {
        for(int i = 0;i < 4;i++)
        {
            m_rhmsg->m_setTrace[i] = angle[i]/180*RH_ENCOUNTER_MAX;
            m_rhmsg->m_setForce[i] = force[i];
        }
    }
    static int times = 0;
    if (++times < DG_RESOLUTION) return;
    times = 0;
    memcpy(points_angle, angle, 20);
    for(int i = 0; i < 5; i++)
    {
        points_angle[i]+= 180*i;
    }

    if (m_points[0].count() < DG_RANGE) {
        points = m_points;
    } else {
        for (int i = 1; i < m_points[0].count(); i++)
        {
            for (int j = 0; j < DG_POINTS_NUM; j++)
            {
                 points[j].append(QPointF(i - 1, m_points[j].at(i).y()));
            }
        }
    }

    qint64 size = points[0].count();
    for (int j = 0; j < DG_POINTS_NUM; j++)
    {
        if(j < 4) {
            points[j].append(QPointF(size, (qint16)data[j]));
        } else if(j >= 4 && j < 9){
            points[j].append(QPointF(size, points_angle[j-4]));
        } else if(j >= 9 && j < 14){
            points[j].append(QPointF(size, data[j+12]*180.0/DG_UPPER_FORCE + 180*(j - 9)));
        } else if(j >= 14 && j < 18){
            points[j].append(QPointF(size, m_rhmsg->m_trace[j-14]*180.0/RH_ENCOUNTER_MAX + 180*(j - 14)));//todo
        } else if(j >= 18 && j < 22){
            points[j].append(QPointF(size, m_rhmsg->m_force[j-18]*180.0/DG_UPPER_FORCE + 180*(j - 18)));
        }
    }
    m_points = points;
    emit m_parentThread->updateSeries();
}

void DGMsg::update(QAbstractSeries *series, int num)
{
    if (series) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(m_points.at(num));
    }
}

void DGMsg::getAngleData(float *angle, quint16 *const data)
{
    memset(angle, 0, 20);
    for(int i = 0; i < 5; i++)
    {
        angle[i] = (data[i]-m_calibmin[i])/(m_calibmax[i]-m_calibmin[i])*180;
        if(angle[i] < 0) angle[i] = 0;
        else if(angle[i] > 180) angle[i] = 180;
    }
}

bool DGMsg::calibrateDG(quint16 *const data)
{
    static int t = 0;
    quint16 *benddata = &data[16];
    if(t == 0) {
        for(int i = 0; i < 5; i++)
        {
            m_calibmin[i] = benddata[i];
        }
        emit m_parentThread->calibStepChanged(0);
        t++;
    } else if(t < DG_CALIB_TIMES) {
        for(int i = 0; i < 5; i++)
        {
            if(m_calibmin[i] > benddata[i]) m_calibmin[i] = benddata[i];
        }
        t++;
    } else if(t == DG_CALIB_TIMES) {
        for(int i = 0; i < 5; i++)
        {
            //m_calibmin[i] = benddata[i];
            m_calibmax[i] = benddata[i];
        }
        emit m_parentThread->calibStepChanged(1);
        t++;
    } else if(t > DG_CALIB_TIMES && t < 2*DG_CALIB_TIMES){
        for(int i = 0; i < 5; i++)
        {
            if(m_calibmax[i] < benddata[i]) m_calibmax[i] = benddata[i];
        }
        t++;
    } else if(t >= 2*DG_CALIB_TIMES) {
        //m_calibmax[i] = benddata[i];
        emit m_parentThread->calibStepChanged(2);
        t = 0;
        return true;
    }
    return false;
}

void DGMsg::startCalib()
{
    QMutexLocker locker(&m_lockCalib);
    m_isCalib = true;
}


void DGMsg::startRH()
{
    disconnect(m_rhmsg, &RHMsg::startRHCTRL, m_rhmsg, &RHMsg::motionDesign);
    connect(m_rhmsg, &RHMsg::startRHCTRL, m_rhmsg, &RHMsg::motionDesign,Qt::DirectConnection);

}

void DGMsg::stopRH()
{
    disconnect(m_rhmsg, &RHMsg::startRHCTRL, m_rhmsg, &RHMsg::motionDesign);
}
