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
    property int duration: 20
    property bool hoverAnimation: false
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
        onHoveredChanged: {
            if (hoverAnimation) {
                implicitHeight = btn.hovered ? implicitHeight * 1.05 : implicitHeight / 1.05
                implicitWidth = btn.hovered ? implicitWidth * 1.05 : implicitWidth / 1.05
                control.fontSize = btn.hovered ? control.fontSize * 1.05 : control.fontSize / 1.05
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
        background: Rectangle {
            implicitWidth: 28
            implicitHeight: 28
            radius: 10
            color: {
                if (!enabled) {
                    return backgroundDisableColor
                }
                if (btn.hovered) {
                    return backgroundHoverColor
                }
                return backgroundNormalColor
            }
        }
    }
    Behavior on fontSize {
        NumberAnimation {
            duration: control.duration
        }
    }
}
