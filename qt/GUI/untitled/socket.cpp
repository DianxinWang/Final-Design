#include "socket.h"
#include <QObject>
socket::socket(QObject *parent) : QObject(parent)
{
    m_socket=new QUdpSocket;
    port = UNIX_PORT;
    //connect(m_socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    m_socket->bind(QHostAddress::Any, port);
    m_packet = "A";
}

bool socket::initudp()
{
    bool result = m_socket->bind(port);
    return result;
}

void socket::dataRecieved()
{

}

void socket::send(quint8 *data)
{
    qDebug()<< m_socket->writeDatagram((char *)data, 20, QHostAddress(UNIX_IP), UNIX_PORT);
    return;
}
