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

QT_CHARTS_USE_NAMESPACE

class MsgThread;

class Msg : public QObject
{
    Q_OBJECT

public:
    explicit Msg(MsgThread *parentThread, QObject *parent = nullptr);
    ~Msg();
    void buildMotionCTRLMsg(uint16_t *motion, unsigned char *packet);
    void buildIntervalCTRLMsg(uint16_t *interval, unsigned char *packet);
    void buildMotorStatusCTRLMsg(uint16_t *PIDpara, unsigned char *packet);
    void buildPIDPareCTRLMsg(uint16_t *PIDpara, unsigned char *packet);
    void buildInteLimitCTRLMsg(uint16_t *intelimit, unsigned char *packet);

private:
    void buildCMDMsg(CMDType type, uint16_t *data_ptr, unsigned char *packet);
    void buildMsg(unsigned char *data_ptr, PacketDestination dest, PacketType type\
                  , uint8_t data_size, packetType packet);
    int isPacketValid();


    MsgThread *m_parentThread;
    Hid m_hid;
    packetType m_rawdata;
    QVector<int> m_rhtrace;
    QVector<int> m_rhsetTrace;
    QVector<int> m_rhforce;
    packetType m_packet;

    mathpresso::Context *ctx;
    mathpresso::Expression *exp;
    QMutex m_lock;
    bool m_isCanRun;

    QTimer *m_timer;


signals:
    void msgReceived();
    void startReceiv();
    void stoptimer();
public slots:
    //Thread 0
    void starhid();
    void stophid();
    void startTest(QString expr, int interval);


    //Thread 1
    void getRegularRawMsg();


    //Thread 2
    void processMsg();  //connect this with msgReceived()
    void testPID();

    void sendInteLimit(float intelimit);
    void sendPID(float p, float i, float d);
    void sendInterval(int interval);
    void sendMotion(uint16_t motion1, uint16_t motion2, uint16_t motion3, uint16_t motion4);
    void sendEnable(int enable1, int enable2, int enable3, int enable4);
};

class MsgThread : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    MsgThread() {
        m_msg = new Msg(this, nullptr);
        m_msg->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, m_msg, &QObject::deleteLater);
        workerThread.start();
    }
    ~MsgThread() {
        workerThread.quit();
        workerThread.wait();
    }
public:
    Msg *m_msg;
public slots:
signals:
    void traceDraw(const QVector<int> &realtrace, const QVector<int> &idealtrace);
    void forceDraw(const QVector<int> &force);
};
#endif // MSGPROCESS_H
