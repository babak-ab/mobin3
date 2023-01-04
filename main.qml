import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import AppControl 1.0
import QtMultimedia 5.12
import "content"

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

    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    RowLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 25
        z: 1000

        Button {
            id: menuButton
            icon.source: "Images/menu-icon.png"
            icon.color: "transparent"
            icon.height: 60
            icon.width: 60
            background: Rectangle {
                color: "black"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {

                    //drawer.open()
                    stackView.visible = !stackView.visible

                    if (stackView.visible) {
                        menuButton.icon.source = "qrc:/Images/navigation_previous_item.png"
                        menuButton.icon.color = "transparent"
                        stackView.pop()
                        background.color = "black"

                        stackView.anchors.topMargin = 100
                        stackView.anchors.leftMargin = 30
                    } else {
                        menuButton.icon.source = "qrc:/Images/menu-icon.png"
                        menuButton.icon.color = "transparent"
                    }
                }
            }
        }
        Button {
            text: "Record"
            checkable: true
            highlighted: checked
            onClicked: {
                if (checked) {
                    var dt = new Date().toLocaleString(Qt.locale(),
                                                       "yyyy-MM-dd_hh_mm_ss")

                    camera.fileName = "video_" + dt + ".mp4"
                    camera.videoRecorder.record()
                } else {
                    camera.videoRecorder.stop()
                }
            }
        }
    }

    VideoRender {
        id: videoRender
        anchors.fill: parent
        source: camera
        Camera {
            id: camera
            captureMode: Camera.CaptureVideo
            property string fileName: "test.mp4"
            property string path: "/home/babak/Videos/"
            onFileNameChanged: {
                camera.videoRecorder.outputLocation = path + fileName
            }

            videoRecorder {
                resolution: "640x480"
                frameRate: 30
                mediaContainer: "mp4"
            }
        }
    }

    Repeater {
        model: appControl.buttonModel
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

    ListModel {
        id: pageModel
        ListElement {
            title: "Zoom & Focus"
            page: "content/ZoomFocus.qml"
        }
        ListElement {
            title: "Pan & Tilt"
            page: "content/SliderPage.qml"
        }
        ListElement {
            title: "Camera & Filter"
            page: "content/ProgressBarPage.qml"
        }
        ListElement {
            title: "FOV"
            page: "content/TabBarPage.qml"
        }
        ListElement {
            title: "Settings"
            page: "content/TextInputPage.qml"
        }
        ListElement {
            title: "Advanced Settings"
            page: "content/ListPage.qml"
        }
        ListElement {
            title: "Menu"
            page: "content/ButtonPage.qml"
        }
        ListElement {
            title: "Illuminator"
            page: "content/ButtonPage.qml"
        }
        ListElement {
            title: "Sending Mode"
            page: "content/ButtonPage.qml"
        }
        ListElement {
            title: "Connections"
            page: "content/Connections.qml"
        }
    }

    StackView {
        id: stackView
        width: 400
        visible: false
        height: root.height
        anchors.fill: parent
        background: Rectangle {
            id: background
            color: "black"
            opacity: 0.6
            width: 400
        }

        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop()
                             event.accepted = true
                         }

        initialItem: Item {
            width: /*parent.width*/ 400
            height: parent.height
            ListView {
                model: pageModel
                anchors.fill: parent
                delegate: AndroidDelegate {
                    text: title
                    onClicked: {
                        stackView.push(Qt.resolvedUrl(page))
                        background.color = "transparent"
                    }
                }
            }
        }
    }

    //    Drawer {
    //        id: drawer
    //        width: 0.4 * root.width
    //        height: root.height

    //        contentItem: Item {
    //            anchors.fill: parent

    //            ColumnLayout {
    //                anchors.fill: parent
    //                RowLayout {
    //                    Layout.fillWidth: true
    //                    ToolButton {
    //                        text: qsTr("<")
    //                        onClicked: drawer.close()

    //                    }
    //                    Label {
    //                        text: "Menu"

    //                    }
    //                }
    //                MenuSeparator {
    //                    Layout.fillWidth: true
    //                }
    //                ListView {
    //                    Layout.fillWidth: true
    //                    Layout.fillHeight: true
    //                    model: ContactModel {}

    //                    focus: true
    //                    delegate: ColumnLayout {
    //                        width: drawer.width
    //                        height: 60

    //                        RowLayout {
    //                            Layout.fillHeight: true
    //                            Label {
    //                                text: name
    //                            }
    //                            Item {
    //                                Layout.fillWidth: true
    //                            }
    //                            ToolButton {
    //                                text: qsTr(">")
    //                            }
    //                        }
    //                        Rectangle {
    //                            width: parent.width
    //                            height: 1
    //                        }
    //                    }
    //                }
    //                Item {
    //                    Layout.fillHeight: true
    //                    Layout.fillWidth: true
    //                }
    //            }
    //        }
    //    }
}
