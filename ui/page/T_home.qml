import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI

Flickable {
    objectName: "home"
    ScrollBar.vertical: FluScrollBar{}
    contentHeight: layout.implicitHeight

    ColumnLayout {
        id: layout
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: 100
            leftMargin: 65
            rightMargin: 65
        }

        FluImage {
            Layout.topMargin: 100
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:///res/img/sast_music.svg"
            height: 200
            fillMode: Image.PreserveAspectFit
        }
    }
}
