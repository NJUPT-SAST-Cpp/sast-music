import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"

Rectangle {
    id: rec_lyrics
    property Animation animationdown
    property int windowHeight
    property int volumeValue
    Image {
        id: img_bg
        anchors.fill: parent
        source: PlayingSongViewModel.imgUrl
        cache: true
    }
    FluAcrylic {
        anchors.fill: parent
        target: img_bg
        blurRadius: 100
        tintOpacity: 0
        tintColor: Qt.rgba(0, 0, 0, 0.39)
    }
    MouseArea {
        anchors.fill: parent
    }
    Loader {
        anchors.fill: parent
        sourceComponent: SongLyricViewModel.hasLyric ? com_lyric_music : com_no_lyric_music
    }
    Component {
        id: com_lyric_music
        Item {
            width: rec_lyrics.width
            height: rec_lyrics.height
            RowLayout {
                spacing: 50
                Loader {
                    sourceComponent: com_no_lyric_music
                }
                // TODO: lyrics
            }
        }
    }
    Component {
        id: com_no_lyric_music
        Item {
            width: rec_lyrics.width
            height: rec_lyrics.height
            FluClip {
                id: song_img
                width: windowHeight * 0.5
                height: width
                radius: [10, 10, 10, 10]
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 100 * windowHeight / 720
                }
                Image {
                    id: img
                    anchors.fill: parent
                    source: PlayingSongViewModel.imgUrl
                    cache: true
                }
                FluShadow {
                    radius: 10
                }
            }
            IconButton {
                id: btn_plus
                iconUrl: "qrc:///res/img/plus.svg"
                anchors {
                    top: song_img.bottom
                    topMargin: 35
                    right: song_img.right
                }
                width: 34
                height: 34
                iconWidth: 18
                iconHeight: 18
                iconColor: "#fff"
                hoverColor: Qt.rgba(1, 1, 1, 0.08)
            }
            IconButton {
                id: btn_liked
                property bool liked: false
                width: 34
                height: 34
                iconWidth: 18
                iconHeight: 18
                iconColor: "#fff"
                hoverColor: Qt.rgba(1, 1, 1, 0.08)
                iconUrl: liked ? "qrc:///res/img/heart.svg" : "qrc:///res/img/heart-solid.svg"
                anchors {
                    top: btn_plus.top
                    right: btn_plus.left
                    rightMargin: 8
                }
                onClicked: {
                    liked = !liked
                }
            }
            MusicSlider {
                id: slider_volume
                anchors {
                    verticalCenter: btn_plus.verticalCenter
                    right: btn_liked.left
                    rightMargin: 8
                }
                value: VolumeViewModel.volume
                width: 84
                active: true
                activeColor: Qt.rgba(1, 1, 1, 0.7)
                handleVisible: item_mouse_slider_volume.containsMouse
                onValueChanged: {
                    VolumeViewModel.volume = value
                }
            }
            MouseArea {
                id: item_mouse_slider_volume
                propagateComposedEvents: true
                width: 85
                height: 15
                anchors {
                    right: slider_volume.right
                    top: song_img.bottom
                    topMargin: 46
                }
                onClicked: mouse => mouse.accepted = false
                onPressAndHold: mouse => mouse.accepted = false
                onPressed: mouse => mouse.accepted = false
                onReleased: mouse => mouse.accepted = false
                hoverEnabled: true
            }
            IconButton {
                id: btn_volume
                property bool mute: VolumeViewModel.volume === 0
                width: 34
                height: 34
                iconWidth: 18
                iconHeight: 18
                iconColor: "#fff"
                hoverColor: Qt.rgba(1, 1, 1, 0.08)
                iconUrl: mute ? "qrc:///res/img/volume-mute.svg" : (slider_volume.value > 50 ? "qrc:///res/img/volume.svg" : "qrc:///res/img/volume-half.svg")
                anchors {
                    verticalCenter: slider_volume.verticalCenter
                    right: item_mouse_slider_volume.left
                    rightMargin: 8
                }
                onClicked: {
                    if (!mute) {
                        volumeValue = VolumeViewModel.volume
                        VolumeViewModel.volume = 0
                    } else {
                        VolumeViewModel.volume = volumeValue
                    }
                }
            }
            Text {
                id: text_song
                text: PlayingSongViewModel.name
                font.family: "MiSans"
                font.pixelSize: 22
                font.weight: 500
                color: Qt.rgba(1, 1, 1, 0.95)
                elide: Text.ElideRight
                maximumLineCount: 1
                anchors {
                    left: song_img.left
                    right: btn_volume.left
                    rightMargin: 8
                    top: song_img.bottom
                    topMargin: 20
                }
            }
            Text {
                id: text_singer
                text: PlayingSongViewModel.artists
                font.family: "MiSans"
                font.pixelSize: 14
                font.weight: 500
                color: Qt.rgba(1, 1, 1, 0.7)
                elide: Text.ElideRight
                maximumLineCount: 1
                anchors {
                    left: song_img.left
                    right: btn_volume.left
                    rightMargin: 8
                    top: text_song.bottom
                    topMargin: 5
                }
            }
            Text {
                id: text_start_time
                anchors {
                    left: song_img.left
                    top: text_singer.bottom
                    topMargin: 22
                }
                text: slider_progress.milsec2Time(
                          PlayingSongViewModel.timeStamp)
                font.family: "MiSans"
                font.pixelSize: 14
                font.weight: 500
                color: Qt.rgba(1, 1, 1, 0.7)
            }
            Text {
                id: text_end_time
                anchors {
                    right: song_img.right
                    top: text_singer.bottom
                    topMargin: 22
                }
                text: PlayingSongViewModel.durationTime
                font.family: "MiSans"
                font.pixelSize: 14
                font.weight: 500
                color: Qt.rgba(1, 1, 1, 0.7)
            }
            MusicSlider {
                id: slider_progress
                anchors {
                    verticalCenter: text_start_time.verticalCenter
                    left: text_start_time.right
                    right: text_end_time.left
                    leftMargin: 8
                    rightMargin: 8
                }
                handleVisible: item_mouse_slider_progress.containsMouse
                active: true
                activeColor: "#fff"
                to: PlayingSongViewModel.duration
                value: PlayingSongViewModel.timeStamp
                onValueChanged: {
                    PlayingSongViewModel.timeStamp = value
                }
            }
            MouseArea {
                id: item_mouse_slider_progress
                propagateComposedEvents: true
                height: 15
                anchors {
                    left: slider_progress.left
                    right: slider_progress.right
                    top: text_singer.bottom
                    topMargin: 19
                }
                onClicked: mouse => mouse.accepted = false
                onPressAndHold: mouse => mouse.accepted = false
                onPressed: mouse => mouse.accepted = false
                onReleased: mouse => mouse.accepted = false
                hoverEnabled: true
            }
            RowLayout {
                anchors {
                    horizontalCenter: song_img.horizontalCenter
                    top: text_start_time.bottom
                    topMargin: 18
                }
                spacing: 16
                IconButton {
                    iconUrl: {
                        switch (NextUpViewModel.playMode) {
                        case NextUpViewModel.PlayOnce:
                            return "qrc:///res/img/repeat"
                        case NextUpViewModel.ListRepeat:
                            return "qrc:///res/img/repeat.svg"
                        case NextUpViewModel.RepeatOne:
                            return "qrc:///res/img/repeat-1.svg"
                        case NextUpViewModel.Shuffle:
                            return "qrc:///res/img/repeat.svg"
                        }
                    }
                    iconColor: NextUpViewModel.playMode
                               === NextUpViewModel.PlayOnce ? "#fff" : "#335eea"
                    iconWidth: 16
                    iconHeight: 16
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                    onClicked: {
                        if (NextUpViewModel.playMode === NextUpViewModel.RepeatOne)
                            NextUpViewModel.playMode = NextUpViewModel.PlayOnce
                        else if (NextUpViewModel.playMode === NextUpViewModel.PlayOnce)
                            NextUpViewModel.playMode = NextUpViewModel.ListRepeat
                        else
                            NextUpViewModel.playMode = NextUpViewModel.RepeatOne
                    }
                }
                IconButton {
                    iconUrl: "qrc:///res/img/previous.svg"
                    width: 38
                    height: 38
                    iconWidth: 22
                    iconHeight: 22
                    iconColor: "#fff"
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                    onClicked: {
                        PlayingSongViewModel.previous()
                    }
                }
                IconButton {
                    id: btn_play
                    property bool playing: PlayingSongViewModel.playing
                    width: 48
                    height: 48
                    radius: 10
                    iconWidth: 30
                    iconHeight: 30
                    iconColor: "#fff"
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                    iconUrl: playing ? "qrc:///res/img/pause.svg" : "qrc:///res/img/play.svg"
                    onClicked: {
                        playing ? PlayingSongViewModel.pause(
                                      ) : PlayingSongViewModel.play()
                    }
                    Layout.alignment: Qt.AlignCenter
                }
                IconButton {
                    iconUrl: "qrc:///res/img/next.svg"
                    width: 38
                    height: 38
                    iconWidth: 22
                    iconHeight: 22
                    iconColor: "#fff"
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                    onClicked: {
                        PlayingSongViewModel.next()
                    }
                }
                IconButton {
                    iconUrl: "qrc:///res/img/shuffle.svg"
                    iconColor: "#fff"
                    iconWidth: 16
                    iconHeight: 16
                    hoverColor: NextUpViewModel.playMode
                                === NextUpViewModel.Shuffle ? "#335eea" : Qt.rgba(
                                                                  1, 1, 1, 0.08)
                    onClicked: {
                        if (NextUpViewModel.playMode === NextUpViewModel.Shuffle)
                            NextUpViewModel.playMode = NextUpViewModel.ListRepeat
                        else
                            NextUpViewModel.playMode = NextUpViewModel.Shuffle
                    }
                }
            }
        }
    }
    IconButton {
        anchors {
            right: parent.right
            top: parent.top
            rightMargin: 20
            topMargin: 20
        }
        width: 44
        height: 44
        iconSource: FluentIcons.ChevronDownSmall
        iconSize: 18
        hoverColor: Qt.rgba(1, 1, 1, 0.08)
        iconColor: hovered ? "#fff" : Qt.rgba(1, 1, 1, 0.28)
        onClicked: {
            animationdown.start()
        }
        Connections {
            target: animationdown
            function onFinished() {
                rec_lyrics.y = Qt.binding(() => {
                                              return window.height
                                          })
            }
        }
    }
}
