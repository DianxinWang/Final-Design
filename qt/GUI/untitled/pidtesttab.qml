import QtQuick 2.10
import QtQuick.Layouts 1.3
import Material 0.3
import Material.ListItems 0.1 as ListItem
Item {
    id: root

    property var pidprevious
    function resetpid()
    {
        if(enablingSwitch.checked)
        {
            p.text = pidprevious[0+3*fingerselect.selectedIndex]
            i.text = pidprevious[1+3*fingerselect.selectedIndex]
            d.text = pidprevious[2+3*fingerselect.selectedIndex]
            interv.text = pidprevious[12]
            rhMsg.sendPID(fingerselect.selectedIndex, sidebar.pidvalue[0], sidebar.pidvalue[1], sidebar.pidvalue[2])
            rhMsg.sendInterval(sidebar.pidvalue[3])
            rhMsg.stopTest()
            rhMsg.sendMotion(0,0,0,0)
        } else {
            p.text = ""
            i.text = ""
            d.text = ""
            interv.text = ""
            fingerselect.selectedIndex = 0
            testselect.selectedIndex = 0
        }
    }

    Sidebar {
        id: sidebar
        expanded: true
        property var pidvalue: [0,0,0,0]


        Column {
            width: parent.width
            ListItem.Subheader {
                text: "Robothand Connection"
                showDivider: true
            }
            ListItem.Subtitled {
                text: "Connect"
                property var connectMsg: ["Disconnected","Connected"]
                subText: connectMsg[!enablingSwitch.checked? 0:1]
                secondaryItem: Switch {
                    id: enablingSwitch
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged: {
                        if(checked) {
                            if(rhMsg.starthid()){
                                rhMsg.sendRHStatusRequest()
                            } else {
                                rhMsg.stopTest()
                                checked = false
                            }
                        } else {
                            rhMsg.stopTest()
                            dgMsg.stopRH()
                            rhMsg.stophid()
                        }
                    }
                    Connections {
                        target: rhMsgThread;
                        onHiddisconnected:{
                            enablingSwitch.checked = false
                        }
                    }
                    Connections {
                        target: rhMsgThread;
                        onHidconnected:{
                            enablingSwitch.checked = true
                        }
                    }
                    Connections {
                        target: rhMsgThread;
                        onUpdatePreviousPID:{
                            root.pidprevious = pidprevious
                            resetpid()
                        }
                    }
                }

                onClicked: enablingSwitch.checked = !enablingSwitch.checked

                action: Icon {
                    anchors.centerIn: parent
                    name: "social/share"
                    size: dp(32)
                }
            }
            ListItem.Subheader {
                text: "PID Configuration"
                showDivider: true
            }
            ListItem.SimpleMenu {
                id: fingerselect
                text: " Choose Finger"
                model: [" Thumb", " Forefinger", " Middle Finger", " Ring Finger & Little Finger"]
                onSelectedIndexChanged: {
                    example.index = selectedIndex
                    resetpid()
                }
            }
            ListItem.SimpleMenu {
                id: testselect
                text: " Choose Test"
                model: [" Step Response", " Sinusoid Response", " Custom Response"]
                onMenucliked: {
                    if(selectedIndex==2) {
                        customexpr.show()
                    }
                }
            }
            Column {
                width: parent.width
                spacing: dp(18)
                topPadding: dp(10)
                bottomPadding: dp(10)
                leftPadding: dp(20)

                TextField {
                    id: p
                    placeholderText: "P value"
                    floatingLabel: true
                    //anchors.horizontalCenter: parent.horizontalCenter
                    validator: DoubleValidator{bottom: 0;decimals: 10}
                    onTextChanged: sidebar.pidvalue[0] = parseFloat(text)
                }
                TextField {
                    id: i
                    placeholderText: "I value"
                    floatingLabel: true
                    //anchors.horizontalCenter: parent.horizontalCenter
                    validator: DoubleValidator{bottom: 0;decimals: 10}
                    onTextChanged: sidebar.pidvalue[1] = parseFloat(text)
                }
                TextField {
                    id: d
                    placeholderText: "D value"
                    floatingLabel: true
                    //anchors.horizontalCenter: parent.horizontalCenter
                    validator: DoubleValidator{bottom: 0;decimals: 10}
                    onTextChanged: sidebar.pidvalue[2] = parseFloat(text)
                }

                TextField {
                    id: interv
                    placeholderText: "Interval value (ms)"
                    floatingLabel: true
                    //anchors.horizontalCenter: parent.horizontalCenter
                    validator: IntValidator{bottom: 0}
                    onTextChanged: sidebar.pidvalue[3] = parseInt(text)
                }
            }
            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: dp(8)

                anchors {
                    right: parent.right
                    margins: dp(16)
                }

                Button {
                    id:reset
                    text: "Reset"
                    textColor: Theme.primaryColor
                    onClicked: resetpid()

                }

                Button {
                    text: "Start"
                    textColor: Theme.primaryColor
                    enabled: enablingSwitch.checked
                    onClicked: {
                        rhMsg.sendEnable(0==fingerselect.selectedIndex,1==fingerselect.selectedIndex,2==fingerselect.selectedIndex,3==fingerselect.selectedIndex)
                        rhMsg.sendPID(fingerselect.selectedIndex, sidebar.pidvalue[0], sidebar.pidvalue[1], sidebar.pidvalue[2])
                        rhMsg.sendInterval(sidebar.pidvalue[3])
                        switch (testselect.selectedIndex){
                        case 0:
                            rhMsg.startTest("30000*round((t%800)/800)")
                            break
                        case 1:
                            rhMsg.startTest("-15000*cos(0.01*t)+15000+1000")
                            break
                        case 2:
                            rhMsg.startTest(optionText.text)
                            break
                        }
                        dialogSnackBar.open("%1 Start".arg(testselect.model[testselect.selectedIndex]))
                    }
                }
            }
        }
    }
    Rhchart {
        id:example
        visible: true
        anchors {
            left: sidebar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
    }
        /*
    Flickable {
        anchors {
            left: sidebar.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        clip: true
        contentHeight: Math.max(example.height + 40, height)

        Loader {
            id: example
            anchors.fill: parent
            asynchronous: true
            visible: status == Loader.Ready
            //property int index: fingerselect.selectedIndex
            source: "Rhchart.qml"
            onLoaded: {
                item.resetpidsignal.connect(resetpid)
            }
        }*/


/*
        ProgressCircle {
            anchors.centerIn: parent
            visible: example.status == Loader.Loading
        }*/

    }

    Dialog {
        id: customexpr
        title: "Math Expression"
        hasActions: true

        TextField {
            id: optionText
            width: parent.width
            placeholderText: "The Parameter is \"t\""
        }

        onAccepted: {
            dialogSnackBar.open("You entered: %1".arg(optionText.text))
        }
    }

    Snackbar {
        id: dialogSnackBar
    }
}



