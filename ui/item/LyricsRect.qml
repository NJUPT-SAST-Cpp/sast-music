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
        onWheel: wheel => wheel.accepted = true
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
                MusicFullscreenInfo {
                    height: rec_lyrics.height
                    width: rec_lyrics.width * 0.4
                }

                // TODO: lyrics
                ListView {
                    id: lyric
                    width: rec_lyrics.width * 0.5
                    height: rec_lyrics.height * 0.8
                    Layout.alignment: Qt.AlignVCenter
                    property bool autoCenter: true
                    currentIndex: SongLyricViewModel.currentLyricIndex
                    clip: true
                    reuseItems: true
                    preferredHighlightBegin: width * 0.45
                    preferredHighlightEnd: width * 0.5
                    highlightRangeMode: ListView.ApplyRange
                    model: SongLyricViewModel
                    delegate: LyricLine {
                        width: ListView.view.width
                        onLyricTapped: time => {
                                           PlayingSongViewModel.timeStamp = time
                                           lyric.centerCurrItem()
                                       }
                    }
                    header: Item {
                        width: ListView.view.width
                        height: ListView.view.height / 2
                    }
                    footer: Item {
                        width: ListView.view.width
                        height: ListView.view.height / 2
                    }

                    // highlight: Component {
                    //     Rectangle {
                    //         color: "#88FFFFFF"
                    //     }
                    // }
                    spacing: 5

                    Timer {
                        id: autoCenterTimer
                        running: false
                        interval: 5000
                        onTriggered: {
                            lyric.autoCenter = true
                            lyric.centerCurrItem()
                        }
                    }

                    onMovementStarted: {
                        console.log("listview.move start")
                        autoCenterTimer.running = false
                        autoCenter = false
                    }

                    onMovementEnded: {
                        console.log("listview.move end")
                        autoCenterTimer.running = true
                    }

                    Connections {
                        target: PlayingSongViewModel
                        function onSongIdChanged() {
                            SongLyricViewModel.currentLyricIndex = 0
                        }
                    }

                    function centerCurrItem() {
                        positionViewAtIndex(
                                    SongLyricViewModel.currentLyricIndex,
                                    ListView.Center)
                        // SongLyricViewModel.currentLyricIndex=SongLyricViewModel.currentLyricIndex
                        // currentIndex++
                        // currentIndex--
                    }
                }
            }
        }
    }
    Component {
        id: com_no_lyric_music
        MusicFullscreenInfo {
            height: rec_lyrics.height
            width: rec_lyrics.width
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
