import QtQuick 2.10
import QtQuick.Controls.Material 2.0
import Material 0.3
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: demo

    title: "Software Test Platform"

    // Necessary when loading the window from C++
    visible: true
    width: dp(1024)
    height: dp(768)
    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }


    property var sectionTitles: [ "Robothand Test", "Dataglove Test"]

    property string selectedComponent: sectionTitles[0]

    initialPage: TabbedPage {
        id: page

        title: "Demo"

        actionBar.maxActionCount: 4

        actions: [
            Action {
                iconName: "alert/warning"
                name: "Dummy error"
                onTriggered: {
                    if(rhMsg.checkconnectstate())
                    {
                        rhMsg.sendEnable(0,0,0,0)
                        emergentStop.show()
                    }
                }
            },

            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
            },

            Action {
                iconName: "alert/warning"
                name: "THIS SHOULD BE HIDDEN!"
                visible: false
            },

            Action {
                iconName: "action/language"
                name: "Language"
                enabled: false
            },

            Action {
                iconName: "action/account_circle"
                name: "Accounts"
            }
        ]

        Dialog {
            id: emergentStop
            width: dp(300)
            title: "Emergency Stop"
            text: "The robothand has stopped by emergency. Would you like to recover to previous PID parameters?"
            hasActions: true
            positiveButtonText: "Yes"
            negativeButtonText: "No"
            }

        Tab {
            id: pidtest
            title: sectionTitles[0]            
            property string selectedComponent: sectionTitles[0]
            property var section: sectionTitles[0]
            source: "pidtesttab.qml"
        }

        Tab {
            title: sectionTitles[1]

            property string selectedComponent: sectionTitles[1]
            property var section: sectionTitles[1]
            source: "dataglovetest.qml"
        }

    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: dp(160)
        }

        Grid {
            columns: 7
            spacing: dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent


                        onPressed: {
                            switch(selection.selectedIndex) {
                            case 0:
                                theme.primaryColor = parent.color
                                break;
                            case 1:
                                theme.accentColor = parent.color
                                break;
                            case 2:
                                theme.backgroundColor = parent.color
                                break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }
}
