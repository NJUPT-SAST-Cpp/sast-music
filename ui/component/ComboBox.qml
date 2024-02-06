import QtQuick 2.15
import QtQuick.Controls
import FluentUI
import QtQuick.Templates as T

FluComboBox {
    id: control
    property color normalBackgroundColor: "#f5f5f7"
    property color activeBackgroundColor: "#eaeffd"
    property color activeColor: "#335eea"
    editable: false
    height: 32
    width: Math.max(implicitWidth, 192)
    
    font.weight: 600
    font.pixelSize: 10
    focusPolicy: Qt.StrongFocus
    background: Rectangle {
        implicitHeight: height
        implicitWidth: width
        width: control.width
        height: control.height
        visible: !control.flat || control.down
        radius: 12
        FluFocusRectangle {
            visible: control.visualFocus
            radius: 4
            anchors.margins: -2
        }
        color: control.activeFocus ? activeBackgroundColor : normalBackgroundColor
    }
    indicator: null
    contentItem: T.TextField {
        property bool disabled: !control.editable
        leftPadding: !control.mirrored ? 10 : control.editable
                                         && activeFocus ? 3 : 1
        rightPadding: control.mirrored ? 10 : control.editable
                                         && activeFocus ? 3 : 1
        topPadding: 6 - control.padding
        bottomPadding: 6 - control.padding
        renderType: FluTheme.nativeText ? Text.NativeRendering : Text.QtRendering
        selectionColor: FluTools.colorAlpha(FluTheme.primaryColor, 0.5)
        selectedTextColor: color
        text: control.editable ? control.editText : control.displayText
        enabled: control.editable
        autoScroll: control.editable
        font: control.font
        readOnly: control.down
        color: control.activeFocus ? activeColor : "#000"
        inputMethodHints: control.inputMethodHints
        validator: control.validator
        selectByMouse: true
        verticalAlignment: Text.AlignVCenter
        leftInset: 1
        topInset: 1
        bottomInset: 1
        rightInset: 1
        background: null
        Keys.onEnterPressed: event => handleCommit(event)
        Keys.onReturnPressed: event => handleCommit(event)
        function handleCommit(event) {
            control.commit(control.editText)
            accepted()
        }
    }
    delegate: T.ItemDelegate {
        id: item_delegate
        implicitWidth: Math.max(
                           implicitBackgroundWidth + leftInset + rightInset,
                           implicitContentWidth + leftPadding + rightPadding)
        implicitHeight: Math.max(
                            implicitBackgroundHeight + topInset + bottomInset,
                            implicitContentHeight + topPadding + bottomPadding,
                            implicitIndicatorHeight + topPadding + bottomPadding)
        padding: 0
        verticalPadding: 8
        horizontalPadding: 10
        width: ListView.view.width
        palette.text: control.palette.text
        palette.highlightedText: control.palette.highlightedText
        highlighted: control.highlightedIndex === index
        hoverEnabled: item_delegate.hoverEnabled
        contentItem: FluText {
            text: control.textRole ? (Array.isArray(
                                          control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
            font: control.font
            color: item_delegate.hovered ? "#fff" : activeColor
        }
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 30
            radius: 10
            color: item_delegate.hovered ? activeColor : activeBackgroundColor
            visible: control.down || control.highlighted || control.visualFocus
        }
        Component.onCompleted: {
            control.implicitWidth = Qt.binding(() => {
                                                   return width
                                               })
        }
    }
    popup: T.Popup {
        y: control.height
        width: control.width
        height: Math.min(contentItem.implicitHeight,
                         control.Window.height - topMargin - bottomMargin)
        topMargin: 6
        bottomMargin: 6
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0
            boundsMovement: Flickable.StopAtBounds
            T.ScrollIndicator.vertical: ScrollIndicator {}
        }
        enter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: FluTheme.enableAnimation ? 83 : 0
            }
        }
        exit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: FluTheme.enableAnimation ? 83 : 0
            }
        }
        background: Rectangle {
            color: activeBackgroundColor
            radius: 10
            FluShadow {
                radius: 10
            }
        }
    }
}
