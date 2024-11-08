import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import AppControl 1.0
import SerialControl 1.0
import QtMultimedia 5.12
import "content"
import Reticle 1.0

import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.1
import QtQuick.Controls 2.12 as QQC2

ApplicationWindow {
    id: root
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Mobin3")
    property  bool isSecondPageOpen: false

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
            font.family: "Helvetica"
            icon.height: 90
            icon.width: 90
            background: Rectangle {
                color: "black"
                opacity: 0.6
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {

                    if (isSecondPageOpen === false) {

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
                    } else {

                        menuButton.icon.source = "qrc:/Images/navigation_previous_item.png"
                        menuButton.icon.color = "transparent"
                        stackView.pop()
                        background.color = "black"

                        stackView.anchors.topMargin = 120
                        stackView.anchors.leftMargin = 30

                        isSecondPageOpen = false
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

    Reticle {
        id: reticle
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: parent.height
        visible: appControl.reticleVisible

        onSigDataChanged:
        {
            reticle.update()
        }
    }

    Row {
        spacing: 25
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 15

        Text {
            text: "NR: " + appControl.serialControl.noiseReductionType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 25
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "Defog: " + appControl.serialControl.defogType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 25
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "Gamma: " + appControl.serialControl.gammaType.toString() + ","
            font.family: "Helvetica"
            font.pointSize: 25
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "FOV: " + (appControl.serialControl.fovPosition / 1000).toFixed(2) + " °,"
            font.family: "Helvetica"
            font.pointSize: 25
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }

        Text {
            text: "Focus: " + (appControl.serialControl.focusPosition)
            font.family: "Helvetica"
            font.pointSize: 25
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
        }
    }


    Row {
        spacing: 20
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 15

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
                spacing: 15
                Text {
                    text: "Illuminator:"
                    font.family: "Helvetica"
                    font.pointSize: 20
                    font.bold: true
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id: illuminatorOnOffSwitch
                    checked: appControl.serialControl.illuminator

                    onReleased: {

                        if (!checked) {

                            if (illuminatorOnOffSwitch.checked !== appControl.serialControl.illuminator) {
                                appControl.serialControl.enableIlluminator(
                                            illuminatorOnOffSwitch.checked)
                            }
                        } else {
                            illuminatorDialog.open()
                        }
                    }
                }


                Text {
                    id: ratioAngleOffsetText
                    text: ", Ratio: " + (appControl.serialControl.illuminatorAngleOffset / 100.0).toFixed(2)
                    font.family: "Helvetica"
                    font.pointSize: 18
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                }

                Slider {
                    id: angleOffsetSlider
                    value: appControl.serialControl.illuminatorAngleOffset
                    from: 50
                    to: 100

                    implicitWidth: 100

                    onPressedChanged: {

                        if (!pressed) {
                            if (angleOffsetSlider.value
                                    !== appControl.serialControl.illuminatorAngleOffset)
                                appControl.serialControl.setIlluminatorAngleOffset(
                                            angleOffsetSlider.value)
                        }
                    }

                    onValueChanged: {

                        ratioAngleOffsetText.text = ", Ratio: " + (angleOffsetSlider.value / 100.0).toFixed(2)
                    }
                }

                Text {
                    id: brightnessValueText
                    text: ", Brightness: " + appControl.serialControl.illuminatorBrightnessLevel
                    font.family: "Helvetica"
                    font.pointSize: 18
                    font.bold: true
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                    width: 200
                    anchors.verticalCenter: parent.verticalCenter
                }

                Slider {
                    id: brightnessSlider
                    anchors.margins: 20
                    anchors.topMargin: 50
                    value: appControl.serialControl.illuminatorBrightnessLevel
                    from: 0
                    to: 255
                    implicitWidth: 100

                    onPressedChanged: {

                        if (!pressed) {

                            if (brightnessSlider.value !== appControl.serialControl.illuminatorBrightnessLevel) {
                                appControl.serialControl.setIlluminatorBrightness(
                                            brightnessSlider.value)
                            }
                        }
                    }

                    onValueChanged: {

                        brightnessValueText.text = ", Brightness:  " + value.toFixed(0)
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
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera
                             === SerialControl.CameraSelection_ContinuousZoom
                    font.pixelSize: 18
                    font.bold: true

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
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera
                             === SerialControl.CameraSelection_Spotter
                    font.pixelSize: 18
                    font.bold: true

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
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedFilter === SerialControl.Color
                    font.pixelSize: 18
                    font.bold: true
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

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
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedFilter === SerialControl.NIR
                    font.pixelSize: 18
                    font.bold: true
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

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
                    font.family: "Helvetica"
                    checked: appControl.serialControl.focusMode
                    font.pixelSize: 18
                    font.bold: true

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
                    font.family: "Helvetica"
                    checked: !appControl.serialControl.focusMode
                    font.pixelSize: 18
                    font.bold: true

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
            icon.color: "transparent"
            icon.height: 60
            icon.width: 60
            font.family: "Helvetica"

            checkable: true
            highlighted: checked

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 60
                color: recordButton.checked ? "red" : "transparent"
                opacity: 0.5
                radius: 5
            }

            onReleased: {
                if (checked) {
                    recordButton.icon.source = ""
                    recordButton.icon.source = "qrc:/Images/stop-record-icon.png"
                    appControl.startRecord()

                } else {
                    recordButton.icon.source = ""
                    recordButton.icon.source = "qrc:/Images/record-icon.png"
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
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                opacity: 0.5
                color: zoomInButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.zoomIn()
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
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                color: zoomOutButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.zoomOut()
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
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                opacity: 0.5
                color: focusFarButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.focusFar()
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
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                opacity: 0.5
                color: focusNearButton.down ? "red" : "black"
            }

            onPressed: {
                appControl.serialControl.focusNear()
            }

            onDownChanged: {
                if (!focusNearButton.pressed)
                    appControl.serialControl.focusStop()
            }
        }

        Button {
            id: rightPanButton
            icon.source: "qrc:/Images/right-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                color: rightPanButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.panRight()
            }

            onDownChanged: {
                if (!rightPanButton.pressed)
                    appControl.serialControl.panStop()
            }
        }

        Button {
            id: leftPanButton
            icon.source: "qrc:/Images/left-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                color: leftPanButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.panLeft()
            }

            onDownChanged: {
                if (!leftPanButton.pressed)
                    appControl.serialControl.panStop()
            }
        }

        Button {
            id: upTiltButton
            icon.source: "qrc:/Images/up-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                color: upTiltButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.tiltUp()
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
            font.family: "Helvetica"
            icon.height: 70
            icon.width: 70

            background: Rectangle {
                color: downTiltButton.down ? "red" : "black"
                opacity: 0.5
            }

            onPressed: {
                appControl.serialControl.tiltDown()
            }

            onDownChanged: {
                if (!downTiltButton.pressed)
                    appControl.serialControl.tiltStop()
            }
        }

        Text {
            text: appControl.appVersion
            font.family: "Helvetica"
            font.pointSize: 20
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
            title: "Sensor Information"
            page: "content/SensorInformation.qml"
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
        font.family: "Helvetica"
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
            width: 400
            height: parent.height
            ListView {
                id: listView
                model: pageModel
                anchors.fill: parent
                spacing: -12
                delegate: AndroidDelegate {
                    text: title

                    onClicked: {
                        if (title == "Exit") {
                            exitDialog.open()
                        } else {

                            isSecondPageOpen = true

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
        font.pointSize: 20

        Text {
            text: "Are you sure to exit the application?"
            font.family: "Helvetica"
            font.pointSize: 20
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
            appControl.stopRecord();

            Qt.quit()
        }
    }

    Dialog {
        id: illuminatorDialog
        standardButtons: Dialog.Ok | Dialog.Cancel
        parent: Overlay.overlay
        font.pointSize: 20

        Text {
            text: "Are you sure to power on the illuminator?"
            font.family: "Helvetica"
            font.pointSize: 20
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
            if (illuminatorOnOffSwitch.checked !== appControl.serialControl.illuminator) {
                appControl.serialControl.enableIlluminator(
                            illuminatorOnOffSwitch.checked)
            }
        }

        onRejected: {
            illuminatorOnOffSwitch.checked = false
        }
    }

    Dialog {
        id: serialDialog
        visible: appControl.serialControl.messageBox === "" ? false : true
        //        standardButtons: Dialog.Ok
        //        parent: Overlay.overlay
        font.pointSize: 20
        width: 450
        height: 280

        spacing: 40

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

        Column {

        Text {
            text: appControl.serialControl.messageBox
            font.family: "Helvetica"
            font.pointSize: 25
            color: "red"
            style: Text.Outline;
            styleColor: "black"
        }


            spacing: 40
            Row {
                spacing: 15
                Text {
                    text: "Serial Port: "
                    font.family: "Helvetica"
                    font.pointSize: 20
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                }

                QQC2.ComboBox {
                    id: control
                    model: appControl.serialPortList
                    font.family: "Helvetica"
                    width: 150

                    delegate: QQC2.ItemDelegate {
                        width: control.width
                        contentItem: Text {
                            text: modelData
                            color: "black"
                            font: control.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        highlighted: control.highlightedIndex === index
                    }

                    currentIndex: appControl.findSerialPortName(
                                      appControl.serialControl.serialportName())
                }
            }

            Button {
                id: controlBt
                text: appControl.serialControl.isConnected ? "Disconnect" : "Connect"
                font.family: "Helvetica"
                font.pixelSize: 32
                checkable: true
                checked: appControl.serialControl.isConnected
                anchors.horizontalCenter: parent.horizontalCenter

                contentItem: Text {
                    text: appControl.serialControl.isConnected ? "Disconnect" : "Connect"
                    font: controlBt.font
                    opacity: enabled ? 1.0 : 0.3
                    color: appControl.serialControl.isConnected ? "green" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 150
                    implicitHeight: 60
                    opacity: enabled ? 1 : 0.3
                    border.color: appControl.serialControl.isConnected ? "darkred" : "gray"
                    border.width: 1
                    radius: 2
                }

                onClicked: {

                    appControl.setSerialPortName(control.currentText)


                    if (controlBt.text == "Connect") {

                        if (appControl.connectToSerialPort()) {
                            serialDialog.close()
                        }

                    } else {
                        appControl.disconnectSerialPort()
                        serialDialog.close()
                    }
                }
            }
        }
    }

    Dialog {
        id: cameraLoginMenu
        //anchors.margins: 5
        visible: appControl.serialControl.showLoginWindow
        parent: Overlay.overlay
        Column
        {
            Text {
                text: "Camera Menu Login"
                font.family: "Helvetica"
                font.pointSize: 15
                color: "red"
                style: Text.Outline
                styleColor: "black"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Label {
                    text: "Login Password: "
                    font.family: "Helvetica"
                }
                TextField {
                    id: password
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                    placeholderText: "password"
                    font.family: "Helvetica"
                }
            }

            RowLayout {
                Layout.fillWidth: true
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    text: "OK"
                    font.family: "Helvetica"
                    Layout.fillWidth: true
                    onClicked: {
                        if (password.text == "1234") {

                            appControl.serialControl.login()
                            cameraLoginMenu.close();

                        } else {
                            passwordDialog.open()
                        }
                    }
                }
                Button {
                    text: "Cancel"
                    font.family: "Helvetica"
                    Layout.fillWidth: true

                    onClicked: {
                        //appControl.serialControl.resetCameraLoginMenu()
                        cameraLoginMenu.close()
                    }
                }
            }

        }
        x: parent ? ((parent.width - width) / 2) : 0
        y: parent ? ((parent.height - height) / 2) : 0
    }

    Dialog {
        id: passwordDialog
        title: "Password is wrong"
        font.family: "Helvetica"
        modal: true

        parent: Overlay.overlay

        x: parent ? ((parent.width - width) / 2) : 0
        y: parent ? ((parent.height - height) / 2) : 0

        width: 300
        height: 100
        standardButtons: Dialog.Ok

        onAccepted: passwordDialog.close()
    }

    RecordIndicator {}

    IlluminatorIndicator {}
}

