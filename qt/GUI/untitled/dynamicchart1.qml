import QtQuick 2.10
import QtCharts 2.2

Item {
    id:root
    property alias title: rh_view.title
    property alias real_trace: real_trace
    property alias ideal_trace: ideal_trace
    ChartView {
        id: rh_view
        z:1
        anchors.fill: parent
        antialiasing: true
        animationOptions: ChartView.NoAnimation
        animationDuration: 400
        title: "STAND"
        titleColor: "gray"
        titleFont.family: "Bauhaus 93"
        titleFont.pointSize: 30
        legend {
            visible: true
            font.family:"Arial Black"
            font.pixelSize: 15
            markerShape:Legend.MarkerShapeCircle
            alignment: Qt.AlignBottom
        }

        theme: ChartView.ChartThemeLight

        ValueAxis {
            id: rh_xAxis
            min: 0
            max: 1000
            labelsFont {
                family: "Arial Black"
                pixelSize: 10
            }
        }

        ValueAxis {
            id:rh_yAxis
            min: 0
            max: 65536
            labelsFont {
                family: "Arial Black"
                pixelSize: 10
            }
        }

        LineSeries {
            id: real_trace
            name: "Output"
            useOpenGL: true
            width: 2
            axisX: rh_xAxis
            axisY: rh_yAxis
        }
        LineSeries {
            id: ideal_trace
            name: "Set point"
            useOpenGL: true
            width: 2
            axisX: rh_xAxis
            axisY: rh_yAxis
        }
    }
}
