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
import QtQuick.Controls 2.2
//import QtQuick.Controls.Styles 1.1

Item {
    width: parent.width
    height: parent.height

//    property real progress: 0
//    SequentialAnimation on progress {
//        loops: Animation.Infinite
//        running: true
//        NumberAnimation {
//            from: 0
//            to: 1
//            duration: 3000
//        }
//        NumberAnimation {
//            from: 1
//            to: 0
//            duration: 3000
//        }
//    }

    GroupBox {
        title: qsTr("Camera Selection")
        spacing: 50
////        leftPadding: 50
////        bottomPadding: 50
////        padding: 50
//        implicitWidth: 500
//        implicitHeight: 400

//        Column {
//            spacing: 50
//            anchors.fill: parent
//            RadioButton {
//                id: continuousCamera
//                topPadding: 5
//                bottomPadding: 5
//                checked: true
//                contentItem: Text {
//                    text: "Continuous Zoom"
//                    color: "white"
//                    opacity: 1.0
//                    horizontalAlignment: Text.AlignRight
//                    verticalAlignment: Text.AlignVCenter
//                    font.pixelSize: 25
//                    font.bold: true
//                    //elide: Text.ElideRight
//                }

//                background: Rectangle {
//                    implicitWidth: 100
//                    implicitHeight: 60
//                    color: continuousCamera.checked ? "red" : "black"
//                    opacity: 0.5
//                    radius: 5
//                }

//                onClicked: {
//                    appControl.networkControl.setTvMode(
//                                NetworkControl.TvCommand_AutoMode)
//                }
//            }
//            RadioButton {
//                text: "Radio Button"
//                style: RadioButtonStyle {
//                    indicator: Rectangle {
//                            implicitWidth: 16
//                            implicitHeight: 16
//                            radius: 9
//                            border.color: control.activeFocus ? "darkblue" : "gray"
//                            border.width: 1
//                            Rectangle {
//                                anchors.fill: parent
//                                visible: control.checked
//                                color: "#555"
//                                radius: 9
//                                anchors.margins: 4
//                            }
//                    }
//                }
//             }

//            RadioButton {
//                id: control
//                width: 15
//                height: 15
//                text: "xlasdasdsx"
//                contentItem: Text {
//                    text: control.text
//                    color: "white"
//                    //leftPadding: control.indicator.width + control.spacing
//                    verticalAlignment: Text.AlignVCenter
//                }
//            }
            CheckBox {
                id: control
                text: qsTr("CheckBox")
                checked: true

                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: control.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 3
                    border.color: control.down ? "#17a81a" : "#21be2b"

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 2
                        color: control.down ? "#17a81a" : "#21be2b"
                        visible: control.checked
                    }
                }

                contentItem: Text {
                    text: control.text
                    font: control.font
                    opacity: enabled ? 1.0 : 0.3
                    color: control.down ? "#17a81a" : "#21be2b"
                    verticalAlignment: Text.AlignVCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
            RadioButton {
                text: qsTr("Day")
                topPadding: 5
                bottomPadding: 5
                onClicked: {
                    appControl.networkControl.setTvMode(
                                NetworkControl.TvCommand_DayMode)
                }
            }
            RadioButton {
                text: qsTr("Night")
                topPadding: 5
                bottomPadding: 5
                onClicked: {
                    appControl.networkControl.setTvMode(
                                NetworkControl.TvCommand_NightMode)
                }
            }
        }
//    }
}
