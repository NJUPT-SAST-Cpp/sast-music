import QtQuick
import FluentUI

Item {
    id: control
    property string text: ""
    property int fontWeight: 700
    property real fontSize: 18
    property color textColor: "#000"
    property color backgroundHoverColor: Qt.rgba(209 / 255, 209 / 255,
                                                 214 / 255, 0.28)
    property color backgroundNormalColor: "transparent"
    property int duration: 2
    signal clicked
    Component.onCompleted: {
        width = btn.implicitWidth
    }
    height: implicitHeight
    implicitHeight: 30
    FluTextButton {
        id: btn
        text: control.text
        font.family: "Barlow-Bold"
        font.pixelSize: control.fontSize
        font.weight: control.fontWeight
        textColor: control.textColor
        implicitHeight: parent.implicitHeight
        backgroundHoverColor: control.backgroundHoverColor
        backgroundNormalColor: control.backgroundNormalColor
        onClicked: {
            control.clicked()
        }
        anchors.centerIn: control
        Behavior on implicitHeight {
            NumberAnimation {
                duration: control.duration
            }
        }
        Behavior on implicitWidth {
            NumberAnimation {
                duration: control.duration
            }
        }
        onPressed: {
            implicitHeight = implicitHeight * 0.95
            implicitWidth = implicitWidth * 0.95
            control.fontSize = control.fontSize * 0.95
        }
        onReleased: {
            implicitHeight = implicitHeight / 0.95
            implicitWidth = implicitWidth / 0.95
            control.fontSize = control.fontSize / 0.95
        }
    }
    Behavior on fontSize {
        NumberAnimation {
            duration: control.duration
        }
    }
}
