import QtQuick 2.10
import QtCharts 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Styles.Material 0.1 as MaterialStyle
import Material 0.2

Item {
    id:root
    property int index
    property bool istracked:false
    property alias xaxis: dg_xAxis
    property alias yaxis: dg_yAxis
    ChartView {
        id: dg_view
        anchors.fill: parent
        antialiasing: true

        legend {
            visible: false
        }

        ValueAxis {
            id: dg_xAxis
            min: 0
            max: 500
            labelsVisible: false
        }

        ValueAxis {
            id:dg_yAxis
            min: 0
            max: 900
            labelsVisible: false
            tickCount: 6
        }

        LineSeries {
            id: line0
            name: "Trace"
            useOpenGL: true
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line1
            name: "Trace"
            useOpenGL: true
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line2
            name: "Trace"
            useOpenGL: true
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line3
            name: "Trace"
            useOpenGL: true
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line4
            name: "Trace"
            useOpenGL: true
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line5
            name: "Trace"
            color: "#FF1A00"
            useOpenGL: true
            visible: istracked
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line6
            name: "Trace"
            color: "#FF1A00"
            useOpenGL: true
            visible: istracked
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line7
            name: "Trace"
            color: "#FF1A00"
            useOpenGL: true
            visible: istracked
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
        }
        LineSeries {
            id: line8
            name: "Trace"
            color: "#FF1A00"
            useOpenGL: true
            visible: istracked
            width: 2
            axisX: dg_xAxis
            axisY: dg_yAxis
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
        target: dgMsgThread
        onUpdateSeries:{
            dgMsg.update(line0,4)
            dgMsg.update(line1,5)
            dgMsg.update(line2,6)
            dgMsg.update(line3,7)
            dgMsg.update(line4,8)
            if(istracked){
                dgMsg.update(line5,14)
                dgMsg.update(line6,15)
                dgMsg.update(line7,16)
                dgMsg.update(line8,17)
            } else {
                line5.clear()
                line6.clear()
                line7.clear()
                line8.clear()
            }
        }
    }
}
