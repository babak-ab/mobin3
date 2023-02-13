/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.12
import QtQuick.Controls.Styles 1.1

import QtQuick.Layouts 1.14

import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls 1.2 as QQC1

import AppControl 1.0
import SerialControl 1.0

Item {

    property real progress: 0
    SequentialAnimation on progress {
        loops: Animation.Infinite
        running: true
        NumberAnimation {
            from: 0
            to: 1
            duration: 3000
        }
        NumberAnimation {
            from: 1
            to: 0
            duration: 3000
        }
    }

    Column {
        spacing: 40

        Text {
            text: "  Illuminator: "
            font.family: "Helvetica"
            font.pointSize: 25
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        Row {
            spacing: 25
            Text {
                text: "Illuminator Power: "
                font.family: "Helvetica"
                font.pointSize: 15
                color: "white"
                style: Text.Outline;
                styleColor: "black"
                            anchors.verticalCenter: parent.verticalCenter
            }

            QQC1.Switch {
                id: illuminatorOnOffSwitch
                style: switchStyle
                checked: appControl.serialControl.illuminator

                onClicked: {

                    if (!checked) {

                        illuminatorOnOffSwitch.checked = false
                        if (illuminatorOnOffSwitch.checked !== appControl.serialControl.illuminator) {
                            appControl.serialControl.enableIlluminator(
                                        illuminatorOnOffSwitch.checked)
                        }
                    } else {
                        if (illuminatorOnOffSwitch.checked !== appControl.serialControl.illuminator)
                            illuminatorDialog.open()
                    }
                }
            }
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Illuminator Brightness: "
                color: "white"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 60
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Column {
                spacing: 20

                Text {
                    id: brightnessValueText
                    text: "Brightness:  " + appControl.serialControl.illuminatorBrightnessLevel
                    font.family: "Helvetica"
                    font.pointSize: 15
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                }

                QQC1.Slider {
                    id: brightnessSlider
                    anchors.margins: 20
                    anchors.topMargin: 50
                    style: sliderTouchStyle
                    value: appControl.serialControl.illuminatorBrightnessLevel
                    minimumValue: 0
                    maximumValue: 255

                    onPressedChanged: {

                        if (!pressed) {

                            if (brightnessSlider.value !== appControl.serialControl.illuminatorBrightnessLevel) {
                                appControl.serialControl.setIlluminatorBrightness(
                                            brightnessSlider.value)
                            }
                        }
                    }

                    onValueChanged: {

                        brightnessValueText.text = "Brightness:  " + value.toFixed(0)
                    }
                }
            }
        }

        Row {
            spacing: 25
            QQC2.Button {
                id: smallerAngleButton
                contentItem: Text {
                    text: "Smaller Angle"
                    color: "white"
                    opacity: 1.0
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 60
                    color: smallerAngleButton.down ? "red" : "black"
                    opacity: 0.5
                    radius: 5
                    border.color: "white"
                }

                onReleased: {
                    appControl.serialControl.setIlluminatorSmallerAngle()
                }
            }

            QQC2.Button {
                id: largerAngleButton
                contentItem: Text {
                    text: "Larger Angle"
                    color: "white"
                    opacity: 1.0
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 25
                    font.bold: true
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 60
                    color: largerAngleButton.down ? "red" : "black"
                    opacity: 0.5
                    radius: 5
                    border.color: "white"
                }

                onReleased: {
                    appControl.serialControl.setIlluminatorLargerAngle()
                }
            }
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Angle Ratio of the Illuminator: "
                color: "white"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 60
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            Column {
                spacing: 20

                Text {
                    id: ratioValueText
                    text: "Ratio:  " + (appControl.serialControl.illuminatorAngleOffset / 100.0).toFixed(2)
                    font.family: "Helvetica"
                    font.pointSize: 15
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                }

                QQC1.Slider {
                    id: angleOffsetSlider
                    anchors.margins: 20
                    anchors.topMargin: 50
                    style: sliderTouchStyle
                    value: appControl.serialControl.illuminatorAngleOffset
                    updateValueWhileDragging: false
                    minimumValue: 0
                    maximumValue: 100

                    onPressedChanged: {

                        if (!pressed) {

                            if (angleOffsetSlider.value < 50) {
                                angleOffsetSlider.value = 50
                            }

                            if (angleOffsetSlider.value !== appControl.serialControl.illuminatorAngleOffset)
                                appControl.serialControl.setIlluminatorAngleOffset(
                                            angleOffsetSlider.value)
                        }
                    }

                    onValueChanged: {

                        if (pressed) {

                            ratioValueText.text = "Ratio:  " + (value / 100.0).toFixed(2)
                        }
                    }
                }
            }
        }
    }

    QQC2.Dialog {
        id: illuminatorDialog
        standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel
        parent: QQC2.Overlay.overlay

        Text {
            text: "Are you sure to power on the illuminator?"
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
            if (illuminatorOnOffSwitch.checked !== appControl.serialControl.illuminator) {
                appControl.serialControl.enableIlluminator(
                            illuminatorOnOffSwitch.checked)
            }
        }

        onRejected: {
            illuminatorOnOffSwitch.checked = false
        }
    }

    Component {
        id: touchStyle
        ButtonStyle {
            panel: Item {
                implicitHeight: 60
                implicitWidth: 180
                BorderImage {
                    anchors.fill: parent
                    antialiasing: true
                    border.bottom: 8
                    border.top: 8
                    border.left: 8
                    border.right: 8
                    anchors.margins: control.pressed ? -4 : 0
                    source: control.pressed ? "../Images/button_pressed.png" : "../Images/button_default.png"

                    Rectangle {
                        implicitWidth: parent.width
                        implicitHeight: parent.height
                        opacity: 0.5
                        border.width: 1
                        radius: 2
                        color: "black"
                    }

                    Text {
                        text: control.text
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 25
                        renderType: Text.NativeRendering
                    }
                }
            }
        }
    }

    Component {
        id: sliderTouchStyle
        SliderStyle {
            handle: Rectangle {
                width: 30
                height: 30
                radius: height
                antialiasing: true
                color: Qt.lighter("#fc0303", 1.2)
            }

            groove: Item {
                implicitHeight: 50
                implicitWidth: 400
                Rectangle {
                    height: 8
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#444"
                    opacity: 0.8
                    Rectangle {
                        antialiasing: true
                        radius: 1
                        color: "#fc0303"
                        height: parent.height
                        width: parent.width * control.value / control.maximumValue
                    }
                }
            }
        }
    }

    Component {
        id: switchStyle
        SwitchStyle {

            groove: Rectangle {
                implicitHeight: 50
                implicitWidth: 152
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width/2 - 2
                    height: 20
                    anchors.margins: 2
                    color: control.checked ? "#468bb7" : "#222"
                    Behavior on color {ColorAnimation {}}
                    Text {
                        font.pixelSize: 23
                        color: "white"
                        anchors.centerIn: parent
                        text: "ON"
                    }
                }
                Item {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    Text {
                        font.pixelSize: 23
                        color: "white"
                        anchors.centerIn: parent
                        text: "OFF"
                    }
                }
                color: "#222"
                border.color: "#444"
                border.width: 2
            }
            handle: Rectangle {
                width: parent.parent.width/2
                height: control.height
                color: "#444"
                border.color: "#555"
                border.width: 2
            }
        }
    }

}
