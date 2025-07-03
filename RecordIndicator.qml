import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.14

RowLayout {
    id: root
    visible: appControl.recordVisible

    RoundButton {
        implicitHeight: 20
        implicitWidth: 20
        radius: 20
        Rectangle {
            anchors.fill: parent
            color: "red"
            radius: 15
        }
    }

    Label {
        Text {
            text: "REC"
            font.family: "Helvetica"
            font.pointSize: 15
            font.bold: true
            color: "white"
            style: Text.Outline;
            styleColor: "black"
        }
    }

    Timer {
        interval: 1000
        running: appControl.recordVisible
        repeat: true
        onRunningChanged: {
            if (running == false) {
                root.visible = false
            }
        }
        onTriggered: {
            if (root.visible) {
                root.visible = false
            } else {
                root.visible = true
            }
        }
    }
}
