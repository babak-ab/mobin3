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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
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

    GridLayout {
        rowSpacing: 9

        rows: 5
        columns: 2

        Text {
            Layout.column: 0
            Layout.row: 0

            text: "  Settings: "
            font.family: "Helvetica"
            font.pointSize: 12
            color: "white"
            style: Text.Outline;
            styleColor: "#fc0303"
        }

        QQC2.GroupBox {
            Layout.column: 0
            Layout.row: 1

            font.pointSize: 12

            label: QQC2.Label {
                text: " Defog: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            //            QQC2.SpinBox {
            //                id: defogSpinbox
            //                from: 0
            //                to: items.length - 1
            //                value: appControl.serialControl.defogMode

            //                property var items: ["Off", "Low", "Medium", "High"]

            //                validator: RegExpValidator {
            //                    regExp: new RegExp("(Off|Low|Medium|High)", "i")
            //                }

            //                textFromValue: function(value) {
            //                    return items[value];
            //                }

            //                valueFromText: function(text) {
            //                    for (var i = 0; i < items.length; ++i) {
            //                        if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
            //                            return i
            //                    }
            //                    return sb.value
            //                }

            //                onValueChanged: {
            //                    if (defogSpinbox.value !== appControl.serialControl.defogMode) {
            //                        appControl.serialControl.setDefogMode(
            //                                    defogSpinbox.value)
            //                    }
            //                }
            //            }

            Column {
                spacing: 10
                Row {
                    spacing: 10
                    QQC2.Button {
                        contentItem: Text {
                            text: "Off"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: appControl.serialControl.defogMode === 0 ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.setDefogMode(0)
                        }
                    }
                    QQC2.Button {
                        contentItem: Text {
                            text: "Low"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: appControl.serialControl.defogMode === 1 ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.setDefogMode(1)
                        }
                    }
                }

                Row {
                    spacing: 10
                    QQC2.Button {
                        contentItem: Text {
                            text: "Medium"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: appControl.serialControl.defogMode === 2 ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.setDefogMode(2)
                        }
                    }
                    QQC2.Button {
                        contentItem: Text {
                            text: "High"
                            color: "white"
                            font.family: "Helvetica"
                            opacity: 1.0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 108
                            implicitHeight: 36
                            color: appControl.serialControl.defogMode === 3 ? "red" : "black"
                            opacity: 0.5
                            radius: 5
                            border.color: "white"
                        }

                        onPressed: {
                            appControl.serialControl.setDefogMode(3)
                        }
                    }
                }
            }
        }

        QQC2.GroupBox {
            Layout.column: 0
            Layout.row: 2

            font.pointSize: 12

            label: QQC2.Label {
                text: " Gamma: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.5
                radius: 5
                border.color: "white"
            }

            QQC2.SpinBox {
                id: gamaSpinbox
                from: 0
                to: items.length - 1
                value: appControl.serialControl.gammaLevel
                font.family: "Helvetica"

                property var items: ["Level 1", "Level 2"]

                validator: RegExpValidator {
                    regExp: new RegExp("(Level 1|Level 2)", "i")
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
                    if (gamaSpinbox.value !== appControl.serialControl.gammaLevel) {
                        appControl.serialControl.setGammaLevel(
                                    gamaSpinbox.value)
                    }
                }
            }
        }

        QQC2.GroupBox {
            Layout.column: 0
            Layout.row: 3

            font.pointSize: 12

            label: QQC2.Label {
                text: " Noise Reduction: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.4
                radius: 5
                border.color: "white"
            }

            QQC2.SpinBox {
                id: noiseReductionSpinbox
                from: 0
                to: items.length - 1
                value: appControl.serialControl.noiseReductionMode
                font.family: "Helvetica"

                property var items: ["Low", "High"]

                validator: RegExpValidator {
                    regExp: new RegExp("(Low|High)", "i")
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
                    if (noiseReductionSpinbox.value !== appControl.serialControl.noiseReductionMode) {
                        appControl.serialControl.setNoiseReductionMode(
                                    noiseReductionSpinbox.value)
                    }
                }
            }
        }

        QQC2.GroupBox {
            Layout.column: 0
            Layout.row: 4

            font.pointSize: 12

            label: QQC2.Label {
                text: " Digital Zoom: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.4
                radius: 5
                border.color: "white"
            }

            QQC1.Switch {
                id: digitalZoomSwitch
                style: switchStyle
                checked: appControl.serialControl.digitalZoom

                onCheckedChanged: {
                    if (digitalZoomSwitch.checked !== appControl.serialControl.digitalZoom) {
                        appControl.serialControl.enableDigitalZoom(
                                    digitalZoomSwitch.checked)
                    }
                }
            }
        }

        QQC2.GroupBox {
            Layout.column: 1
            Layout.row: 1
            Layout.rowSpan: 2

            font.pointSize: 12

            label: QQC2.Label {
                text: " Reticle: "
                color: "white"
                font.family: "Helvetica"
                elide: Text.ElideRight
                style: Text.Outline;
                styleColor: "black"
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: "black"
                opacity: 0.4
                radius: 5
                border.color: "white"
            }

            Column {
                spacing: 9

                QQC1.Switch {
                    id: reticleSwitch
                    style: switchStyle
                    checked: appControl.reticleVisible

                    onCheckedChanged: {
                        if (reticleSwitch.checked !== appControl.reticleVisible) {
                            appControl.setReticleVisible(
                                        reticleSwitch.checked)
                        }
                    }
                }

                Row {

                    Text {
                        text: "Color:  "
                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "red"
                        style: Text.Outline;
                        styleColor: "black"
                    }

                    QQC2.SpinBox {
                        id: reticleColorSpinbox
                        from: 0
                        to: items.length - 1
                        value: reticle.color

                        property var items: ["Blue", "Red", "Green"]

                        validator: RegExpValidator {
                            regExp: new RegExp("(Blue|Red|Green)", "i")
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
                            reticle.setReticleColor(
                                        reticleColorSpinbox.value)
                        }
                    }
                }

                Row {

                    Text {
                        text: "Shape:  "
                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "red"
                        style: Text.Outline;
                        styleColor: "black"
                    }

                    QQC2.SpinBox {
                        id: spinBoxReticleShape
                        from: 0
                        to: items.length - 1
                        value: reticle.shape

                        width: 180

                        property var items:
                            ["Cross",
                            "Hollow Cross",
                            "Point Cross",
                            "Rect Cross"]

                        validator: RegExpValidator {
                            regExp: new RegExp("(Cross|Hollow Cross|Point Cross|Rect Cross)", "i")
                        }

                        textFromValue: function(value)
                        {
                            return items[value];
                        }

                        valueFromText: function(text)
                        {
                            for (var i = 0; i < items.length; ++i)
                            {
                                if (items[i].toLowerCase().
                                        indexOf(text.toLowerCase()) === 0)
                                    return i
                            }
                            return sb.value
                        }

                        onValueChanged: {
                            reticle.setReticleShape(
                                        spinBoxReticleShape.value)
                        }
                    }
                }

                GridLayout
                {
                    rows: 2
                    columns: 3

                    Text
                    {
                        Layout.column: 0
                        Layout.row: 0
                        Layout.alignment: Qt.AlignCenter

                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "red"
                        style: Text.Outline;
                        styleColor: "black"

                        text: "Reticle X:"
                    }

                    Text
                    {
                        Layout.column: 0
                        Layout.row: 1
                        Layout.alignment: Qt.AlignCenter

                        font.family: "Helvetica"
                        font.pointSize: 12
                        color: "red"
                        style: Text.Outline;
                        styleColor: "black"

                        text: "Reticle Y:"
                    }

                    QQC2.SpinBox
                    {
                        id: sliderReticlePositionX
                        Layout.column: 1
                        Layout.row: 0
                        Layout.alignment: Qt.AlignCenter

                        from: -960
                        to: 960
                        value: reticle.reticlePositionX

                        onValueChanged:
                        {
                            reticle.setReticlePositionX(
                                        sliderReticlePositionX.value)
                        }
                    }

                    QQC2.SpinBox
                    {
                        id: sliderReticlePositionY
                        Layout.column: 1
                        Layout.row: 1
                        Layout.alignment: Qt.AlignCenter

                        from: -540
                        to: 540
                        value: reticle.reticlePositionY

                        onValueChanged:
                        {
                            reticle.setReticlePositionY(
                                        sliderReticlePositionY.value)
                        }
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

                font.family: "Helvetica"
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
                        font.pointSize: 23
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
                implicitHeight: 30
                implicitWidth: 91
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width/2 - 2
                    height: 12
                    anchors.margins: 2
                    color: control.checked ? "#468bb7" : "#222"
                    Behavior on color {ColorAnimation {}}
                    Text {
                        font.pointSize: 12
                        color: "white"
                        anchors.centerIn: parent
                        text: "ON"
                        font.family: "Helvetica"
                    }
                }
                Item {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    Text {
                        font.pointSize: 12
                        color: "white"
                        anchors.centerIn: parent
                        text: "OFF"
                        font.family: "Helvetica"
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
