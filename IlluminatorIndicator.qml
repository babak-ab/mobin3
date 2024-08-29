import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.14

RowLayout {
    id: root
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.rightMargin: 110
    anchors.topMargin: 110
    visible: appControl.serialControl.illuminator

    RoundButton {
        implicitHeight: 60
        implicitWidth: 60
        radius: 20
        icon.source: "qrc:/Images/caution-icon.png"
        icon.color: "transparent"
        icon.height: 60
        icon.width: 60

    }

    Timer {
        interval: 1000
        running: appControl.serialControl.illuminator
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
