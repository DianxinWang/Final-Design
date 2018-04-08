#ifndef HID_H
#define HID_H

#include <QObject>
#include "hidapi.h"
#include "definitions.h"
class Hid : public QObject
{
    Q_OBJECT
public:
    explicit Hid(QObject *parent = nullptr);
    Hid(unsigned short vid, unsigned short pid, QObject *parent = nullptr);
    ~Hid();

    static int init();
    void open(unsigned short VID, unsigned short PID);
    void close();
    int read(unsigned char *data);
    int read_wait(unsigned char *data, int milliseconds);
    int write(const unsigned char *data);
    int set_nonblocking(int nonblock);

    hid_device *handle;

signals:

public slots:
};

#endif // HID_H
