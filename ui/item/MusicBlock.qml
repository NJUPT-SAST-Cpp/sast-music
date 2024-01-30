import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"

Item {
    id: control
    property bool playing: false
    property bool liked: false
    property color hoverBgColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
    property color activeBgColor: "#eaeffd"
    property color activeColor: "#335eea"
    property string imgSource: "qrc:///res/img/netease-music.png"
    property string songTitle: "Song"
    property string songSubtitle: "SongSubTitle"
    property string singer: "singer"
    property string album: "album"
    property string time: "4:12"
    signal playClicked(bool playing)
    signal likedClicked(bool licked)

    height: 60
    Rectangle {
        id: background
        color: playing ? activeBgColor : mouseArea.containsMouse ? hoverBgColor : "transparent"
        anchors.fill: parent
        radius: 10
    }

    FluClip {
        id: image_song
        radius: [3, 3, 3, 3]
        width: 35
        height: 35
        anchors {
            left: parent.left
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        FluImage {
            anchors.fill: parent
            source: control.imgSource
        }
        FluShadow {}
    }

    Text {
        id: text_song
        text: songTitle
        elide: Text.ElideRight
        maximumLineCount: 1
        font.family: "MiSans"
        font.bold: true
        font.pixelSize: 16
        color: playing ? activeColor : "#000"
        anchors {
            left: image_song.right
            leftMargin: 10
            top: parent.top
            topMargin: 10
        }
    }

    Text {
        id: subheading
        text: "(" + control.songSubtitle + ")"
        elide: Text.ElideRight
        color: playing ? activeColor : "#7a7a7a"
        maximumLineCount: 1
        font.family: "MiSans"
        font.bold: true
        font.pixelSize: 16
        anchors {
            left: text_song.right
            leftMargin: 2
            top: parent.top
            topMargin: 10
        }
    }

    Text {
        id: text_singer
        text: control.singer
        elide: Text.ElideRight
        maximumLineCount: 1
        font.family: "MiSans"
        font.pixelSize: 12
        color: playing ? activeColor : "#000"
        anchors {
            left: image_song.right
            leftMargin: 10
            top: parent.top
            topMargin: 35
        }
    }

    Text {
        id: album
        text: "album"
        elide: Text.ElideRight
        maximumLineCount: 1
        font.family: "MiSans"
        font.pixelSize: 16
        color: playing ? activeColor : "#000"
        anchors {
            left: parent.left
            leftMargin: 400
            verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: text_time
        text: control.time
        color: "black"
        anchors {
            right: parent.right
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onDoubleClicked: {
            playing = !playing
            control.playClicked(playing)
        }
    }

    IconButton {
        id: btn_like
        width: 40
        height: 40
        iconWidth: 15
        iconHeight: 15
        iconUrl: control.liked ? "qrc:///res/img/heart-solid.svg" : "qrc:///res/img/heart.svg"
        iconColor: activeColor
        visible: control.liked || mouseArea.containsMouse
        anchors {
            right: text_time.left
            rightMargin: 20
            verticalCenter: parent.verticalCenter
        }
        onClicked: {
            liked = !liked
            control.likedClicked(liked)
        }
    }
}
