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
            text: "  Menu: "
            font.family: "Helvetica"
            font.pointSize: 25
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.Button {
            id: upButton
            icon.source: "qrc:/Images/up-tilt-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60
            anchors.horizontalCenter: parent.horizontalCenter

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 60
                color: upButton.down ? "red" : "black"
                opacity: 0.5
                radius: 5
            }

            onPressed: {
                appControl.serialControl.menuUpPressed()
            }

            onDownChanged: {
                if (!upButton.pressed)
                    appControl.serialControl.menuUpReleased()
            }
        }

        Row {
            spacing: 80
            anchors.horizontalCenter: parent.horizontalCenter

            QQC2.Button {
                id: leftButton
                icon.source: "qrc:/Images/left-pan-icon.png"
                icon.color: "white"
                icon.height: 60
                icon.width: 60

                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 60
                    color: leftButton.down ? "red" : "black"
                    opacity: 0.5
                    radius: 5
                }

                onPressed: {
                    appControl.serialControl.menuLeftPressed()
                }

                onDownChanged: {
                    if (!leftButton.pressed)
                        appControl.serialControl.menuLeftReleased()
                }
            }

            Column {
                id: column
                //anchors.verticalCenter: column.verticalCenter
                QQC2.Button {
                    id: menuButton
                    text: "Menu"
                    font.bold: true
                    font.pixelSize: 24

                    background: Rectangle {
                        implicitWidth: 80
                        implicitHeight: 80
                        color: menuButton.down ? "red" : "black"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onPressed: {
                        appControl.serialControl.showMenuPressedRequested()

                    }

                    onDownChanged: {
                        if (!menuButton.pressed)
                            appControl.serialControl.showMenuReleased()
                    }
                }

                QQC2.Button {
                    anchors.horizontalCenter: column.horizontalCenter
                    id: escButton
                    text: "ESC"
                    font.bold: true
                    font.pixelSize: 24

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 60
                        color: escButton.down ? "red" : "black"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onDownChanged: {
                        if (!escButton.pressed)
                            appControl.serialControl.menuESCReleased()
                    }
                }
            }

            QQC2.Button {
                id: rightButton
                icon.source: "qrc:/Images/right-pan-icon.png"
                icon.color: "white"
                icon.height: 60
                icon.width: 60

                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 60
                    color: rightButton.down ? "red" : "black"
                    opacity: 0.5
                    radius: 5
                }

                onPressed: {
                    appControl.serialControl.menuRightPressed()
                }

                onDownChanged: {
                    if (!rightButton.pressed)
                        appControl.serialControl.menuRightReleased()
                }
            }
        }

        QQC2.Button {
            id: downButton
            icon.source: "qrc:/Images/down-tilt-icon.png"
            icon.color: "white"
            icon.height: 60
            icon.width: 60
            anchors.horizontalCenter: parent.horizontalCenter

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 60
                color: downButton.down ? "red" : "black"
                opacity: 0.5
                radius: 5
            }

            onPressed: {
                appControl.serialControl.menuDownPressed()
            }

            onDownChanged: {
                if (!downButton.pressed)
                    appControl.serialControl.menuDownReleased()
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
}
