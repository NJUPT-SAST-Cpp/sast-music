import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"

ScrollablePage {
    objectName: "library"

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
            anchors.verticalCenter: avatar.verticalCenter
            text: "Username's Library"
            font.family: "Barlow-Bold"
            font.weight: 700
            font.pixelSize: 42
        }
    }
}
