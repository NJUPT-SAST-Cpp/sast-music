import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"
import "../item"

ScrollablePage {
    id: page
    objectName: "songListInfo"
    Row {
        spacing: 56
        PlayListCover {
            id: cover
            source: "qrc:///res/img/background.png"
            width: 290
        }
        Item {
            width: page.width - 290 - 56
            anchors.top: cover.top
            anchors.bottom: cover.bottom
            Text {
                id: title
                text: "Song List Title"
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
                font.bold: true
                font.pixelSize: 36
                width: parent.width
            }
            Text {
                id: playlist_by_username
                anchors.top: title.bottom
                anchors.topMargin: 24
                text: "Playlist by Username"
                elide: Text.ElideRight
                maximumLineCount: 1
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
                font.pixelSize: 18
                width: page.width - 56 - 290
            }
            Text {
                id: playlist_update_num
                anchors.top: playlist_by_username.bottom
                anchors.topMargin: 2
                text: "Updated at Dec 11, 2023 · 166 Songs"
                elide: Text.ElideRight
                maximumLineCount: 1
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
                font.pixelSize: 14
                font.weight: 200
                width: page.width - 56 - 290
            }

            Text {
                anchors.top: playlist_update_num.bottom
                anchors.topMargin: 24
                topPadding: 24
                text: "description"
                wrapMode: Text.WordWrap
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
                font.pixelSize: 14
                font.weight: 200
                width: page.width - 56 - 290 - 130
            }
            IconButton {
                anchors.bottom: parent.bottom
                width: 98
                height: 40
                display: Button.TextBesideIcon
                iconUrl: "qrc:///res/img/play.svg"
                iconColor: "#335eea"
                normalColor: "#eaeffd"
                hoverColor: normalColor
                textColor: "#335eea"
                text: "PLAY"
            }
        }
    }

    ListView {
        id: playList
        implicitHeight: contentHeight
        height: implicitHeight
        Layout.fillWidth: true
        interactive: false
        Layout.topMargin: 35
        Layout.alignment: Qt.AlignHCenter
        model: 20

        delegate: MusicBlock {
            width: page.width - 130
            onPlayClicked: playing => {//TODO
                           }
            onLikedChanged: liked => {//TODO
                            }
        }
    }
}
