import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"

Item {
    id: control
    property bool nowplay: false
    property color hoverBgColor: Qt.rgba(209 / 255, 209 / 255, 214 / 255, 0.28)
    property color activeBgColor: "#eaeffd"
    property color activeColor: "#335eea"

    property string lyric: "lyric"
    property string trlyric: "trlyric"

    property bool ishowtrlyric: true

    signal playClicked
    signal likedClicked(bool licked)

    height: 60
    Rectangle {
        id: background
        color: nowplay ? activeBgColor : mouseArea.containsMouse ? hoverBgColor : "transparent"
        anchors.fill: parent
        radius: 10
    }

    Text {
        id: text_lyric
        text: control.lyric
        elide: Text.ElideRight
        maximumLineCount: 1
        font.family: "MiSans"
        font.bold: true
        font.pixelSize: ishowtrlyric ? 16 : 32
        color: nowplay ? activeColor : "white" //"#000"
        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 10
        }
    }

    Text {
        id: text_trlyric
        text: control.trlyric
        elide: Text.ElideRight
        width: 520
        maximumLineCount: 1
        font.family: "MiSans"
        font.pixelSize: 16
        color: nowplay ? activeColor : "white" //"#000"
        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 40
        }
        enabled: ishowtrlyric
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onDoubleClicked: {
            control.playClicked()
        }
    }
}
