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
            text: "  Advanced Settings: "
            font.family: "Helvetica"
            font.pointSize: 25
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Contrast: "
                color: "white"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 150
                implicitHeight: 60
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }


            QQC2.SpinBox {
                id: contrastSpinbox
                from: 0
                to: items.length - 1
                value: appControl.serialControl.contrastLevel

                property var items: ["Contrast 1", "Contrast 2", "Contrast 3"]

                validator: RegExpValidator {
                    regExp: new RegExp("(Contrast 1|Contrast 2|Contrast 3)", "i")
                }

                textFromValue: function(value) {
                    return items[value];
                }

                valueFromText: function(text) {
                    for (var i = 0; i < items.length; ++i) {
                        if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                            return i
                    }
                    return sb.value
                }

                onValueChanged: {
                    if (contrastSpinbox.value !== appControl.serialControl.contrastLevel) {
                        appControl.serialControl.setContrastMode(
                                    contrastSpinbox.value)
                    }
                }
            }
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Brightness: "
                color: "white"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 150
                implicitHeight: 60
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            QQC2.SpinBox {
                id: brightnessSpinbox
                from: 0
                to: items.length - 1
                value: appControl.serialControl.brightnessLevel

                property var items: ["Brightness 1", "Brightness 2", "Brightness 3", "Brightness 4"]

                validator: RegExpValidator {
                    regExp: new RegExp("(Brightness 1|Brightness 2|Brightness 3|Brightness 4)", "i")
                }

                textFromValue: function(value) {
                    return items[value];
                }

                valueFromText: function(text) {
                    for (var i = 0; i < items.length; ++i) {
                        if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                            return i
                    }
                    return sb.value
                }

                onValueChanged: {
                    if (brightnessSpinbox.value !== appControl.serialControl.brightnessLevel) {
                        appControl.serialControl.setBrightnessLevel(
                                    brightnessSpinbox.value)
                    }
                }
            }
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Mode: "
                color: "white"
                elide: Text.ElideRight
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

            QQC2.SpinBox {
                id: modeSpinbox
                from: 0
                to: items.length - 1
                value: appControl.serialControl.mode

                property var items: ["Mode 1", "Mode 2", "Mode 3", "Mode 4", "Mode 5"]

                validator: RegExpValidator {
                    regExp: new RegExp("(Mode 1|Mode 2|Mode 3|Mode 4|Mode 5)", "i")
                }

                textFromValue: function(value) {
                    return items[value];
                }

                valueFromText: function(text) {
                    for (var i = 0; i < items.length; ++i) {
                        if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                            return i
                    }
                    return sb.value
                }

                onValueChanged: {
                    if (modeSpinbox.value !== appControl.serialControl.mode) {
                        appControl.serialControl.setMode(
                                    modeSpinbox.value)
                    }
                }
            }
        }
    }

    Component {
        id: comboTouchStyle
        ComboBoxStyle {

            background: Rectangle {
                id: rectCategory
                radius: 5
                border.width: 2
                color: "#555"
            }

            label: Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 15
                font.family: "Courier"
                font.capitalization: Font.SmallCaps
                color: "white"
                text: control.currentText
            }
        }
    }

    Component {
        id: touchStyle
        ButtonStyle {
            panel: Item {
                implicitHeight: 60
                implicitWidth: 150
                BorderImage {
                    anchors.fill: parent
                    antialiasing: true
                    border.bottom: 8
                    border.top: 8
                    border.left: 8
                    border.right: 8
                    anchors.margins: control.pressed ? -4 : 0
                    source: control.pressed ? "../Images/button_pressed.png" : "../Images/button_default.png"
                    Text {
                        text: control.text
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 23
                        renderType: Text.NativeRendering
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
