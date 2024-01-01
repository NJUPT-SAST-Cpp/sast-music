import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"

Flickable {
    objectName: "library"
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
        Row {
            spacing: 10
            Clip {
                id: avatar
                height: 46
                width: 46
                radius: 23
                Image {
                    anchors.fill: parent
                    source: "qrc:///res/img/avatar.svg"
                    fillMode: Image.PreserveAspectFit
                }
            }
            FluText {
                text: "Username's Library"
                font.family: "Barlow-Bold"
                font.weight: 700
                font.pixelSize: 42
            }
        }
    }
}
