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


Item {
    width: parent.width
    height: parent.height

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
//        anchors.centerIn: parent
//        anchors.topMargin: 300
//        anchors.leftMargin: 300



        Row {
            spacing: 15
            Text {
                Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    opacity: 0.2
                    border.width: 1
                    radius: 2
                    color: "black"
                }

                text: qsTr("Serial Port:")
                //anchors.centerIn: parent
                opacity: 1
                color: "#FFFFFF"
                font.pixelSize: 23
                renderType: Text.NativeRendering
            }

//            ComboBox {
//                id: serialPortNames
//                //anchors.fill: parent
//                //model: appControl.serialPort
//                //currentIndex: appControl.connectToSerialPort()
//                style: comboTouchStyle
//                editable: true
//                model: ListModel {
//                    id: model
//                    ListElement { text: "Banana" }
//                    ListElement { text: "Apple" }
//                    ListElement { text: "Coconut" }
//                }
//                onAccepted: {
//                    if (find(editText) === -1)
//                        model.append({text: editText})
//                }
//            }


            ComboBox {
                activeFocusOnPress: true
                style: comboTouchStyle

                model: /*ListModel {
                    id: cbItems
                    ListElement { text: "Banana" }
                    ListElement { text: "Apple" }
                    ListElement { text: "Coconut" }
                }*/
                       appControl.serialPort

                currentIndex: 0
                width: 200
            }
        }

        Button {
            style: touchStyle

            Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                opacity: 0.5
                border.width: 1
                radius: 2
                color: "black"
            }

            Text {
                id: controlText
                anchors.fill: parent
                   horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 24
                color: "#FFFFFF"
                text: "Connect"
            }
        }

        Button {
            style: touchStyle
            text: "Press me too"
        }

        Button {
            anchors.margins: 20
            style: touchStyle
            text: "Don't press me"
            onClicked: {
                if (stackView) {
                           stackView.pop()
                       }
            }
        }

        Row {
            spacing: 20
            Switch {
                style: switchStyle
            }
            Switch {
                style: switchStyle
            }
        }

    }


//    CustomMessageBox {
//        id: messageDialog
//        visible: false

//        _title: appControl.messageTitle
//        _text: appControl.messageText
//    }

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



//                // drop-down customization here
//                property Component __dropDownStyle: MenuStyle {
//                    __maxPopupHeight: 600

//                    __menuItemType: "comboboxitem"

//                    frame: Rectangle {              // background
//                        color: "#FFACACAC"
//                    }

//                    itemDelegate.label: Text {
//                        verticalAlignment: Text.AlignVCenter
//                        horizontalAlignment: Text.AlignHCenter
//                        font.pointSize: 12
//                        font.family: "sans serif"
//                        color: styleData.selected ?  "#FF6BC1E5" : "#FF404040"
//                        text: styleData.text
//                    }

//                    itemDelegate.background: Rectangle {  // selection of an item
//                        color: styleData.selected ?  "#FF404040" : "#FFECECEC"
//                    }

//                    __scrollerStyle: ScrollViewStyle { }
//                }
//            }
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
