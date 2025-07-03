import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.14

Rectangle
{
    visible: appControl.serialControl.illuminator
    color: "#80000000"

    height: 40
    width: 40
    radius: width / 2
    layer.enabled: false

    Image
    {
        anchors.fill: parent
        anchors.margins: 7
        anchors.topMargin: 3

        source: "qrc:/Images/caution-icon.png"
    }

    Timer {
        interval: 1000
        running: appControl.serialControl.illuminator
        repeat: true
        onRunningChanged: {
            if (running == false) {
                parent.visible = false
            }
        }
        onTriggered: {
            if (parent.visible) {
                parent.visible = false
            } else {
                parent.visible = true
            }
        }
    }
}

//    RoundButton {
//        visible: appControl.serialControl.illuminator
//        implicitHeight: 60
//        implicitWidth: 60
//        radius: 20
//        icon.source: "qrc:/Images/caution-icon.png"
//        icon.color: "transparent"
//        icon.height: 60
//        icon.width: 60

//        Timer {
//            interval: 1000
//            running: appControl.serialControl.illuminator
//            repeat: true
//            onRunningChanged: {
//                if (running == false) {
//                    parent.visible = false
//                }
//            }
//            onTriggered: {
//                if (parent.visible) {
//                    parent.visible = false
//                } else {
//                    parent.visible = true
//                }
//            }
//        }
//    }
