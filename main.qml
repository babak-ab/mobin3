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
            icon.height: 70
            icon.width: 70
            background: Rectangle {
                color: "black"
                opacity: 0.6
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

                        stackView.anchors.topMargin = 120
                        stackView.anchors.leftMargin = 30
                    } else {
                        menuButton.icon.source = "qrc:/Images/menu-icon.png"
                        menuButton.icon.color = "transparent"
                    }
                }
            }
        }

        //        Button {
        //            text: "Record"
        //            checkable: true
        //            highlighted: checked
        //            onClicked: {
        //                if (checked) {
        //                    var dt = new Date().toLocaleString(Qt.locale(),
        //                                                       "yyyy-MM-dd_hh_mm_ss")

        //                    camera.fileName = camera.path + "video_" + dt + ".mp4"

        //                    console.log("fileName: ", camera.fileName)
        //                    camera.videoRecorder.record()
        //                } else {
        //                    camera.videoRecorder.stop()
        //                }
        //            }
        //        }
    }

    VideoRender {
        id: videoRender
        anchors.fill: parent
        source: camera
        Camera {
            id: camera
            captureMode: Camera.CaptureVideo
            property string fileName: "test.mp4"
            property string path: "C:/Users/Morteza/Desktop/record/"
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

    Column {
        anchors.fill: parent
        Button {
            id: recordButton
            icon.source: "qrc:/Images/record-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 15

            checkable: true
            highlighted: checked

            background: Rectangle {
                color: "black"
                opacity: 0.5
            }

            onClicked: {
                if (checked) {
                    var dt = new Date().toLocaleString(Qt.locale(),
                                                       "yyyy-MM-dd_hh_mm_ss")

                    camera.fileName = camera.path + "video_" + dt + ".mp4"

                    console.log("fileName: ", camera.fileName)
                    camera.videoRecorder.record()
                } else {
                    camera.videoRecorder.stop()
                }
            }
        }



        //        Button {
        //            id: testButton
        //            contentItem: Text {
        //                text: "Zoom In"
        //                color: "white"
        //                opacity: 1.0
        //                horizontalAlignment: Text.AlignHCenter
        //                verticalAlignment: Text.AlignVCenter
        //                font.pixelSize: 25
        //                font.bold: true
        //                elide: Text.ElideRight
        //            }

        //            background: Rectangle {
        //                implicitWidth: 180
        //                implicitHeight: 60
        //                color: zoomInButton.down ? "red" : "black"
        //                opacity: 0.5
        //                radius: 5
        //                border.color: "white"
        //            }

        //            onPressed: {
        //                appControl.serialControl.zoomIn()
        //            }

        //            onReleased: {
        //                appControl.serialControl.zoomStop()
        //            }
        //        }

    }

    ButtonGroup {

        buttons: column.children
        onClicked: console.log("clicked:", button.text)
    }

    Column {

        id: column
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 15

        Button {
            id: zoomInButton
            icon.source: "qrc:/Images/zoom-in-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                opacity: 0.5
                color: zoomInButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.zoomIn()
            }

            onReleased: {
                appControl.serialControl.zoomStop()
            }
        }

        Button {
            id: zoomOutButton
            icon.source: "qrc:/Images/zoom-out-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                color: zoomOutButton.down ? "red" : "black"
                opacity: 0.5
            }


            onPressed: {
                appControl.serialControl.zoomOut()
            }

            onReleased: {
                appControl.serialControl.zoomStop()
            }
        }

        Button {
            id: focusFarButton
            icon.source: "qrc:/Images/focus-far.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                opacity: 0.5
                color: focusFarButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.focusFar()
            }

            onReleased: {
                appControl.serialControl.focusStop()
            }
        }

        Button {
            id: focusNearButton
            icon.source: "qrc:/Images/focus-near.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                opacity: 0.5
                color: focusNearButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.focusNear()
            }

            onReleased: {
                appControl.serialControl.focusStop()
            }
        }

        Button {
            id: leftPanButton
            icon.source: "qrc:/Images/left-arrow.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                color: leftPanButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.panLeft()
            }

            onReleased: {
                appControl.serialControl.panStop()
            }
        }

        Button {
            id: rightPanButton
            icon.source: "qrc:/Images/right-arrow.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                color: rightPanButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.panRight()
            }

            onReleased: {
                appControl.serialControl.panStop()
            }
        }

        Button {
            id: upTiltButton
            icon.source: "qrc:/Images/up-arrow.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                color: upTiltButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.tiltUp()
            }

            onReleased: {
                appControl.serialControl.tiltStop()
            }
        }

        Button {
            id: downTiltButton
            icon.source: "qrc:/Images/down-arrow.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            background: Rectangle {
                color: downTiltButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.tiltDown()
            }

            onReleased: {
                appControl.serialControl.tiltStop()
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
            page: "content/PanTilt.qml"
        }
        ListElement {
            title: "Camera & Filter"
            page: "content/CameraFilter.qml"
        }
        ListElement {
            title: "FOV"
            page: "content/FOV.qml"
        }
        ListElement {
            title: "Settings"
            page: "content/Settings.qml"
        }
        ListElement {
            title: "Advanced Settings"
            page: "content/AdvancedSettings.qml"
        }
        ListElement {
            title: "Menu"
            page: "content/Menu.qml"
        }
        ListElement {
            title: "Illuminator"
            page: "content/Illuminator.qml"
        }
        ListElement {
            title: "Sending Mode"
            page: "content/SendingMode.qml"
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
}
