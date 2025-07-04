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
import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Controls.Styles 1.1

import AppControl 1.0
import SerialControl 1.0

Item {
    //    width: parent.width
    //    height: parent.height

    Column {
        spacing: 9

        Text {
            text: "  Camera & Filter: "
            font.family: "Helvetica"
            font.pointSize: 12
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.GroupBox {

            label: QQC2.Label {
                text: " Camera Mode: "
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

            Column {
                spacing: 9

                QQC2.RadioButton {
                    id: continuousZoom
                    text: "Continuous Zoom"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                    font.pointSize: 12

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 36
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

                QQC2.RadioButton {
                    id: spotter
                    text: "Spotter"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                    font.pointSize: 12

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 36
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

                QQC2.RadioButton {
                    id: swirSpotter
                    text: "SWIR"
                    font.family: "Helvetica"
                    checked: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_SWIRSpotter
                    font.pointSize: 12
                    visible: true

                    background: Rectangle {
                        implicitWidth: 60
                        implicitHeight: 36
                        color: swirSpotter.checked ? "red" : "gray"
                        opacity: 0.5
                        radius: 5
                        border.color: "white"
                    }

                    onClicked: {
                        appControl.serialControl.setSelectedCamera(
                                    SerialControl.CameraSelection_SWIRSpotter)
                    }
                }
            }
        }

        QQC2.GroupBox {

            label: QQC2.Label {
                text: " Filter Mode: "
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

            Column {
                spacing: 9

                Column {
                    spacing: 9

                    QQC2.RadioButton {
                        id: colorFilter
                        text: "Color"
                        font.family: "Helvetica"
                        checked: appControl.serialControl.selectedFilter === SerialControl.Color
                        font.pointSize: 12
                        visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                                 || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                                 || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 36
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

                    QQC2.RadioButton {
                        id: nirFilter
                        text: "NIR"
                        font.family: "Helvetica"
                        checked: appControl.serialControl.selectedFilter === SerialControl.NIR
                        font.pointSize: 12
                        visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_ContinuousZoom
                                 || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                                 || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 36
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

                    QQC2.RadioButton {
                        id: laserFilter
                        text: "Laser"
                        font.family: "Helvetica"
                        checked: appControl.serialControl.selectedFilter === SerialControl.F1064
                        font.pointSize: 12
                        visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Spotter
                                 || appControl.serialControl.selectedCamera === SerialControl.CameraSelection_Unknown

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 36
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

                Column {
                    spacing: 9

                    QQC2.RadioButton {
                        id: bwFilter
                        text: "BW"
                        font.family: "Helvetica"
                        checked: appControl.serialControl.selectedFilter === SerialControl.Color
                        font.pointSize: 12
                        visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_SWIRSpotter

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 36
                            color: bwFilter.checked ? "red" : "gray"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onClicked: {
                            appControl.serialControl.setSelectedFilter(
                                        SerialControl.Color)
                        }
                    }

                    QQC2.RadioButton {
                        id: gwFilter
                        text: "GW"
                        font.family: "Helvetica"
                        checked: appControl.serialControl.selectedFilter === SerialControl.NIR
                        font.pointSize: 12
                        visible: appControl.serialControl.selectedCamera === SerialControl.CameraSelection_SWIRSpotter

                        background: Rectangle {
                            implicitWidth: 60
                            implicitHeight: 36
                            color: gwFilter.checked ? "red" : "gray"
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
        }
    }
}
