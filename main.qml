import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.14
import AppControl 1.0
import SerialControl 1.0
import QtMultimedia 5.12
import "content"
import Reticle 1.0

import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.1
import QtQuick.Controls 2.12 as QQC2
import QtQml 2.12

ApplicationWindow {
    id: root
    visible: true
    //     width: 1280
    //     height: 720
    title: qsTr("Mobin3")

    screen: Qt.application.screens[1]

    property bool isSecondPageOpen: false

    onVisibleChanged: {
        if (visible) {
            root.showFullScreen()
        }
    }

    Rectangle {
        color: "#212126"
        anchors.fill: parent
    }

    VideoRender {
        id: videoRender
        anchors.fill: root
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

    GridLayout
    {
        anchors.fill: parent

        columns: 6
        rows: 10

        //  ===========================
        //      Top Row
        //  ===========================
        Button {
            id: menuButton

            Layout.column: 0
            Layout.row: 0

            icon.source: "Images/menu-icon.png"
            icon.color: "transparent"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42
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

        GroupBox {
            Layout.column: 1
            Layout.row: 0

            padding: 5

            background: Rectangle {
                implicitWidth: 12
                implicitHeight: 12
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            implicitWidth: 570

            RowLayout {
                anchors
                {
                    fill: parent
                    margins: 5
                }

                spacing: 5

                Text {
                    text: "Illuminator:"
                    font.family: "Helvetica"
                    font.pointSize: 12
                    font.bold: true
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                }

                Switch {
                    id: illuminatorOnOffSwitch
                    checked: appControl.serialControl.illuminator

                    height: 10

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
                    font.pointSize: 12
                    font.bold: true
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                }

                Slider {
                    id: angleOffsetSlider

                    implicitWidth: 80

                    value: appControl.serialControl.illuminatorAngleOffset
                    from: 50
                    to: 100

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
                    font.pointSize: 12
                    font.bold: true
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                }

                Slider {
                    id: brightnessSlider

                    implicitWidth: 80

                    value: appControl.serialControl.illuminatorBrightnessLevel
                    from: 0
                    to: 255

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
            Layout.column: 2
            Layout.row: 0

            padding: 3

            background: Rectangle {
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            RowLayout {
                anchors.fill: parent

                spacing: 3

                RadioButton {
                    id: continuousZoom
                    text: "Cont. Zoom"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera
                             === SerialControl.CameraSelection_ContinuousZoom
                    font.pointSize: 12
                    font.bold: true

                    background: Rectangle {
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
                    font.pointSize: 12
                    font.bold: true

                    background: Rectangle {
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
            Layout.column: 3
            Layout.row: 0

            padding: 3

            background: Rectangle {
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            RowLayout {
                anchors.fill: parent

                spacing: 3

                RadioButton {
                    id: colorFilter
                    text: "Color"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedFilter === SerialControl.Color
                    font.pointSize: 12
                    font.bold: true
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

                    background: Rectangle {
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
                    font.pointSize: 12
                    font.bold: true
                    visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                             || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

                    background: Rectangle {
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
            Layout.column: 4
            Layout.row: 0

            padding: 3

            background: Rectangle {
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            RowLayout {
                anchors.fill: parent

                spacing: 3

                RadioButton {
                    id: autoFocusButton
                    text: "AF"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.focusMode
                    font.pointSize: 12
                    font.bold: true

                    background: Rectangle {
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
                    font.pointSize: 12
                    font.bold: true

                    background: Rectangle {
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

            Layout.column: 5
            Layout.row: 0

            icon.source: "qrc:/Images/record-icon.png"
            icon.color: "transparent"
            icon.height: 42
            icon.width: 42
            font.family: "Helvetica"

            checkable: true
            highlighted: checked

            background: Rectangle {
                color: recordButton.checked ? "red" : "transparent"
                opacity: 0.5
                radius: 5
            }

            onReleased: {
                console.log("video record button ", checked)
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

        //  ===========================
        //      Right Column
        //  ===========================
        Button {
            id: zoomInButton

            Layout.column: 5
            Layout.row: 1

            icon.source: "qrc:/Images/zoom-in-icon.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 2

            icon.source: "qrc:/Images/zoom-out-icon.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 3

            icon.source: "qrc:/Images/focus-far.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 4

            icon.source: "qrc:/Images/focus-near.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 5

            icon.source: "qrc:/Images/right-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 6

            icon.source: "qrc:/Images/left-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 7

            icon.source: "qrc:/Images/up-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

            Layout.column: 5
            Layout.row: 8

            icon.source: "qrc:/Images/down-arrow.png"
            icon.color: "white"
            font.family: "Helvetica"
            icon.height: 42
            icon.width: 42

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

        //  ===========================
        //      Bottom Row
        //  ===========================
        RowLayout {
            Layout.column: 0
            Layout.columnSpan: 5
            Layout.row: 9
            Layout.alignment: Qt.AlignHCenter

            spacing: 30

            Text {
                text: "NR: " + appControl.serialControl.noiseReductionType.toString() + ","
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }

            Text {
                text: "Defog: " + appControl.serialControl.defogType.toString() + ","
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }

            Text {
                text: "Gamma: " + appControl.serialControl.gammaType.toString() + ","
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }

            Text {
                text: "FOV: " + (appControl.serialControl.fovPosition / 1000).toFixed(2) + " °,"
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }

            Text {
                text: "Focus: " + (appControl.serialControl.focusPosition)
                font.family: "Helvetica"
                font.pointSize: 15
                font.bold: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }
        }

        Text {
            text: appControl.appVersion

            Layout.column: 5
            Layout.row: 9
            Layout.alignment: Qt.AlignHCenter

            font.family: "Helvetica"
            font.pointSize: 12
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
        width: 240
        visible: false
        height: root.height - 230

        y: (root.height / 2) - (height / 2)
        x: 20

        font.family: "Helvetica"
        background: Rectangle {
            id: background
            color: "black"
            opacity: 0.6
            width: stackView.width
        }

        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop()
                             event.accepted = true
                         }

        initialItem: ColumnLayout {
            width: 240
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
        font.pointSize: 10

        Text {
            text: "Are you sure to exit the application?"
            font.family: "Helvetica"
            font.pointSize: 10
            color: "white"
            style: Text.Outline;
            styleColor: "black"
        }

        background: Rectangle {
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
        font.pointSize: 10

        Text {
            text: "Are you sure to power on the illuminator?"
            font.family: "Helvetica"
            font.pointSize: 10
            color: "white"
            style: Text.Outline;
            styleColor: "black"
        }

        background: Rectangle {
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
        font.pointSize: 10
        width: 270
        height: 168

        spacing: 40

        background: Rectangle {
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
                font.pointSize: 10
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
                    font.pointSize: 10
                    color: "white"
                    style: Text.Outline
                    styleColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                }

                QQC2.ComboBox {
                    id: control
                    model: appControl.serialPortList
                    font.family: "Helvetica"
                    width: 90

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
                font.pointSize: 10
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
                font.pointSize: 10
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

        width: 180
        height: 60
        standardButtons: Dialog.Ok

        onAccepted: passwordDialog.close()
    }

    RecordIndicator
    {
        id: recordIndicator

        x: zoomInButton.x - 90
        y: zoomInButton.y
    }

    IlluminatorIndicator
    {
        anchors.horizontalCenter: recordIndicator.horizontalCenter
        anchors.top: recordIndicator.bottom
        anchors.topMargin: 20
    }

    function handleSerialBoardCommand(command)
    {
        var delay = 1000

        if (command === 0) // Normal
        {
            if (appControl.lastSerialCommand === 13 ||
                    appControl.lastSerialCommand === 14)
            {
                appControl.serialControl.tiltStop()
            }
            else if (appControl.lastSerialCommand === 15 ||
                     appControl.lastSerialCommand === 16)
            {
                appControl.serialControl.panStop()
            }
            else if (appControl.lastSerialCommand === 17 ||
                     appControl.lastSerialCommand === 18)
            {
                appControl.serialControl.zoomStop()
            }
            else if (appControl.lastSerialCommand === 19 ||
                     appControl.lastSerialCommand === 20)
            {
                appControl.serialControl.focusStop()
            }
        }
        else if (command === 1) // Continuous
        {
            continuousZoom.clicked()
        }
        else if (command === 2) // Spotter
        {
            spotter.clicked()
        }
        else if (command === 3) // NIR Filter
        {
            if (colorFilter.checked === true)
            {
                nirFilter.clicked()
            }
            else
            {
                colorFilter.clicked()
            }
        }
        else if (command === 4) // Snapshot
        {
            appControl.takeSnapshot()
        }
        else if (command === 5) // Video Record
        {
            recordButton.checked = !recordButton.checked
            recordButton.released();
        }
        else if (command === 6) // Laser
        {
            illuminatorOnOffSwitch.clicked()
        }
        else if (command === 7) // Beam angle +
        {
            appControl.serialControl.setIlluminatorLargerAngle()
        }
        else if (command === 8) // Beam angle -
        {
            appControl.serialControl.setIlluminatorSmallerAngle()
        }
        else if (command === 9) // Intensity +
        {
            var intensityPlus = appControl.serialControl.illuminatorBrightnessLevel

            intensityPlus = intensityPlus + 10

            if (intensityPlus > 255)
            {
                intensityPlus = 255
            }

            appControl.serialControl.setIlluminatorBrightness(intensityPlus)
        }
        else if (command === 10) // Intensity -
        {
            var intensityMinus = appControl.serialControl.illuminatorBrightnessLevel

            intensityMinus = intensityMinus - 10

            if (intensityMinus < 0)
            {
                intensityMinus = 0
            }

            appControl.serialControl.setIlluminatorBrightness(intensityMinus)
        }
        else if (command === 11) // Speed +
        {
            var speedPlus = appControl.serialControl.panTiltSpeed

            speedPlus = speedPlus + 10

            if (speedPlus > 255)
            {
                speedPlus = 255
            }

            appControl.serialControl.setPanTiltSpeed(speedPlus)

        }
        else if (command === 12) // Speed -
        {
            var speedMinus = appControl.serialControl.panTiltSpeed

            speedMinus = speedMinus - 10

            if (speedMinus < 0)
            {
                speedMinus = 0
            }

            appControl.serialControl.setPanTiltSpeed(speedMinus)
        }
        else if (command === 13) // Move Up
        {
            appControl.serialControl.tiltUp()
        }
        else if (command === 14) // Move Down
        {
            appControl.serialControl.tiltDown()
        }
        else if (command === 15) // Move Left
        {
            appControl.serialControl.panLeft()
        }
        else if (command === 16) // Move Right
        {
            appControl.serialControl.panRight()
        }
        else if (command === 17) // Zoom +
        {
            appControl.serialControl.zoomIn()
        }
        else if (command === 18) // Zoom -
        {
            appControl.serialControl.zoomOut()
        }
        else if (command === 19) // Focus +
        {
            appControl.serialControl.focusFar()
        }
        else if (command === 20) // Focus -
        {
            appControl.serialControl.focusNear()
        }
        else if (command === 21) // Auto Focus
        {
            if (autoFocusButton.checked === true)
            {
                manualFocusButton.clicked()
            }
            else
            {
                autoFocusButton.clicked()
            }
        }

        appControl.lastSerialCommand = command
    }

    function sendWithDelay(delay, lambda)
    {
        var timer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", root);
        timer.interval = delay;
        timer.repeat = false;
        timer.triggered.connect(lambda);
        timer.start();
    }
}
