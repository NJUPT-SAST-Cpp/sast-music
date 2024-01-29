import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"
import "../item"

ScrollablePage {
    objectName: "playList"

    Text {
        id: text_nowPlay
        text: "Now Playing"
        font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
        font.pixelSize: 30
        font.bold: true
    }

    MusicBlock {
        Layout.topMargin: 10
        Layout.fillWidth: true
        playing: true
        onLikedClicked: liked => {//TODO
                        }
    }

    Text {
        id: text_nextUP
        text: "Next UP"
        font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
        font.pixelSize: 30
        font.bold: true
        Layout.topMargin: 10
        Layout.alignment: Qt.AlignTop
    }

    ListView {
        id: playList
        implicitHeight: contentHeight
        height: implicitHeight
        interactive: false
        spacing: 10
        Layout.topMargin: 10
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        model: 20

        delegate: MusicBlock {
            width: playList.width
            onPlayClicked: playing => {//TODO
                           }
            onLikedChanged: liked => {//TODO
                            }
        }
    }
}
