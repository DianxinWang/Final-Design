import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.0
import QtCharts 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0


//import an.qt.Msg 1.0

Window {
    visible: true
    width: 1224
    height: 770
    title: qsTr("PID TEST")
    color:"#FFFFFF"
    Rectangle {
        id: parameterrect
        anchors.left:parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#9999FF"
        z:2
        width: 270

        TextField {
            id: p_pid
            anchors.top:parent.top
            anchors.topMargin: 30
            anchors.left:parent.left
            anchors.leftMargin: 20
            width: 108
            height: 40
            focus: true
            text: "0.02"
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
            anchors.left: p_pid.left
            anchors.top: p_pid.bottom
            anchors.topMargin: 30
            width: 108
            height: 40
            focus: true
            text: "0.02"
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
            anchors.left: i_pid.left
            anchors.top: i_pid.bottom
            anchors.topMargin: 30
            width: 108
            height: 40
            focus: true
            text: "0.003"
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
            anchors.left: d_pid.left
            anchors.top:  d_pid.bottom
            anchors.topMargin: 30
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
            anchors.left: interval.left
            anchors.top:  interval.bottom
            anchors.topMargin: 30
            width: 108
            height: 40
            focus: true
            text: "300"
            //verticalAlignment: Text.AlignVCenter  //垂直对齐
            validator: DoubleValidator{
                bottom: 0
                decimals: 10
            }
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
            anchors.left: intelimit.left
            anchors.top: intelimit.bottom
            anchors.topMargin: 30
            y: 21
            width: 208
            height: 40
            focus: true
            text: "15000*sin(0.01*t)"
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
                        implicitWidth: 308
                        implicitHeight: 40
                        border.color: "#333"
                        border.width: 1
            }
        }

        Button {
            id: test
            z: 3
            text: "start test"
            anchors.leftMargin: 4
            anchors.left: expression.left
            anchors.top: expression.bottom
            anchors.topMargin: 17
            onClicked: {
                var p = parseFloat(p_pid.text)
                var i = parseFloat(i_pid.text)
                var d = parseFloat(d_pid.text)
                var intelim = parseFloat(intelimit.text)
                var interv = parseInt(interval.text)
                rhMsg.sendEnable(1,1,1,1)
                rhMsg.sendPID(p, i, d)
                rhMsg.sendInterval(interv)
                //rhMsg.sendInteLimit(intelim)
                rhMsg.startTest(expression.text, interv)
            }
        }

    }


    GridLayout {
        rows: 2
        columns: 2
        rowSpacing: 0
        columnSpacing: 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        anchors.left:parameterrect.right
        Loader {
            id: chart0
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            asynchronous: false
            source: "DynamicChart.qml"            
        }
        Loader {
            id: chart1
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            asynchronous: false
            source: "DynamicChart.qml"
        }
        Loader {
            id: chart2
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            asynchronous: false
            source: "DynamicChart.qml"
        }
        Loader {
            id: chart3
            Layout.fillWidth: true;
            Layout.fillHeight: true;
            asynchronous: false
            source: "DynamicChart.qml"
        }
        Connections {
            target: rhMsgThread;
            onUpdateSeries:{
                rhMsg.update(chart0.item.real_trace,0)
                rhMsg.update(chart0.item.ideal_trace,8)
                rhMsg.update(chart1.item.real_trace,1)
                rhMsg.update(chart1.item.ideal_trace,9)
                rhMsg.update(chart2.item.real_trace,2)
                rhMsg.update(chart2.item.ideal_trace,10)
                rhMsg.update(chart3.item.real_trace,3)
                rhMsg.update(chart3.item.ideal_trace,11)
            }
        }
    }


    Button {
        id: connect
        z: 3
        text: "hid start"
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 8;
        anchors.horizontalCenter: parent.horizontalCenter;
        onClicked:{
            rhMsg.starhid()
        }
    }

    Button {
        id: stophid
        z: 3
        text: "stop hid"
        anchors.rightMargin: 92
        anchors.right: connect.left
        anchors.bottom: connect.bottom

        onClicked: {
            rhMsg.stophid()
        }
    }
}
