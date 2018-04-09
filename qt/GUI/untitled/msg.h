#ifndef MSGPROCESS_H
#define MSGPROCESS_H

#include <QObject>
#include <QQuickItem>
#include "hid.h"
#include <QtCharts/QLineSeries>
#include <QVector>
#include <QThread>
QT_CHARTS_USE_NAMESPACE
class MsgThread;
class Msg : public QObject
{
    Q_OBJECT

public:
    explicit Msg(MsgThread *parentThread, QObject *parent = nullptr);

    int buildMsg();


private:
    int isPacketValid()
    {
        return (
            m_rawdata[0] == PACKET_HEADER &&
            m_rawdata[3] <= DATA_SIZE_MAX &&
            m_rawdata[1] == (m_rawdata[m_rawdata[3]+5] ^ 0xFF) &&
            m_rawdata[2] == (m_rawdata[m_rawdata[3]+4] ^ 0xFF) &&
            m_rawdata[m_rawdata[3]+6] == PACKET_FOOTER);
    }
    MsgThread *m_parentThread;
    Hid m_hid;
    packet m_rawdata;
    QVector<int> m_rhtrace;
    QVector<int> m_rhforce;


signals:
    void msgReceived();
    void startReceiv();
public slots:
    void getRegularRawMsg();
    void processMsg();  //connect this with msgReceived()
    void starhid();
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
    void traceDraw(const QVector<int> &trace);
    void forceDraw(const QVector<int> &force);
};
#endif // MSGPROCESS_H
