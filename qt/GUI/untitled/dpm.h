#ifndef DPM_H
#define DPM_H

#include <QObject>

class dpm : public QObject
{
    Q_OBJECT
public:
    explicit dpm(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DPM_H