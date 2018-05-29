#ifndef DGMSG_H
#define DGMSG_H

#include <QObject>
#include <QQuickItem>
#include "hid.h"
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QtConcurrent>
#include "definitions.h"
#include "socket.h"
#include "rhmsg.h"

QT_CHARTS_USE_NAMESPACE

class DGMsgThread;
class DGMsg : public QObject
{
    Q_OBJECT
public:
    explicit DGMsg(DGMsgThread *parentThread, RHMsg *rhmsg, QObject *parent = nullptr);
    ~DGMsg();

private:
    void getAngleData(float *angle, quint16 *const data);
    bool calibrateDG(quint16 *const data);
    bool isPacketValid(packetType packet);
    void processDGEMGTraceMsg(quint16 *data);    
    void plot(quint16 *data);

    DGMsgThread *m_parentThread;
    RHMsg *m_rhmsg;
    Hid m_hid;
    QVector<QVector<QPointF>> m_points;
    QFuture<void> future;

    QMutex m_lockhidrun;
    bool m_isCanReceive;

    QMutex m_lockCalib;
    bool m_isCalib;
    QVector<float> m_calibmax;
    QVector<float> m_calibmin;
    socket m_unity;


signals:
    void msgReceived(unsigned char *rawdata);
    void sendout(quint8 *data1, quint16 *data2);

public slots:
    //Thread 0
    bool starthid();
    void startRH();
    void stophid();
    void stopRH();

    bool isConnected();
    void update(QAbstractSeries *series, int num);
    void startCalib();


    //Thread 1
    void getRawMsg();

    //Thread 2
    void processMsg(packetType rawdata);
};

class DGMsgThread : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    DGMsgThread(RHMsg *rhmsg) {
        m_msg = new DGMsg(this, rhmsg, nullptr);
        m_msg->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, m_msg, &QObject::deleteLater);
        workerThread.start();
    }
    ~DGMsgThread() {
        workerThread.quit();
        workerThread.wait();
    }
public:
    DGMsg *m_msg;
public slots:
signals:
    void updateSeries();
    void hiddisconnected();
    void calibStepChanged(int index);

};

#endif // DGMSG_H
