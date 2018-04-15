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

    void buildMotionCTRLMsg(uint16_t *motion, unsigned char *packet);
    void buildFrequencCTRLMsg(uint16_t *frequence, unsigned char *packet);
    void buildMotorStatusCTRLMsg(uint16_t *PIDpara, unsigned char *packet);
    void buildPIDPareCTRLMsg(uint16_t *PIDpara, unsigned char *packet);



private:
    void buildCMDMsg(CMDType type, uint16_t *data_ptr, unsigned char *packet);
    void buildMsg(unsigned char 		*data_ptr,
                 PacketDestination	dest,
                 PacketType			type,
                 uint8_t 			data_size,
                 packetType         packet);
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
    packetType m_rawdata;
    QVector<int> m_rhtrace;
    QVector<int> m_rhforce;
    packetType m_packet;


signals:
    void msgReceived();
    void startReceiv();
public slots:
    void getRegularRawMsg();
    void processMsg();  //connect this with msgReceived()
    void starhid();

    void sendPID(float p, float i, float d);
    void sendFrequence(float p, float i, float d);
    void sendMotion(uint16_t motion1, uint16_t motion2, uint16_t motion3, uint16_t motion4);
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
