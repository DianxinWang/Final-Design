import QtQuick 2.10
import QtQuick.Layouts 1.3
import Material 0.3
import Material.ListItems 0.1 as ListItem
import QtCharts 2.2

Item {
    Sidebar {
        id: sidebar
        expanded: true

        Column {
            width: parent.width
            ListItem.Subheader {
                text: "DataGlove Connection"
                showDivider: true
            }
            ListItem.Subtitled {
                text: "Connect"
                property var connectMsg: ["Disconnected","Connected"]
                subText: connectMsg[!enableSwitch.checked? 0:1]
                secondaryItem: Switch {
                    id: enableSwitch
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged: {
                        if(exportSwitch.checked)
                            exportSwitch.checked = false
                        if(checked) {
                            if(!dgMsg.starthid()){
                                checked = false
                            }
                        } else {
                            dgMsg.stophid()
                        }
                    }
                }
                onClicked: enableSwitch.checked = !enableSwitch.checked

                action: Icon {
                    anchors.centerIn: parent
                    name: "social/share"
                    size: dp(32)
                }
            }


            ListItem.Subtitled {
                id: exportindex
                text: "Export"
                property var connectMsg: ["Disconnected","Connected"]
                subText: connectMsg[!exportSwitch.checked? 0:1]
                secondaryItem: Switch {
                    id: exportSwitch
                    enabled: enableSwitch.checked
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged: {
                        if(checked){
                            rhMsg.starthid()
                            if(rhMsg.isConnected())
                            {
                                rhMsg.stopTest()
                                rhMsg.sendEnable(1,1,1,1)
                                dgMsg.startRH()
                                dgchartloader0.item.istracked = true
                                dgchartloader1.item.istracked = true
                            } else {
                                checked = false
                            }
                        } else {
                            dgchartloader0.item.istracked = false
                            dgchartloader1.item.istracked = false
                            if(rhMsg.isConnected())
                            {
                                rhMsg.stopTest()
                                rhMsg.sendEnable(0,0,0,0)
                            }
                            dgMsg.stopRH()
                        }
                    }

                    Connections {
                        target: rhMsgThread;
                        onHiddisconnected:{
                            exportSwitch.checked = false
                            exportSwitch.checked = false
                        }
                    }
                    Connections {
                        target: dgMsgThread;
                        onHiddisconnected:{
                            enableSwitch.checked = false

                        }
                    }
                }                
                onClicked: exportSwitch.checked = !exportSwitch.checked
                action: Icon {
                    anchors.centerIn: parent
                    name: "social/share"
                    size: dp(32)
                }
            }

            ListItem.Subheader {
                text: "Calibration"
                showDivider: true
            }

            ListItem.Subtitled {
                text: " Calibrtation Tool"
                enabled:enableSwitch.checked
                onClicked: {
                    calibdialog.show()
                    dgMsg.startCalib()
                }
            }
        }
    }

    Flickable {
        anchors {
            left: sidebar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        clip: true
        contentHeight: Math.max(dgchartloader0.implicitHeight + 40, height)
        Loader {
            id: dgchartloader0
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width/2
            asynchronous: true
            visible: status == Loader.Ready
            source: "dgchart_1.qml"
            onLoaded: {
            }
        }
        Loader {
            id: dgchartloader1
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: parent.width/2
            asynchronous: true
            visible: status == Loader.Ready
            source: "dgchart_2.qml"
            onLoaded: {
            }
        }

        ProgressCircle {
            anchors.centerIn: parent
            visible: dgchartloader0.status == Loader.Loading
        }

        Ink {
            anchors.fill: parent
            onClicked: {
                overlayview.open(dgchartloader0)
                chartfull.enabled = true
            }
        }
    }

    OverlayView {
        id: overlayview
        ChartView {
            id: chartfull
            anchors.fill: parent
            antialiasing: true
            enabled: false
            theme: ChartView.ChartThemeLight
            Ink {
                anchors.fill:parent
                onClicked: overlayview.close()
            }

            legend {
                visible: false
            }
            ValueAxis {
                id: full_xAxis
                min: 0
                max: 500
                labelsVisible: false
            }

            ValueAxis {
                id:full_yAxis
                min: -500
                max: 500
                labelsVisible: false
            }
            LineSeries {
                id: line0
                name: "Trace"
                useOpenGL: true
                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
            LineSeries {
                id: line1
                name: "Trace"
                useOpenGL: true
                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
            LineSeries {
                id: line2
                name: "Trace"
                useOpenGL: true
                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
            LineSeries {
                id: line3
                name: "Trace"
                useOpenGL: true
                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
            Connections {
                target: dgMsgThread
                onUpdateSeries:{
                    //dgMsg.update(line0,0)
                    //dgMsg.update(line1,1)
                    //dgMsg.update(line2,2)
                    dgMsg.update(line3,3)
                }
            }
        }
    }

    Snackbar {
        id: dialogSnackBar
    }

    Dialog {
        id: calibdialog
        width: dp(300)
        title: "Tutorial"
        text: "Follow the instruction to calibrate DataGlove."
        hasActions: true
        positiveButtonText: "Confirm"
        positiveButtonEnabled: false
        negativeButtonText: "Cancel"
        onAccepted: {
            calibdialog.positiveButtonEnabled = false
        }

        Connections {
            target: dgMsgThread
            onCalibStepChanged:{
                if(index == 0)
                    calibdialog.text = "Follow the instruction to calibrate DataGlove.\nOpen your hand."
                else if(index == 1)
                    calibdialog.text = "Follow the instruction to calibrate DataGlove.\nFist your hand."
                else if(index == 2){
                    calibdialog.text = "Finish Calibration."
                    calibdialog.positiveButtonEnabled = true
                    }
            }
        }
    }
}
