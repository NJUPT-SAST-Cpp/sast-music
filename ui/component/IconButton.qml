import QtQuick 2.15
import FluentUI

FluIconButton {
    id: control
    property bool liked: false
    property string iconUrl
    property int iconWidth: 17
    property int iconHeight: 17
    radius: 10
    width: 30
    height: 30
    iconDelegate: Image {
        width: control.iconWidth
        height: control.iconHeight
        fillMode: Image.PreserveAspectFit
        source: iconUrl
        anchors.centerIn: parent
    }
    hoverColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
}
