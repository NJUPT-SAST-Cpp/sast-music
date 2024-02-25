import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"
import "../item"

ScrollablePage {
    id: page
    objectName: "playList"

    Row {
        spacing: 56
        PlayListCover {
            id: cover
            source: SongViewModel.coverImgUrl
            width: 290
        }
        Item {
            width: page.width - 290 - 56
            anchors.top: cover.top
            anchors.bottom: cover.bottom
            Text {
                id: title
                text: SongViewModel.name
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                
                font.bold: true
                font.pixelSize: 36
                width: parent.width
            }
            Text {
                id: playlist_by_username
                anchors.top: title.bottom
                anchors.topMargin: 24
                text: "Playlist by " + SongViewModel.creatorName
                elide: Text.ElideRight
                maximumLineCount: 1
                
                font.pixelSize: 18
                width: page.width - 56 - 290
            }
            Text {
                id: playlist_update_num
                anchors.top: playlist_by_username.bottom
                anchors.topMargin: 2
                text: "Updated at " + SongViewModel.updateTime + " · "
                      + SongViewModel.count + " Songs"
                elide: Text.ElideRight
                maximumLineCount: 1
                
                font.pixelSize: 14
                font.weight: 200
                width: page.width - 56 - 290
            }

            Text {
                anchors.top: playlist_update_num.bottom
                anchors.topMargin: 24
                text: SongViewModel.description
                wrapMode: Text.WordWrap
                
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
                onClicked: {
                    SongViewModel.playAllSongs()
                }
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
        model: SongViewModel

        delegate: MusicBlock {
            songTitle: model.name
            songSubtitle: model.alias
            imgSource: model.imgUrl
            album: model.album
            singer: model.artists
            time: model.duration
            width: playList.width
            playing: PlayingSongViewModel.songId === model.songId
            onPlayClicked: {
                if (!playing)
                    SongViewModel.playSongByIndex(index)
            }
            onLikedChanged: liked => {//TODO
                 liked = !liked
                 control.likedClicked(liked)
                            }
        }
    }

    Component.onCompleted: {
        statusMode = FluStatusViewType.Loading
        startLoading()
        SongViewModel.loadSongs(SongViewModel.playlistId)
    }

    Connections {
        target: SongViewModel
        function onLoadSongsFailed(message) {
            showError(message, 4000)
            statusMode = FluStatusViewType.Error
        }
    }

    onErrorClicked: {
        statusMode = FluStatusViewType.Loading
        startLoading()
        SongViewModel.loadSongs(SongViewModel.playlistId)
    }

    Connections {
        target: SongViewModel
        function onLoadSongsSuccess() {
            endLoading()
            statusMode = FluStatusViewType.Success
        }
    }
}
