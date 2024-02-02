import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import sast_music
import FluentUI
import "../component"
import "../item"
import "../"

ScrollablePage {
    objectName: "likedSongs"
    id: library_page
    Row {
        spacing: 10
        FluClip {
            id: avatar
            height: 46
            width: 46
            radius: [23, 23, 23, 23]
            Image {
                anchors.fill: parent
                source: UserProfileViewModel.avatarUrl
                fillMode: Image.PreserveAspectFit
                cache: true
            }
        }
        Text {
            anchors.verticalCenter: avatar.verticalCenter
            text: UserProfileViewModel.nickname + "'s Liked Songs"
            font.family: "MiSans"
            font.weight: 700
            font.pixelSize: 42
        }
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
        model: SongViewModel

        delegate: MusicBlock {
            width: playList.width
            songTitle: model.name
            songSubtitle: model.alias
            imgSource: model.imgUrl
            album: model.album
            singer: model.artists
            time: model.duration
            playing: PlayingSongViewModel.songId === model.songId
            onPlayClicked: {
                if (!playing)
                    LikedSongViewModel.playSongByIndex(index)
            }
            onLikedChanged: liked => {//TODO
                            }
        }
    }
}
