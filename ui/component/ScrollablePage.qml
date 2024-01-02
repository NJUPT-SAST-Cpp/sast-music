import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import FluentUI

Item {
    id: control
    default property alias content: container.data
    property alias statusMode: status_view.statusMode
    property alias timer: timer_progress
    property color color: "#fff"
    property int spacing: 0
    property int topMargin: 100
    property int leftRightMargin: 65
    property int topLoadingMargin: 60
    property color activeColor: "#335eea"
    property real progress
    signal errorClicked

    function startLoading() {
        control.progress = 0
        timer.restart()
    }
    function endLoading() {
        timer.start()
    }
    opacity: visible
    visible: false
    Timer {
        id: timer_progress
        interval: 100
        onTriggered: {
            control.progress = (control.progress + 0.1).toFixed(1)
            if (control.progress === 0.8)
                timer_progress.stop()
            else
                timer_progress.start()
        }
    }
    FluStatusView {
        id: status_view
        Layout.fillWidth: true
        objectName: "explore"
        anchors{
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            bottomMargin: 10
        }
        color: "#00000000"
        statusMode: FluStatusViewType.Success
        onErrorClicked: control.errorClicked()
        errorText: "oops, something wrong!"
        errorButtonText: "Reload"
        loadingItem: com_loading
        Component {
            id: com_loading
            FluArea {
                Component.onCompleted: {
                    startLoading()
                }
                Component.onDestruction: {
                    endLoading()
                }
                paddings: 0
                border.width: 0
                radius: 0
                color:control.color
                Layout.fillWidth: true
                ColumnLayout{
                    anchors {
                        top: parent.top
                        topMargin: control.topLoadingMargin
                    }
                    FluClip{
                        id: clip
                        width: status_view.width
                        height: 4
                        Rectangle{
                            id:rect_back
                            width: clip.width * control.progress
                            height: control.progress === 1 ? 0: clip.height
                            radius: 1
                            color: control.activeColor
                            anchors.bottom: clip.bottom
                            Behavior on height{
                                enabled: control.progress === 1
                                SequentialAnimation {
                                    PauseAnimation {
                                        duration: 400
                                    }
                                    NumberAnimation{
                                        duration: 100
                                        from: 4
                                        to: 0
                                    }
                                }
                            }
                            Behavior on width{
                                NumberAnimation{
                                    duration: 167
                                }
                            }
                        }
                    }
                }
            }
        }

        Flickable {
            id: flickview
            clip: true
            anchors.fill: parent
            contentWidth: parent.width
            contentHeight: container.height + control.topMargin
            ScrollBar.vertical: FluScrollBar {
                anchors {
                    right: flickview.right
                    rightMargin: 2
                    top: parent.top
                    topMargin: 40
                }
            }
            ColumnLayout {
                id: container
                spacing: control.spacing
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    topMargin: control.topMargin
                    leftMargin: control.leftRightMargin
                    rightMargin: control.leftRightMargin
                }
                width: parent.width
            }
        }
    }
}
