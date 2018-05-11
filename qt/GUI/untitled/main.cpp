#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtQml>
#include "rhmsg.h"
#include "dgmsg.h"
#include "socket.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    qDebug()<<"Main ID"<<QThread::currentThreadId();\

    RHMsgThread rhmsgthread;
    DGMsgThread dgmsgthread(rhmsgthread.m_msg);
    QQmlApplicationEngine engine;
    socket unity;
    engine.rootContext()->setContextProperty("rhMsg", rhmsgthread.m_msg);
    engine.rootContext()->setContextProperty("rhMsgThread", &rhmsgthread);
    engine.rootContext()->setContextProperty("dgMsg", dgmsgthread.m_msg);
    engine.rootContext()->setContextProperty("dgMsgThread", &dgmsgthread);
    engine.rootContext()->setContextProperty("unity", &unity);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
