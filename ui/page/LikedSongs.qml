import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
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
                source: "qrc:///res/img/avatar.svg"
                fillMode: Image.PreserveAspectFit
                cache: true
            }
        }
        Text {
            anchors.verticalCenter: avatar.verticalCenter
            text: "Username's Liked Songs"
            font.family: "Misans"
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
        model: 20

        delegate: MusicBlock {
            width: playList.width
            onPlayClicked: playing => {//TODO
                           }
            onLikedChanged: liked => {//TODO
                            }
        }
    }
}
