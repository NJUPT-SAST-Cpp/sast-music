import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"

ScrollablePage {
    id: play_list_root
    objectName: "playList"

    property var currentSelectedItem: null

    Text {
        id: text_nowPlay
        text: "Now Playing"
        font.family: "Barlow-Bold"
        font.pixelSize: 30
        font.bold: true
    }

    Rectangle {
        id: now_play
        width: 1300
        height: 60
        Layout.topMargin: 10
        Layout.alignment: Qt.AlignHCenter

        Rectangle {
            id: now_play_background
            color: "#3057d7"
            anchors.fill: parent
            radius: 10
            opacity: 0.2
        }

        FluClip {
            id: now_play_image_song
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
                source: "qrc:///res/img/netease-music.png"
            }
            FluShadow {}
        }

        Text {
            id: now_play_text_song
            text: "Song"
            elide: Text.ElideRight
            maximumLineCount: 1
            font.family: "Barlow-Bold"
            font.bold: true
            font.pixelSize: 16
            color: "#3057d7"
            anchors {
                left: now_play_image_song.right
                leftMargin: 10
                top: parent.top
                topMargin: 10
            }
        }

        Text {
            id: now_play_subheading
            text: "(subheading)"
            elide: Text.ElideRight
            color: "#3057d7"
            maximumLineCount: 1
            font.family: "Barlow-Bold"
            font.bold: true
            font.pixelSize: 16
            anchors {
                left: now_play_text_song.right
                leftMargin: 2
                top: parent.top
                topMargin: 10
            }
        }

        Text {
            id: now_play_text_singer
            text: "singer"
            elide: Text.ElideRight
            maximumLineCount: 1
            font.family: "Barlow-Bold"
            font.pixelSize: 12
            color: "#73706c"
            anchors {
                left: now_play_image_song.right
                leftMargin: 10
                top: parent.top
                topMargin: 35
            }
        }

        Text {
            id: now_play_album
            text: "album"
            elide: Text.ElideRight
            maximumLineCount: 1
            font.family: "Barlow-Bold"
            font.pixelSize: 16
            color: "#3057d7"
            anchors {
                left: parent.left
                leftMargin: 400
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id: now_play_text_time
            text: "4:12"
            color: play_list_root.playingSelected && !play_list_root.currentSelectedItem ? "#3057d7" : "black"
            anchors {
                right: parent.right
                rightMargin: 20
                verticalCenter: parent.verticalCenter
            }
        }

        IconButton {
            id: now_play_btn_like
            property bool liked: false
            width: 40
            height: 40
            iconWidth: 25
            iconHeight: 25
            iconUrl: liked ? "qrc:///res/img/heart.svg" : "qrc:///res/img/heart-solid.svg"
            anchors {
                right: now_play_text_time.left
                rightMargin: 20
                verticalCenter: parent.verticalCenter
            }
            onClicked: {
                liked = !liked
            }
        }

        MouseArea {
            id: now_play_mouseArea
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                play_list_root.playingSelected = !play_list_root.playingSelected;
                play_list_root.currentSelectedItem = null;
            }
        }
    }

    Text {
        id: text_nextUP
        text: "Next UP"
        font.family: "Barlow-Bold"
        font.pixelSize: 30
        font.bold: true
        Layout.topMargin: 10
        Layout.alignment: Qt.AlignTop
    }

    ListView {
        id: playList
        width: 1300
        implicitHeight: contentHeight
        spacing: 10
        Layout.topMargin: 10
        Layout.alignment: Qt.AlignHCenter
        clip: true

        model: 20

        delegate: Item {
            width: playList.width
            height: 60

            Rectangle {
                id: background
                color: (play_list_root.currentSelectedItem === index || mouseArea.containsMouse) ? "gray" : "white"
                anchors.fill: parent
                radius: 10
                opacity: (play_list_root.currentSelectedItem === index || mouseArea.containsMouse) ? 0.2 : 0
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
                    source: "qrc:///res/img/netease-music.png"
                }
                FluShadow {}
            }

            Text {
                id: text_song
                text: "Song"
                elide: Text.ElideRight
                maximumLineCount: 1
                font.family: "Barlow-Bold"
                font.bold: true
                font.pixelSize: 16
                color: "black"
                anchors {
                    left: image_song.right
                    leftMargin: 10
                    top: parent.top
                    topMargin: 10
                }
            }

            Text {
                id: subheading
                text: "(subheading)"
                elide: Text.ElideRight
                color: (play_list_root.currentSelectedItem === index) ? "#73706c" : "#9b9ba0"
                maximumLineCount: 1
                font.family: "Barlow-Bold"
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
                text: "singer"
                elide: Text.ElideRight
                maximumLineCount: 1
                font.family: "Barlow-Bold"
                font.pixelSize: 12
                color: (play_list_root.currentSelectedItem === index) ? "#73706c" : "#9b9ba0"
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
                font.family: "Barlow-Bold"
                font.pixelSize: 16
                color: "black"
                anchors {
                    left: parent.left
                    leftMargin: 400
                    verticalCenter: parent.verticalCenter
                }
            }

            Text {
                id: text_time
                text: "4:12"
                color: "black"
                anchors {
                    right: parent.right
                    rightMargin: 20
                    verticalCenter: parent.verticalCenter
                }
            }

            IconButton {
                id: btn_like
                property bool liked: false
                width: 40
                height: 40
                iconWidth: 25
                iconHeight: 25
                iconUrl: liked ? "qrc:///res/img/heart.svg" : "qrc:///res/img/heart-solid.svg"
                anchors {
                    right: text_time.left
                    rightMargin: 20
                    verticalCenter: parent.verticalCenter
                }
                onClicked: {
                    liked = !liked
                }
            }

            IconButton {
                id: btn_play
                property bool playing: true
                anchors {
                    right: btn_like.left
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                width: 40
                height: 40
                radius: 10
                iconWidth: 20
                iconHeight: 25
                iconUrl: playing ? "qrc:///res/img/play.svg" : "qrc:///res/img/pause.svg"
                onClicked: {
                    playing = !playing
                }
            }

            MouseArea {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    play_list_root.currentSelectedItem = index;
                }
            }
        }
    }
}
