#include "msg.h"
#include <QDateTime>

Msg::Msg(MsgThread *parentThread, QObject *parent) :
     m_parentThread(parentThread),
     QObject(parent),
     m_hid(RH_VID, RH_PID, this),
     m_rhtrace(4),
     m_rhforce(4),
     trace_2(1000)
{
    qDebug()<<"MsgConstructor ID"<<QThread::currentThreadId();
    m_rawdata = new unsigned char[65];
}


void Msg::getRegularRawMsg()
{
    //clear previous data buffer    
    qDebug()<<"Receiving ID"<<QThread::currentThreadId();
    int state;
    for(;;)
    {
        state = m_hid.read_wait(m_rawdata,0);
        if(state == -1) return;
        else if(state == 0) break;
    }
    //m_hid.set_nonblocking(0);
    for(;;)
    {
        state = m_hid.read(m_rawdata);
        if(state == -1) return;
        else if(state > 0) emit(msgReceived());
    }
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

int Msg::buildMsg()
{
    return 1;
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
