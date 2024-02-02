import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import sast_music
import FluentUI
import "../component"
import "../item"

ScrollablePage {
    objectName: "nextUp"

    Text {
        id: text_nowPlay
        text: "Now Playing"
        font.family: "MiSans"
        font.pixelSize: 30
        font.bold: true
    }

    MusicBlock {
        songTitle: PlayingSongViewModel.name
        songSubtitle: PlayingSongViewModel.alias
        imgSource: PlayingSongViewModel.imgUrl
        album: PlayingSongViewModel.album
        singer: PlayingSongViewModel.artists
        time: PlayingSongViewModel.durationTime
        Layout.topMargin: 10
        Layout.fillWidth: true
        playing: true
        onLikedClicked: liked => {//TODO
                        }
    }

    Text {
        id: text_nextUP
        text: "Next UP"
        font.family: "MiSans"
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
        model: NextUpViewModel

        delegate: MusicBlock {
            songTitle: model.name
            songSubtitle: model.alias
            imgSource: model.imgUrl
            album: model.album
            singer: model.artists
            time: model.duration
            width: playList.width
            onPlayClicked: {
                NextUpViewModel.playSongByIndex(index)
            }
            onLikedChanged: liked => {//TODO
                            }
        }
    }
}
