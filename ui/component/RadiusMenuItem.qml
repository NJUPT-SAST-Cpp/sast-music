import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.impl
import QtQuick.Templates as T
import Qt5Compat.GraphicalEffects
import FluentUI

T.MenuItem {
    property Component iconDelegate: com_icon
    property int iconSpacing: 5
    property string iconUrl
    property int iconSize: 16
    property color textColor: {
        if (control.highlighted) {
            return "#335eea"
        }
        return "#000"
    }
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        implicitBackgroundHeight + topInset + bottomInset,
                        implicitContentHeight + topPadding + bottomPadding,
                        implicitIndicatorHeight + topPadding + bottomPadding)
    padding: 6
    spacing: 6
    icon.width: 24
    icon.height: 24
    icon.color: control.palette.windowText
    height: visible ? implicitHeight : 0
    Component {
        id: com_icon
        Item {
            width: control.iconSize
            height: control.iconSize
            Image {
                id: img
                sourceSize.width: control.iconSize
                sourceSize.height: control.iconSize
                fillMode: Image.PreserveAspectFit
                height: control.iconSize
                width: control.iconSize
                source: control.iconUrl
            }
            ColorOverlay {
                anchors.fill: parent
                source: img
                color: {
                    if (control.highlighted) {
                        return "#335eea"
                    }
                    return "#000"
                }
            }
        }
    }
    contentItem: Item {
        Row {
            spacing: control.iconSpacing
            readonly property real arrowPadding: control.subMenu
                                                 && control.arrow ? control.arrow.width
                                                                    + control.spacing : 0
            readonly property real indicatorPadding: control.checkable
                                                     && control.indicator ? control.indicator.width + control.spacing : 0
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: (!control.mirrored ? indicatorPadding : arrowPadding) + 5
                right: parent.right
                rightMargin: (control.mirrored ? indicatorPadding : arrowPadding) + 5
            }
            Loader {
                id: loader_icon
                sourceComponent: iconDelegate
                anchors.verticalCenter: parent.verticalCenter
                visible: status === Loader.Ready
            }
            FluText {
                id: content_text
                text: control.text
                color: control.textColor
                font.family: "MiSans"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
    indicator: FluIcon {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        visible: control.checked
        iconSource: FluentIcons.CheckMark
    }
    arrow: FluIcon {
        x: control.mirrored ? control.leftPadding : control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        visible: control.subMenu
        iconSource: FluentIcons.ChevronRightMed
    }
    background: Item {
        implicitWidth: 150
        implicitHeight: 36
        x: 1
        y: 1
        width: control.width - 2
        height: control.height - 2
        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            radius: 10
            color: {
                if (control.highlighted) {
                    return "#eaeffd"
                }
                return FluTheme.itemNormalColor
            }
        }
    }
}
