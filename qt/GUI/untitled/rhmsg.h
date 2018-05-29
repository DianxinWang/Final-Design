#ifndef MSGPROCESS_H
#define MSGPROCESS_H

#include <QObject>
#include <QQuickItem>
#include "hid.h"
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QThread>
#include <QMutex>
#include "mathpresso.h"
#include <QtConcurrent>
#include "definitions.h"

QT_CHARTS_USE_NAMESPACE

class RHMsgThread;

class RHMsg : public QObject
{
    Q_OBJECT

public:
    explicit RHMsg(RHMsgThread *parentThread, QObject *parent = nullptr);
    ~RHMsg();

public:
    QVector<quint16> m_force;
    QVector<quint16> m_trace;
    QVector<quint16> m_setTrace;
    QVector<quint16> m_setForce;
    QMutex m_locktestrun;
    QMutex m_lockrh;

private:
    bool isPacketValid(packetType packet);

    void processRHTraceForceMsg(quint16 * data);
    void processRHStatusMSG(quint16 * data);
    void buildMotionCTRLMsg(uint16_t *motion, packetType packet);
    void buildIntervalCTRLMsg(uint16_t *interval, packetType packet);
    void buildMotorStatusCTRLMsg(uint16_t *PIDpara, packetType packet);
    void buildPIDPareCTRLMsg(uint16_t *PIDpara, packetType packet);
    void buildInteLimitCTRLMsg(uint16_t *intelimit, packetType packet);
    void buildRH_StatusRETMsg(unsigned char *packet);
    void buildCMDMsg(CMDType type, uint16_t *data_ptr, packetType packet);
    void buildMsg(unsigned char *data_ptr, PacketDestination dest, PacketType type\
                  , uint8_t data_size, packetType packet);

    RHMsgThread *m_parentThread;
    Hid m_hid;
    packetType m_rawdata;
    QVector<QVector<QPointF>> m_points;    
    QFuture<void> future;
    QVector<int> m_rhsetTrace;
    mathpresso::Context *ctx;
    mathpresso::Expression *exp;
    QMutex m_lockhidrun;


    bool m_isCanReceive;
    double m_t;



signals:
    void msgReceived(unsigned char *rawdata);
    void startRHCTRL();

public slots:
    //Thread 0
    bool starthid();
    void stophid();
    void startTest(QString expr);
    void stopTest();
    void update(QAbstractSeries *series, int num);
    void motionDesign();
    bool isConnected();

    //Thread 1
    void getRawMsg();

    //Thread 2
    void processMsg(packetType rawdata);  //connect this with msgReceived()
    void testPID();

    void sendRHStatusRequest();
    void sendInteLimit(float intelimit);
    void sendPID(quint8 index, float p, float i, float d);
    void sendInterval(int interval);
    void sendMotion(uint16_t motion1, uint16_t motion2, uint16_t motion3, uint16_t motion4);
    void sendEnable(int enable1, int enable2, int enable3, int enable4);
};

class RHMsgThread : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    RHMsgThread() {
        m_msg = new RHMsg(this, nullptr);
        m_msg->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, m_msg, &QObject::deleteLater);
        workerThread.start();
    }
    ~RHMsgThread() {
        workerThread.quit();
        workerThread.wait();
    }
public:
    RHMsg *m_msg;
public slots:
signals:
    void updateSeries();
    void hiddisconnected();
    void hidconnected();
    void updatePreviousPID(const QVector<QString> &pidprevious);
};


#endif // MSGPROCESS_H
