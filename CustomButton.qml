import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtCharts 2.3
import QtMultimedia 5.12
import QtQuick.Layouts 1.14
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Imagine 2.12
import QtQuick.Dialogs 1.1

Button {
    property string _text
    property point _point
    property int _width
    property bool _checked
    property bool _visible

    id: control
    width: fontMetrics.advanceWidth(_text) + 10
    height: 50
    x: _point.x - control.width / 2
    y: _point.y
    text: _text
    checked: _checked
    checkable: true
    font.pointSize: 16
    visible: _visible

    FontMetrics {
        id: fontMetrics
        font: control.font
    }

    contentItem: Text {
        text: control.text
        font: control.font

        opacity: enabled ? 1.0 : 0.3
        color: control.checked ? "#000000" : "#43cc0e"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        opacity: enabled ? 1 : 0.3
        color: control.checked ? "#43cc0e" : "#000000"
        radius: 2
    }
}
