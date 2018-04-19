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
    title: qsTr("PID TEST")

    TextField {
        id: p_pid
        x: 62
        y: 21
        width: 108
        height: 40
        focus: true
        text: "0.01"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        font.pixelSize: 25
        validator: DoubleValidator{bottom: 0; decimals: 10}
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    TextField {
        id: i_pid
        x: 62
        y: 67
        width: 108
        height: 40
        focus: true
        text: "0.002"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        validator: DoubleValidator{bottom: 0; decimals: 10}
        font.pixelSize: 25
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    TextField {
        id: d_pid
        x: 62
        y: 113
        width: 108
        height: 40
        focus: true
        text: "0"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        validator: DoubleValidator{bottom: 0; decimals: 10}
        font.pixelSize: 25
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    TextField {
        id: interval
        x: 197
        y: 21
        width: 108
        height: 40
        focus: true
        text: "10"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        validator: DoubleValidator{bottom: 0; decimals: 10}
        font.pixelSize: 25
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    TextField {
        id: intelimit
        x: 197
        y: 67
        width: 108
        height: 40
        focus: true
        text: "300"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        validator: DoubleValidator{bottom: 0; decimals: 10}
        font.pixelSize: 25
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    TextField {
        id: expression
        x: 385
        y: 21
        width: 108
        height: 40
        focus: true
        text: "1000*sin(0.01*t)"
        //verticalAlignment: Text.AlignVCenter  //垂直对齐
        font.pixelSize: 25
        //color: "#707070"
        color: "black"
        font.family: "Tahoma"
        renderType: Text.NativeRendering
        selectionColor: "black"
        z: 3
        background: Rectangle {
                    radius: 2
                    implicitWidth: 108
                    implicitHeight: 40
                    border.color: "#333"
                    border.width: 1
        }
    }

    ChartView {
        id: rh_view
        z:1
        anchors.fill: parent
        antialiasing: true
        animationOptions: ChartView.AllAnimations
        animationDuration: 400
        LineSeries {
            id: real_trace
            name: "Output"
            useOpenGL: true
            axisX: ValueAxis {
                id: rh_xAxis
                min: 0
                max: 1000
            }
            axisY: ValueAxis {
                min: 0
                max: 65536
            }
        }
        LineSeries {
            id: ideal_trace
            name: "Set point"
            useOpenGL: true
        }
    }

    Button {
        id: connect
        z: 3
        text: "hid start"
        anchors.bottom: rh_view.bottom;
        anchors.bottomMargin: 8;
        anchors.horizontalCenter: parent.horizontalCenter;
        onClicked:{
            rhMsg.starhid()
        }

        Connections {
            target: rhMsgThread;
            onTraceDraw:{
                if(real_trace.count >= 1000){
                    real_trace.remove(0)
                    ideal_trace.remove(0)
                    if(real_trace.at(real_trace.count-1).x > (rh_xAxis.max-50))
                    {
                        rh_view.scrollRight(rh_view.plotArea.width/10)
                    }
                }
                real_trace.append(real_trace.at(0).x+real_trace.count, realtrace[2]);
                ideal_trace.append(ideal_trace.at(0).x+ideal_trace.count, idealtrace[2]);
            }
        }
    }

    Button {
        id: stophid
        z: 3
        text: "stop hid"
        anchors.leftMargin: 92
        anchors.left: d_pid.left
        anchors.top: d_pid.bottom
        anchors.topMargin: 279
        onClicked: {
            rhMsg.stophid()
        }
    }

    Button {
        id: test
        z: 3
        text: "start test"
        anchors.leftMargin: 4
        anchors.left: d_pid.left
        anchors.top: d_pid.bottom
        anchors.topMargin: 17
        onClicked: {
            var p = parseFloat(p_pid.text)
            var i = parseFloat(i_pid.text)
            var d = parseFloat(d_pid.text)
            var intelim = parseFloat(intelimit.text)
            var interv = parseInt(interval.text)
            rhMsg.sendEnable(0,0,1,0)
            rhMsg.sendPID(p, i, d)
            rhMsg.sendInterval(interv)
            rhMsg.sendInteLimit(intelim)
            rhMsg.startTest(expression.text, interv)
        }
    }
}
