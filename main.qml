import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import AppControl 1.0
import SerialControl 1.0
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
    }

    VideoRender {
        id: videoRender
        anchors.fill: parent
        source: appControl.videoAdapter
    }

    Row {
        spacing: 20
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 15

        Text {
            text: "NR: " + appControl.serialControl.noiseReductionType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "Defog: " + appControl.serialControl.defogType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "Gamma: " + appControl.serialControl.gammaType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {

            text: "Filter: " + appControl.serialControl.filterType.toString() + ","

            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }


        Text {
            text: "FOV: " + appControl.serialControl.fovPosition / 1000 + " °"
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        GroupBox {
            background: Rectangle {
                implicitWidth: 20
                implicitHeight: 20
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Row {
                spacing: 10

                Text {
                    text: "Ratio:  " + appControl.serialControl.illuminatorAngleOffset / 100.0
                    font.family: "Helvetica"
                    font.pointSize: 12
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                }

                Slider {
                    id: angleOffsetSlider
                    value: appControl.serialControl.illuminatorAngleOffset
                    from: 50
                    to: 100

                    implicitWidth: 80

                    onValueChanged: {
                        if (angleOffsetSlider.value
                                !== appControl.serialControl.illuminatorAngleOffset)
                            appControl.serialControl.setIlluminatorAngleOffset(
                                        angleOffsetSlider.value)
                    }
                }
            }
        }

        GroupBox {
            font.pixelSize: 20

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 40
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Row {
                spacing: 15

                RadioButton {
                    id: continuousZoom
                    text: "Cont. Zoom"
                    checked: appControl.serialControl.selectedCamera
                             === SerialControl.CameraSelection_ContinuousZoom
                    font.pixelSize: 15

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 40
                        color: continuousZoom.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedCamera(
                                    SerialControl.CameraSelection_ContinuousZoom)
                    }
                }

                RadioButton {
                    id: spotter
                    text: "Spotter"
                    checked: appControl.serialControl.selectedCamera
                             === SerialControl.CameraSelection_Spotter
                    font.pixelSize: 15

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 40
                        color: spotter.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedCamera(
                                    SerialControl.CameraSelection_Spotter)
                    }
                }
            }
        }

        GroupBox {
            font.pixelSize: 20

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 40
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Row {
                spacing: 15

                RadioButton {
                    id: colorFilter
                    text: "Color"
                    checked: appControl.serialControl.selectedFilter === SerialControl.Color
                    font.pixelSize: 15
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 40
                        color: colorFilter.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedFilter(
                                    SerialControl.Color)
                    }
                }

                RadioButton {
                    id: nirFilter
                    text: "NIR"
                    checked: appControl.serialControl.selectedFilter === SerialControl.NIR
                    font.pixelSize: 15
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 40
                        color: nirFilter.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedFilter(
                                    SerialControl.NIR)
                    }
                }

                RadioButton {
                    id: laserFilter
                    text: "Laser"
                    checked: appControl.serialControl.selectedFilter === SerialControl.F1064
                    font.pixelSize: 15
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 40
                        color: laserFilter.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedFilter(
                                    SerialControl.F1064)
                    }
                }
            }
        }

        GroupBox {
            font.pixelSize: 20
            anchors.margins: 0

            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 40
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Row {
                spacing: 10

                RadioButton {
                    id: autoFocusButton
                    text: "AF"
                    checked: appControl.serialControl.focusMode
                    font.pixelSize: 15

                    background: Rectangle {
                        implicitWidth: 40
                        implicitHeight: 40
                        color: autoFocusButton.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setFocusMode(
                                    autoFocusButton.checked)
                    }
                }

                RadioButton {
                    id: manualFocusButton
                    text: "MF"
                    checked: !appControl.serialControl.focusMode
                    font.pixelSize: 15

                    background: Rectangle {
                        implicitWidth: 40
                        implicitHeight: 40
                        color: manualFocusButton.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setFocusMode(
                                    autoFocusButton.checked)
                    }
                }
            }
        }

        Button {
            id: recordButton
            icon.source: "qrc:/Images/record-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60

            checkable: true
            highlighted: checked

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 60
                color: recordButton.checked ? "red" : "black"
                opacity: 0.5
                radius: 5
            }

            onClicked: {
                if (checked) {
                    appControl.startRecord()

                } else {
                    appControl.stopRecord()
                }
            }
        }
    }

    ButtonGroup {

        buttons: column.children
        onClicked: console.log("clicked:", button.text)
    }

    Column {

        id: column
        spacing: 15
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

            onDownChanged: {
                if (!zoomInButton.pressed)
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

            onDownChanged: {
                if (!zoomOutButton.pressed)
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

            onDownChanged: {
                if (!focusFarButton.pressed)
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

            onDownChanged: {
                if (!focusNearButton.pressed)
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

            onDownChanged: {
                if (!leftPanButton.pressed)
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

            onDownChanged: {
                if (!rightPanButton.pressed)
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

            onDownChanged: {
                if (!upTiltButton.pressed)
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

            onDownChanged: {
                if (!downTiltButton.pressed)
                    appControl.serialControl.tiltStop()
            }
        }

        Text {
            text: appControl.appVersion
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline
            styleColor: "black"
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
        ListElement {
            title: "Exit"
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

        initialItem: ColumnLayout {
            width: /*parent.width*/ 400
            height: parent.height
            ListView {
                model: pageModel
                anchors.fill: parent
                delegate: AndroidDelegate {
                    text: title
                    onClicked: {
                        if (title == "Exit") {
                            exitDialog.open()
                        } else {

                            stackView.push(Qt.resolvedUrl(page))
                            background.color = "transparent"
                        }
                    }
                }
            }
        }
    }

    Dialog {
        id: exitDialog
        standardButtons: Dialog.Ok | Dialog.Cancel
        parent: Overlay.overlay

        Text {
            text: "Are you sure to exit the application?"
            font.family: "Helvetica"
            font.pointSize: 15
            color: "white"
            style: Text.Outline;
            styleColor: "black"
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 60
            color: "black"
            opacity: 0.5
            radius: 5
            border.color: "white"
        }

        x: parent ? ((parent.width - width) / 2) : 0
        y: parent ? ((parent.height - height) / 2) : 0

        onAccepted: {
            Qt.quit()
        }
    }

    RecordIndicator {}

}

