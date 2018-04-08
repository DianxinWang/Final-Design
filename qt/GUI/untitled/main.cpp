#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "msg.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug()<<"Main ID"<<QThread::currentThreadId();
//    qmlRegisterType<Msg>("an.qt.Msg", 1, 0, "Msg");
    MsgThread msgthread;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("rhMsg", msgthread.m_msg);
    engine.rootContext()->setContextProperty("rhMsgThread", &msgthread);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
