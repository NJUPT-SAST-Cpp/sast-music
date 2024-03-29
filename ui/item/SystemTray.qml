import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import Qt.labs.platform
import sast_music

SystemTrayIcon {
    id: control
    property var window
    visible: true
    icon.source: "qrc:///res/icon/app.ico"
    tooltip: "SAST Music"
    menu: Menu {
        MenuItem {
            text: "显示主面板"
            onTriggered: {
                window.show()
            }
        }
        MenuSeparator {}
        MenuItem {
            icon.source: "qrc:///res/img/play.svg"
            text: PlayingSongViewModel.playing ? "暂停" : "播放"
            onTriggered: {
                PlayingSongViewModel.playing ? PlayingSongViewModel.pause() : PlayingSongViewModel.play()
            }
        }
        MenuItem {
            icon.source: "qrc:///res/img/previous.svg"
            text: "上一首"
            onTriggered: {
                PlayingSongViewModel.previous()
            }
        }
        MenuItem {
            icon.source: "qrc:///res/img/next.svg"
            text: "下一首"
            onTriggered: {
                PlayingSongViewModel.next()
            }
        }
        MenuItem {
            icon.source: "qrc:///res/img/heart.svg"
            text: "喜欢"
            onTriggered: {

                // TODO
            }
        }
        MenuItem {
            icon.source: "qrc:///res/img/logout.svg"
            text: "退出"
            onTriggered: {
                Qt.exit(0)
            }
        }
    }
    onActivated: reason => {
                     if (reason === SystemTrayIcon.Trigger) {
                         window.show()
                         window.raise()
                         window.requestActivate()
                     }
                 }
}
