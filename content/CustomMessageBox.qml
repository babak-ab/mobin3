import QtQuick 2.0
import QtQuick.Dialogs 1.1


MessageDialog {
    property point _title
    property string _text

    id: messageDialog
    visible: false
    title: _title
    text: _text
    onAccepted: {
        console.log("And of course you could only agree.")
        //Qt.quit()
    }

    Component.onCompleted: visible = true

}
