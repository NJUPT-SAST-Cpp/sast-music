import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
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
    property alias normalColor: btn.normalColor
    property alias textColor: btn.textColor
    property color iconColor
    property int iconSize
    property int iconSource
    property alias hovered: btn.hovered
    property int duration: 2
    signal clicked
    signal doubleClicked
    property alias background: btn.background
    property int display: Button.IconOnly
    property alias text: btn.text

    FluIconButton {
        id: btn
        width: parent.width
        height: parent.height
        radius: control.radius
        anchors.centerIn: parent
        iconDelegate: iconUrl ? com_url : com_icon
        pressedColor: hoverColor
        display: control.display
        Component {
            id: com_url
            Item {
                width: iconWidth
                height: iconHeight
                Layout.alignment: Qt.AlignCenter
                Image {
                    id: img
                    anchors.fill: parent
                    sourceSize.width: iconWidth
                    sourceSize.height: iconHeight
                    fillMode: Image.PreserveAspectFit
                    source: iconUrl
                }
                ColorOverlay {
                    anchors.fill: parent
                    source: img
                    color: control.iconColor
                }
            }
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
        onDoubleClicked: {
            control.doubleClicked()
        }
        Component {
            id: com_icon
            FluIcon {
                id: text_icon
                font.pixelSize: iconSize
                iconSize: control.iconSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                iconColor: control.iconColor
                iconSource: control.iconSource
            }
        }
        Component {
            id: com_row
            RowLayout {
                FluLoader {
                    sourceComponent: btn.iconDelegate
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    visible: display !== Button.TextOnly
                }
                Text {
                    text: control.text
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    visible: display !== Button.IconOnly
                    color: control.textColor
                    
                    font.bold: true
                    font.pixelSize: 18
                }
            }
        }
        Component {
            id: com_column
            ColumnLayout {
                FluLoader {
                    sourceComponent: btn.iconDelegate
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    visible: display !== Button.TextOnly
                }
                FluText {
                    text: control.text
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    visible: display !== Button.IconOnly
                    color: control.textColor
                    font: control.font
                }
            }
        }
        contentItem: FluLoader {
            sourceComponent: {
                if (display === Button.TextUnderIcon) {
                    return com_column
                }
                return com_row
            }
        }
    }
}
