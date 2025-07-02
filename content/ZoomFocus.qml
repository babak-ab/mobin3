

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
import QtQuick 2.2
import QtQuick.Controls.Styles 1.1

import QtQuick.Controls 2.3 as QQC2
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Layouts 1.12

import AppControl 1.0


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

    ColumnLayout {
        anchors.fill: parent
        spacing: 9

        Text {
            text: "  Zoom & Focus: "
            font.family: "Helvetica"
            font.pointSize: 12
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.GroupBox {
            label: QQC2.Label {
                text: " Zoom: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
                font.pointSize: 12
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            ColumnLayout {
                anchors.fill: parent

                spacing: 6

                RowLayout {
                    spacing: 9

                    Layout.fillWidth: true

                    QQC2.Button {
                        id: zoomInButton

                        Layout.fillWidth: true

                        contentItem: Text {
                            text: "Zoom In"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: zoomInButton.down ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.zoomIn()
                        }

                        onDownChanged: {
                            if (!zoomInButton.pressed)
                                appControl.serialControl.zoomStop()
                        }
                    }

                    QQC2.Button {
                        id: zoomOutButton

                        Layout.fillWidth: true

                        contentItem: Text {
                            text: "Zoom Out"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: zoomOutButton.down ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.zoomOut()
                        }

                        onDownChanged: {
                            if (!zoomOutButton.pressed)
                                appControl.serialControl.zoomStop()
                        }
                    }
                }

                RowLayout {

                    Text {
                        text: "Speed:  "
                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "white"
                        style: Text.Outline;
                        styleColor: "black"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    QQC1.Slider {
                        id: zoomSlider
                        anchors.margins: 12
                        style: sliderTouchStyle
                        value: appControl.serialControl.zoomSpeed
                        updateValueWhileDragging: false
                        minimumValue: 0
                        maximumValue: 255


                        onValueChanged: {
                            appControl.serialControl.zoomSpeed = zoomSlider.value
                        }
                    }
                }
            }
        }

        QQC2.GroupBox {

            label: QQC2.Label {
                text: " Focus: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
                font.pointSize: 12
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            ColumnLayout {
                anchors.fill: parent

                spacing: 6

                RowLayout {
                    anchors.fill: parent
                    spacing: 9

                    Layout.fillWidth: true

                    QQC2.Button {
                        id: focusFarButton

                        Layout.fillWidth: true

                        contentItem: Text {
                            text: "Focus Far"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: focusFarButton.down ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.focusFar()
                        }

                        onDownChanged: {
                            if (!focusFarButton.pressed)
                                appControl.serialControl.focusStop()
                        }
                    }

                    QQC2.Button {
                        id: focusNearButton

                        Layout.fillWidth: true

                        contentItem: Text {
                            text: "Focus Near"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: focusNearButton.down ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.focusNear()
                        }

                        onDownChanged: {
                            if (!focusNearButton.pressed)
                                appControl.serialControl.focusStop()
                        }
                    }
                }

                RowLayout {

                    Text {
                        text: "Speed:  "
                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "white"
                        style: Text.Outline;
                        styleColor: "black"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    QQC1.Slider {
                        id: focusSlider
                        anchors.margins: 12
                        style: sliderTouchStyle
                        value: appControl.serialControl.focusSpeed
                        minimumValue: 0
                        maximumValue: 255
                        updateValueWhileDragging: false
                        onValueChanged: {
                            appControl.serialControl.focusSpeed = focusSlider.value
                        }
                    }
                }
            }
        }

        QQC2.GroupBox {

            label: QQC2.Label {
                text: " Focus Mode: "
                color: "White"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
                font.pointSize: 12
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            ColumnLayout {
                anchors.fill: parent

                spacing: 9

                QQC2.RadioButton {
                    id: autoFocusButton
                    text: "Auto Focus"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.focusMode
                    font.pixelSize: 12

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 36
                        color: autoFocusButton.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        if (autoFocusButton.checked !== appControl.serialControl.focusMode) {
                            appControl.serialControl.setFocusMode(autoFocusButton.checked)
                        }
                    }
                }

                QQC2.RadioButton {
                    id: manualFocusButton
                    text: "Manual Focus"
                    font.family: "Helvetica"
                    checked: !appControl.serialControl.focusMode
                    font.pixelSize: 12

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 36
                        color: manualFocusButton.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        if (manualFocusButton.checked === appControl.serialControl.focusMode) {
                            appControl.serialControl.setFocusMode(autoFocusButton.checked)
                        }
                    }
                }
            }
        }



        QQC2.GroupBox {
            label: QQC2.Label {
                text: " Angle Ratio of the Illuminator: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
                font.pointSize: 12
            }

            background: Rectangle {
                implicitWidth: 40
                implicitHeight: 40
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 6

                Text {
                    id: ratioValueText
                    text: "Ratio:  " + appControl.serialControl.illuminatorAngleOffset / 100.0
                    font.family: "Helvetica"
                    font.pointSize: 12
                    color: "white"
                    style: Text.Outline;
                    styleColor: "black"
                }

                QQC1.Slider {
                    id: angleOffsetSlider
                    anchors.margins: 12
                    anchors.topMargin: 12
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

    Component {
        id: touchStyle
        ButtonStyle {
            panel: Item {
                implicitHeight: 36
                implicitWidth: 108
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
                        font.family: "Helvetica"
                        font.pixelSize: 15
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
                width: 18
                height: 18
                radius: height
                antialiasing: true
                color: Qt.lighter("#fc0303", 1.2)
            }

            groove: Item {
                implicitHeight: 18
                implicitWidth: 180
                Rectangle {
                    height: 8
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#ede4e4"
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
}
