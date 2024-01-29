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
        source: "qrc:///res/img/background.png"
        cache: true
    }
    FluAcrylic {
        anchors.fill: parent
        target: img_bg
        blurRadius: 100
        tintOpacity: 0
        tintColor: Qt.rgba(0, 0, 0, 0.28)
    }
    Loader {
        anchors.fill: parent
        sourceComponent: com_pure_music
    }
    Component {
        id: com_pure_music
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
                    source: "qrc:///res/img/background.png"
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
                text: "Song"
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
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
                text: "Singer"
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
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
                text: "0:38"
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
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
                text: "4:09"
                font.family: "Barlow,ui-sans-serif,system-ui,-apple-system,BlinkMacSystemFont,MiSans,Helvetica Neue,PingFang SC,Microsoft YaHei,Source Han Sans SC,Noto Sans CJK SC,WenQuanYi Micro Hei,sans-serif,microsoft uighur"
                font.pixelSize: 14
                font.weight: 500
                color: Qt.rgba(1, 1, 1, 0.7)
            }
            MusicSlider {
                id: slider_progress
                value: 20
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
                    iconUrl: "qrc:///res/img/repeat.svg"
                    iconColor: "#fff"
                    iconWidth: 16
                    iconHeight: 16
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                }
                IconButton {
                    iconUrl: "qrc:///res/img/previous.svg"
                    width: 38
                    height: 38
                    iconWidth: 22
                    iconHeight: 22
                    iconColor: "#fff"
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                }
                IconButton {
                    id: btn_play
                    property bool playing: true
                    width: 48
                    height: 48
                    radius: 10
                    iconWidth: 30
                    iconHeight: 30
                    iconColor: "#fff"
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
                    iconUrl: playing ? "qrc:///res/img/play.svg" : "qrc:///res/img/pause.svg"
                    onClicked: {
                        playing = !playing
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
                }
                IconButton {
                    iconUrl: "qrc:///res/img/shuffle.svg"
                    iconColor: "#fff"
                    iconWidth: 16
                    iconHeight: 16
                    hoverColor: Qt.rgba(1, 1, 1, 0.08)
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
