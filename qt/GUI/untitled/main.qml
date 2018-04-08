import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import QtCharts 2.2
//import an.qt.Msg 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ChartView {
        id: rh1_view
        z:1
        anchors.fill: parent
        antialiasing: true
        LineSeries {
            id: rh1_data
            name: "Rh1"
            useOpenGL: true
            axisX: ValueAxis {
                id: rh1_xAxis
                min: 0
                max: 1000
            }

            axisY: ValueAxis {
                min: 0
                max: 65536
            }
        }
    }

    Button {
        id: button1
        z: 3
        text: "ready"
        anchors.bottom: rh1_view.bottom;
        anchors.bottomMargin: 8;
        anchors.horizontalCenter: parent.horizontalCenter;
        onClicked:{
            rhMsg.starhid()
        }

        Connections {
            target: rhMsgThread;
            onTraceDraw:{
                if(rh1_data.count >= 1000){
                    rh1_data.remove(0)
                }
                rh1_data.append(rh1_data.count, trace[2]);
            }
        }
    }
}
