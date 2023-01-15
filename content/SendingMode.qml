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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.1

import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls 2.2 as QQC2

import AppControl 1.0
import SerialControl 1.0

ScrollView {
    width: parent.width
    height: parent.height

    flickableItem.interactive: true

    Column {
        spacing: 40


        Text {
            text: "  Sending Mode: "
            font.family: "Helvetica"
            font.pointSize: 25
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.GroupBox {
            font.pixelSize: 25

            label: QQC2.Label {
                text: " Sending Mode: "
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

            Column {
                spacing: 25

                QQC2.RadioButton {
                    id: requestSendingMode
                    text: "Request"
                    checked: true
                    font.pixelSize: 25

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 60
                        color: requestSendingMode.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        continuousSendingModeSpinBox.visible = false;
                        msec.visible = false

                        appControl.serialControl.setRequestSendingMode()
                    }
                }

                QQC2.RadioButton {
                    id: statusSendingMode
                    text: "Status"
                    checked: false
                    font.pixelSize: 25

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 60
                        color: statusSendingMode.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        continuousSendingModeSpinBox.visible = false;
                        msec.visible = false

                        appControl.serialControl.setStatusSendingMode()
                    }
                }

                QQC2.RadioButton {
                    id: continuousSendingMode
                    text: "Continuous"
                    checked: false
                    font.pixelSize: 25

                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 60
                        visible: control.checked
                        color: continuousSendingMode.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        continuousSendingModeSpinBox.visible = true;
                        msec.visible = true

                        appControl.serialControl.setContinuousSendingMode(
                                    continuousSendingModeSpinBox.value)
                    }
                }

                Row {
                    spacing: 10

                    QQC2.SpinBox {
                        id: continuousSendingModeSpinBox
                        from: 1
                        to: 255
                        value: 1
                        visible: false

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 60
                            color: "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }
                    }

                    Text {

                        id: msec
                        visible: false
                        text: "msec"
                        font.family: "Helvetica"

                        font.pointSize: 15
                        color: "white"
                        style: Text.Outline;
                        styleColor: "black"
                        anchors.topMargin: 25
                    }
                }
            }
        }
    }
}