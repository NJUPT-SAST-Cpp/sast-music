import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import FluentUI

T.Slider {
    id: control
    property bool active: false
    property bool handleVisible: true
    property color normalColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
    property color activeColor: "#335eea"
    property int handleSize: 14
    property int radius: 2
    property bool tooltipEnabled: false
    property string tipText
    to: 100
    stepSize: 1
    height: 4
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitHandleHeight + topPadding + bottomPadding)
    padding: 0
    handle: Rectangle {
        x: control.leftPadding + (control.horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding
           + (control.horizontal ? (control.availableHeight - height) / 2 : control.visualPosition
                                   * (control.availableHeight - height))
        implicitWidth: control.handleSize
        implicitHeight: control.handleSize
        radius: width / 2
        color: "#fff"
        visible: handleVisible
        FluShadow {
            radius: parent.radius
        }
    }
    background: Rectangle {
        x: control.leftPadding + (control.horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (control.horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: control.horizontal ? 200 : control.width
        implicitHeight: control.horizontal ? control.height : 200
        width: control.availableWidth
        height: implicitHeight
        radius: control.radius
        color: normalColor
        scale: control.horizontal && control.mirrored ? -1 : 1
        Rectangle {
            y: control.horizontal ? 0 : control.visualPosition * parent.height
            width: control.horizontal ? control.position * parent.width : control.width
            height: control.horizontal ? control.height : control.position * parent.height
            radius: control.radius
            color: active ? activeColor : "#000"
        }
    }
    FluTooltip {
        parent: control.handle
        visible: control.tooltipEnabled && (control.pressed || control.hovered)
        text: tipText
        font.family: "MiSans"
        font.pixelSize: 12
        font.weight: 500
    }
}
