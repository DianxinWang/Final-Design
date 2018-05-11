#include "hid.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <qDebug>

Hid::Hid(QObject *parent):
    QObject(parent)
{
    Hid::init();
}

Hid::Hid(unsigned short vid, unsigned short pid,QObject *parent) : QObject(parent)
{
    Hid::init();
    open(vid, pid);
    if(handle != NULL)
    {set_nonblocking(0);
    qDebug()<<handle;}
}

int Hid::init()
{
    return hid_init();
}

void Hid::open(unsigned short VID, unsigned short PID)
{
    handle = hid_open(VID, PID, NULL);
}

int Hid::read(unsigned char *data)
{
    return hid_read(handle, data, PACKET_SIZE);
}


int Hid::read_wait(unsigned char *data, int milliseconds)
{
    return hid_read_timeout(handle, data, PACKET_SIZE, milliseconds);
}


int Hid::write(const unsigned char *data)
{
    QMutexLocker locker(&m_lock);
    return hid_write(handle, data, PACKET_SIZE);
}

int Hid::set_nonblocking(int nonblock)
{
    return hid_set_nonblocking(handle, nonblock);
}
void Hid::close()
{
    hid_close(handle);
    handle=NULL;
}

Hid::~Hid()
{
    hid_exit();
}

