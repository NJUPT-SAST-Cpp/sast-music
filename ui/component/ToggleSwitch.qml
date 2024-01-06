import QtQuick 2.15
import QtQuick.Layouts
import FluentUI

FluToggleSwitch {
    id: control
    property color backgroundColor: "#f5f5f7"
    property color activeColor: "#335eea"
    checkColor: activeColor
    normalColor: backgroundColor
    background: Item {
        implicitHeight: 32
        implicitWidth: 52
    }
    contentItem: RowLayout {
        spacing: control.textSpacing
        layoutDirection: control.textRight ? Qt.LeftToRight : Qt.RightToLeft
        Rectangle {
            id: control_backgound
            width: background.width
            height: background.height
            radius: 10
            color: checked ? checkColor : normalColor
            Rectangle {
                width: 20
                height: 20
                radius: 5
                x: checked ? control_backgound.width - width - 6 : 6
                y: (control_backgound.height - height) / 2
                scale: {
                    if (pressed) {
                        return 9 / 10
                    }
                    return hovered ? 12 / 10 : 1
                }
                color: "#fff"
                Behavior on scale {
                    NumberAnimation {
                        duration: 167
                        easing.type: Easing.OutCubic
                    }
                }
                Behavior on x {
                    enabled: FluTheme.enableAnimation
                    NumberAnimation {
                        duration: 167
                        easing.type: Easing.OutCubic
                    }
                }
                FluShadow {
                    radius: 5
                    visible: !checked
                }
            }
        }
    }
}
