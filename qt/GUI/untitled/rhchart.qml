import QtQuick 2.10
import QtCharts 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles.Material 0.1 as MaterialStyle
import Material 0.2

Item {
    id:root
    property var resetpidsignal : emergentStop.onAccepted
    property alias title: rh_view.title
    property alias dialgtitle: emergentStop.title
    property alias dialgtext: emergentStop.text
    property alias real_trace: real_trace
    property alias ideal_trace: ideal_trace
    property int index

    ChartView {
        id: rh_view
        anchors.fill: parent
        antialiasing: true

        legend {
            visible: false
        }

        ValueAxis {
            id: rh_xAxis
            min: 0
            max: 1000
            labelsVisible: false
        }

        ValueAxis {
            id:rh_yAxis
            min: 0
            max: 65536
            labelsFont {
                family: "sans-serif"
                pixelSize: 15
                bold:true
            }
        }

        LineSeries {
            id: real_trace
            name: "Real Trace"
            useOpenGL: true
            width: 2
            axisX: rh_xAxis
            axisY: rh_yAxis
        }
        LineSeries {
            id: ideal_trace
            name: "Ideal Trace"
            useOpenGL: true
            width: 2
            axisX: rh_xAxis
            axisY: rh_yAxis
        }

        Ink {
            anchors.fill: parent
            onClicked: {
                overlayview.open(rh_view)
                chartfull.enabled = true
            }
        }
    }
    Connections {
        target: rhMsgThread
        onUpdateSeries:{
            rhMsg.update(real_trace,index)
            rhMsg.update(ideal_trace,index+8)
            if(chartfull.enabled)
            {
                rhMsg.update(full_real_trace,index)
                rhMsg.update(full_ideal_trace,index+8)
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
                max: 1000
                labelsVisible: false
            }

            ValueAxis {
                id:full_yAxis
                min: 0
                max: 65536
                labelsFont {
                    family: "sans-serif"
                    pixelSize: 15
                    bold:true
                }
            }
            LineSeries {
                id: full_real_trace
                name: "Ideal Trace"
                useOpenGL: true
                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
            LineSeries {
                id: full_ideal_trace
                name: "Ideal Trace"
                useOpenGL: true

                width: 2
                axisX: full_xAxis
                axisY: full_yAxis
            }
        }
        Row {
            anchors {
                top: parent.top
                right: parent.right
                rightMargin: dp(16)
            }
            height: dp(48)
            opacity: overlayview.transitionOpacity

            spacing: dp(24)

            Repeater {
                model: ["alert/warning", "content/forward"]

                delegate: IconButton {
                    id: iconAction

                    iconName: modelData

                    color: Theme.light.iconColor
                    size: iconName == "content/add" ? dp(40) : dp(37)
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        if (iconName == "alert/warning")
                            if(rhMsg.isConnected())
                            {
                                rhMsg.sendEnable(0,0,0,0)
                                emergentStop.show()
                            }
                        if (iconName == "content/forward")
                            if(rhMsg.isConnected())
                            {
                                rhMsg.sendEnable(0,0,0,0)
                            }
                    }
                }
            }
        }
    }

    Dialog {
        id: emergentStop
        width: dp(300)
        title: "Emergency Stop"
        text: "The robothand has stopped by emergency. Would you like to recover to previous PID parameters?"
        hasActions: true
        positiveButtonText: "Yes"
        negativeButtonText: "No"
        onRejected: emergentStop.close()
    }
}
