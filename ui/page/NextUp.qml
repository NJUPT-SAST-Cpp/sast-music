import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
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
