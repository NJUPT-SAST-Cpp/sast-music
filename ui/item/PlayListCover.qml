import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import FluentUI
import "../component"

Rectangle {
    id: control
    property string source: "qrc:///res/img/background.png"
    width: 50
    height: width
    radius: 20
    signal clicked
    signal showSongList

    FluClip {
        id: song_package_image_song
        radius: [20, 20, 20, 20]
        width: parent.width
        height: width
        Image {
            anchors.fill: parent
            source: control.source
            cache: true
        }
        FluShadow {
            radius: 20
        }
    }
    MouseArea {
        id: song_package_mousearea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            control.showSongList()
        }
    }
    IconButton {
        id: song_package_btn
        width: 50
        height: 50
        radius: 25
        iconWidth: 18
        iconHeight: 20
        iconUrl: "qrc:///res/img/play.svg"
        iconColor: "#fff"
        anchors.centerIn: parent
        visible: song_package_mousearea.containsMouse || hovered
        onClicked: {
            control.clicked()
        }
        background: FluClip {
            radius: [25, 25, 25, 25]
            width: 50
            height: 50
            id: song_package_btn_blur_background
            anchors.centerIn: parent
            BlurRectangle {
                id: song_package_btn_blurrec
                anchors.centerIn: parent
                width: 50
                height: 50
                color: "#50ffffff"
                target: song_package_image_song
                blurRadius: song_package_btn.hovered ? 40 : 20
            }
        }
    }
}
