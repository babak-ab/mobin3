import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

ApplicationWindow {
    id: root
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Mobin3")
    onVisibleChanged: {
        if (visible) {
            root.showFullScreen()
        }
    }

    RowLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
        z: 1000
        Button {
            id: menuButton
            text: "Menu"
            background: Rectangle {
                color: "black"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    drawer.open()
                }
            }
        }
    }

    VideoRender {
        anchors.fill: parent
    }

    Repeater {
        model: buttons
        CustomButton {
            id: control
            _point: pos
            _text: name
            _checked: check
            _visible: model.visible

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: false
                onPressed: {
                    model.pressed = true
                }
                onReleased: {
                    model.pressed = false
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: 0.2 * root.width
        height: root.height

        contentItem: Item {
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                RowLayout {
                    Layout.fillWidth: true
                    ToolButton {
                        text: qsTr("<")
                        onClicked: drawer.close()
                    }
                    Label {
                        text: "Menu"
                    }
                }
                MenuSeparator {
                    Layout.fillWidth: true
                }
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: ContactModel {}
                    focus: true
                    delegate: ColumnLayout {
                        width: drawer.width
                        height: 60

                        RowLayout {
                            Layout.fillHeight: true
                            Label {
                                text: name
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                            ToolButton {
                                text: qsTr(">")
                            }
                        }
                        Rectangle {
                            width: parent.width
                            height: 1
                        }
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
    }
}
