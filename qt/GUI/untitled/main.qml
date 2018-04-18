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

    TextField {
        id: p_pid
        x: 62
        y: 21
        width: 108
        height: 40
        focus: true
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
        id: frequence
        x: 197
        y: 21
        width: 108
        height: 40
        focus: true
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

    ChartView {
        id: rh1_view
        z:1
        anchors.fill: parent
        antialiasing: true
        animationOptions: ChartView.AllAnimations
        animationDuration: 400
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
        id: connect
        z: 3
        text: "hid start"
        anchors.bottom: rh1_view.bottom;
        anchors.bottomMargin: 8;
        anchors.horizontalCenter: parent.horizontalCenter;
        onClicked:{
            rhMsg.startTest()
            rhMsg.starhid()
        }

        Connections {
            target: rhMsgThread;
            onTraceDraw:{
                if(rh1_data.count >= 1000){
                    rh1_data.remove(0)
                    if(rh1_data.at(rh1_data.count-1).x > (rh1_xAxis.max-50))
                    {
                        rh1_view.scrollRight(rh1_view.plotArea.width/10)
                    }
                }
                rh1_data.append(rh1_data.at(0).x+rh1_data.count, trace[2]);
            }
        }
    }
    Button {
        id: test
        z: 3
        text: "start test"
        anchors.left: d_pid.left
        anchors.top: d_pid.bottom
        anchors.topMargin: 8
        onClicked: {
            var p = parseFloat(p_pid.text)
            var i = parseFloat(i_pid.text)
            var d = parseFloat(d_pid.text)
            var interv = parseInt(frequence.text)
            rhMsg.sendPID(p, i, d)
            rhMsg.sendInterval(interv)
        }
    }
}
