#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include "definitions.h"

class socket : public QObject
{
    Q_OBJECT
public:
    explicit socket(QObject *parent = nullptr);
    int port;
    QUdpSocket *m_socket;
    QByteArray m_packet;

    bool initudp();
signals:

public slots:
    void dataRecieved();
    void send(quint8 *data);
};

#endif // SOCKET_H
