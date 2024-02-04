import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import sast_music
import FluentUI
import "../component"
import "../item"
import "../"

ScrollablePage {
    objectName: "library"
    id: library_page

    Component.onCompleted: {
        statusMode = FluStatusViewType.Loading
        startLoading()
        PlayListViewModel.loadPlayList(UserProfileViewModel.userId)
    }

    Connections {
        target: PlayListViewModel
        function onLoadPlayListSuccess() {
            LikedSongViewModel.loadLikedSongs(PlayListViewModel.likedPlayListId)
        }
    }

    Connections {
        target: LikedSongViewModel
        function onLoadSongsFailed(message) {
            showError(message, 4000)
            statusMode = FluStatusViewType.Error
        }
    }

    Connections {
        target: PlayListViewModel
        function onLoadPlayListFailed(message) {
            showError(message, 4000)
            statusMode = FluStatusViewType.Error
        }
    }

    onErrorClicked: {
        statusMode = FluStatusViewType.Loading
        startLoading()
        PlayListViewModel.loadPlayList(UserProfileViewModel.userId)
    }

    Connections {
        target: LikedSongViewModel
        function onLoadSongsSuccess() {
            endLoading()
            statusMode = FluStatusViewType.Success
        }
    }

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
            text: UserProfileViewModel.nickname + "'s Library"
            
            font.weight: 700
            font.pixelSize: 42
        }
    }

    Row {
        Layout.topMargin: 20
        spacing: 10
        Item {
            id: liked_song
            width: 410
            height: 260
            Layout.alignment: Qt.AlignHCenter
            Rectangle {
                id: liked_song_background
                color: "#3057d7"
                anchors.fill: parent
                radius: 25
                opacity: 0.1
            }
            Text {
                id: liked_song_num_text
                text: LikedSongViewModel.count + " Songs"
                elide: Text.ElideRight
                color: "#3057d7"
                maximumLineCount: 1
                
                font.bold: false
                font.pixelSize: 16
                anchors {
                    left: liked_song.left
                    leftMargin: 20
                    bottom: parent.bottom
                    bottomMargin: 20
                }
            }
            Text {
                id: liked_song_text
                text: "Liked Songs"
                elide: Text.ElideRight
                maximumLineCount: 1
                
                font.bold: true
                font.pixelSize: 23
                color: "#3057d7"
                anchors {
                    left: liked_song.left
                    leftMargin: 20
                    bottom: liked_song_num_text.bottom
                    bottomMargin: 20
                }
            }
            Text {
                id: liked_song_comment
                text: "好多人都浪漫\n好多人都心酸\n好聚好散"
                elide: Text.ElideRight
                
                font.pixelSize: 14
                font.bold: true
                color: "#3057d7"
                anchors {
                    left: parent.left
                    leftMargin: 22
                    top: parent.top
                    topMargin: 24
                }
                width: parent.width
            }
            MouseArea {
                id: liked_song_mousearea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    LikedSongViewModel.loadAllLikedSongs()
                    pushPage("qrc:///ui/page/LikedSongs.qml")
                }
            }
            Rectangle {
                id: liked_song_btn_rectangle
                color: "#3057d7"
                width: 60
                height: 60
                radius: 30
                anchors {
                    right: parent.right
                    rightMargin: 26
                    bottom: parent.bottom
                    bottomMargin: 26
                }
                FluShadow {
                    radius: parent.radius
                }
            }
            IconButton {
                id: liked_song_btn
                z: liked_song_btn_rectangle + 1
                width: 40
                height: 40
                iconWidth: 18
                iconHeight: 20
                iconUrl: "qrc:///res/img/play.svg"
                iconColor: "#fff"
                anchors.centerIn: liked_song_btn_rectangle
                onClicked: {
                    LikedSongViewModel.playAllSongs()
                }
                hoverColor: Qt.rgba(0, 0, 0, 0)
                onHoveredChanged: {
                    if (hovered)
                        liked_song_btn_rectangle.opacity = 0.65
                    else
                        liked_song_btn_rectangle.opacity = 1
                }
            }
        }
        Item {
            id: liked_grid_rectangle
            height: 260
            width: library_page.width - liked_song.width - 140
            Layout.fillWidth: true
            Grid {
                id: playList
                spacing: 10
                columns: 3
                rows: 4
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Repeater {
                    model: LikedSongViewModel
                    Rectangle {
                        property bool playing: PlayingSongViewModel.songId === model.songId
                        property bool hovered: false
                        width: (liked_grid_rectangle.width - 20) / 3
                        height: 58
                        Layout.topMargin: 10
                        Layout.alignment: Qt.AlignHCenter
                        radius: 18
                        color: playing ? "#eaeffd" : (hovered ? "#10000000" : "#00000000")
                        FluClip {
                            id: liked_grid_image_song
                            radius: [5, 5, 5, 5]
                            width: 45
                            height: 45
                            anchors {
                                left: parent.left
                                leftMargin: 20
                                verticalCenter: parent.verticalCenter
                            }
                            Image {
                                anchors.fill: parent
                                source: model.imgUrl
                                cache: true
                            }
                            FluShadow {}
                        }

                        Text {
                            id: liked_grid_text_song
                            text: model.name
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            
                            font.bold: true
                            font.pixelSize: 18
                            color: parent.playing ? "#3057d7" : "#000000"
                            anchors {
                                left: liked_grid_image_song.right
                                leftMargin: 20
                                top: parent.top
                                topMargin: 10
                            }
                            width: parent.width - 80
                        }
                        Text {
                            id: liked_grid_text_singer
                            text: model.artists
                            elide: Text.ElideRight
                            maximumLineCount: 1
                            
                            font.pixelSize: 11
                            color: parent.playing ? "#3057d7" : "#808080"
                            anchors {
                                left: liked_grid_image_song.right
                                leftMargin: 20
                                top: liked_grid_text_song.bottom
                                topMargin: 0
                            }
                            width: parent.width - 80
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                if (!parent.playing)
                                    parent.hovered = true
                            }
                            onExited: {
                                parent.hovered = false
                            }
                            onDoubleClicked: {
                                if (!parent.playing)
                                    LikedSongViewModel.playSongByIndex(index)
                            }
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: SongViewModel
        function onPrepareForPlaying() {
            SongViewModel.playAllSongs()
        }
    }

    Connections {
        target: SongViewModel
        function onLoadSongsFailed(message) {
            showError(message)
        }
    }

    GridView {
        id: grid_view
        interactive: false
        Layout.fillWidth: true
        Layout.topMargin: 35
        implicitHeight: contentHeight
        implicitWidth: contentWidth
        width: implicitWidth
        height: implicitHeight
        model: PlayListViewModel
        cellHeight: cellWidth * 1.2 + 20
        cellWidth: (library_page.width - 130) / 5
        delegate: Item {
            width: grid_view.cellWidth - 16
            height: width * 1.2
            PlayListCover {
                id: song_package_image_song
                width: parent.width
                source: model.coverImgUrl
                onClicked: {
                    SongViewModel.loadAndPlayAllSongs(model.playListId)
                }
                onShowSongList: {
                    SongViewModel.playlistId = model.playListId
                    SongViewModel.name = model.name
                    SongViewModel.coverImgUrl = model.coverImgUrl
                    SongViewModel.creatorName = model.creatorName
                    SongViewModel.description = model.description
                    SongViewModel.updateTime = model.updateTime
                    pushPage("qrc:///ui/page/PlayList.qml")
                }
            }
            Text {
                id: song_package_text_song
                text: model.name
                elide: Text.ElideRight
                maximumLineCount: 1
                
                font.bold: true
                font.pixelSize: 18
                anchors {
                    top: song_package_image_song.bottom
                    topMargin: 10
                }
                width: parent.width
            }
            Text {
                id: song_package_text_singer
                text: "by " + model.creatorName
                elide: Text.ElideRight
                maximumLineCount: 1
                
                font.pixelSize: 11
                anchors {
                    top: song_package_text_song.bottom
                    topMargin: 0
                }
                width: parent.width
            }
        }
    }
}
