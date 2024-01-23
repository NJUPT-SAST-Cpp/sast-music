import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Basic
import FluentUI

TextField {
    signal commit(string text)
    property bool disabled: false
    property color placeholderNormalColor: "#767676"
    property color placeholderFocusColor: "#335eea"
    id: control
    padding: 7
    width: 200
    height: 30
    leftPadding: padding + 28
    rightPadding: 20
    enabled: !disabled
    placeholderText: "Search"
    font.pixelSize: 16
    font.family: "Barlow-Medium"
    font.bold: true
    color: activeFocus ? "#335eea" : "#767676"
    selectionColor: FluTools.colorAlpha(FluTheme.primaryColor, 0.5)
    selectedTextColor: color
    placeholderTextColor: {
        if (activeFocus) {
            return placeholderFocusColor
        }
        return placeholderNormalColor
    }
    selectByMouse: true
    background: FluClip {
        property Item inputItem
        inputItem: control
        radius: [6, 6, 6, 6]
        Rectangle {
            radius: 6
            anchors.fill: parent
            color: control.activeFocus ? "#eaeffd" : "#f2f2f4"
        }
    }
    Keys.onEnterPressed: event => d.handleCommit(event)
    Keys.onReturnPressed: event => d.handleCommit(event)
    QtObject {
        id: d
        function handleCommit(event) {
            control.commit(control.text)
        }
    }
    MouseArea {
        id: item_mouse
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.RightButton
        hoverEnabled: true
    }
    FluIcon {
        id: icon_search
        anchors {
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
        iconSource: FluentIcons.Search
        iconColor: control.activeFocus ? "#335eea" : "#767676"
    }
    FluIconButton {
        anchors {
            right: parent.right
            rightMargin: 5
        }
        iconSource: FluentIcons.Cancel
        iconSize: 12
        Layout.preferredWidth: 30
        Layout.preferredHeight: 20
        Layout.alignment: Qt.AlignVCenter
        iconColor: control.activeFocus ? "#335eea" : "#767676"
        verticalPadding: 0
        horizontalPadding: 0
        visible: control.text !== "" && item_mouse.containsMouse
        contentDescription: "Clean"
        onClicked: {
            control.text = ""
        }
        hoverEnabled: false
    }
}
