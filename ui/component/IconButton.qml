import QtQuick 2.15
import QtQuick.Layouts
import FluentUI

Item {
    id: control
    width: 30
    height: 30
    property int radius: 10
    property string iconUrl
    property int iconWidth: 16
    property int iconHeight: 16
    property color hoverColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
    property int duration: 2
    signal clicked
    FluIconButton {
        width: parent.width
        height: parent.height
        radius: control.radius
        anchors.centerIn: parent
        iconDelegate: Image {
            width: iconWidth
            height: iconHeight
            sourceSize.width: iconWidth
            sourceSize.height: iconHeight
            fillMode: Image.PreserveAspectFit
            source: iconUrl
            Layout.alignment: Qt.AlignCenter
        }
        hoverColor: control.hoverColor
        Behavior on width {
            NumberAnimation {
                duration: control.duration
            }
        }
        Behavior on height {
            NumberAnimation {
                duration: control.duration
            }
        }
        onPressed: {
            width = width * 0.95
            height = height * 0.95
        }
        onReleased: {
            width = width / 0.95
            height = height / 0.95
        }
        onClicked: {
            control.clicked()
        }
    }
}
