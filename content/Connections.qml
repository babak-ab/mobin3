

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
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.1

import QtQuick.Controls 2.12 as QQC2

Item {
//    width: parent.width
//    height: parent.height

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
            text: "  Connections: "
            font.family: "Helvetica"
            font.pointSize: 25
            color: "white"
            style: Text.Outline
            styleColor: "#fc0303"
        }

        Row {

            spacing: 15

            Text {
                text: "Serial Port: "
                font.family: "Helvetica"
                font.pointSize: 15
                color: "white"
                style: Text.Outline
                styleColor: "black"
            }

            QQC2.ComboBox {
                id: control
                model: appControl.serialPortList

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

                indicator: Canvas {
                    id: canvas
                    x: control.width - width - control.rightPadding
                    y: control.topPadding + (control.availableHeight - height) / 2
                    width: 12
                    height: 8
                    contextType: "2d"

                    Connections {
                        target: control
                        function onPressedChanged() {
                            canvas.requestPaint()
                        }
                    }

                    onPaint: {
                        context.reset()
                        context.moveTo(0, 0)
                        context.lineTo(width, 0)
                        context.lineTo(width / 2, height)
                        context.closePath()
                        context.fillStyle = control.pressed ? "#17a81a" : "black"
                        context.fill()
                    }
                }

                contentItem: Text {
                    anchors.leftMargin: control.indicator.width + control.spacing

                    text: control.displayText
                    font: control.font
                    color: control.pressed ? "#17a81a" : "black"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    border.color: control.pressed ? "#17a81a" : "black"
                    border.width: control.visualFocus ? 2 : 1
                    radius: 2
                }

                popup: QQC2.Popup {
                    y: control.height - 1
                    width: control.width
                    implicitHeight: contentItem.implicitHeight
                    padding: 1

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: control.popup.visible ? control.delegateModel : null
                        currentIndex: control.highlightedIndex
                    }

                    background: Rectangle {
                        border.color: "black"
                        radius: 2
                    }
                }

                currentIndex: appControl.findSerialPortName(
                                  appControl.serialControl.serialportName())
            }
        }

        QQC1.Button {
            id: connectionButton
            style: appControl.serialControl.isConnected ? pressedTouchStyle : touchStyle
            anchors.horizontalCenter: parent.horizontalCenter
            checkable: true
            checked: appControl.serialControl.isConnected

            Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                opacity: 0.5
                border.width: 1
                radius: 2
                color: "black"
                border.color: "white"
            }

            Text {
                id: connectText
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#FFFFFF"
                text: appControl.serialControl.isConnected ? "Disconnect" : "Connect"
            }

            onClicked: {

                appControl.setSerialPortName(control.currentText)

                if (connectText.text == "Connect") {

                    if (appControl.connectToSerialPort()) {
                        connectText.text = "Disconnect"
                        connectionButton.checked = true
                        connectionButton.style = pressedTouchStyle
                    } else {
                        connectText.text = "Connect"
                        connectionButton.checked = false
                        connectionButton.style = touchStyle
                    }
                } else {
                    appControl.disconnectSerialPort()
                    connectText.text = "Connect"
                    connectionButton.style = touchStyle
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
        id: pressedTouchStyle
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
                    source: "../Images/button_pressed.png"
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
}
