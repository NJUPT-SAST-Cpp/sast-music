import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"

BlurRectangle {
    id: navigationBar
    readonly property color backgroundColor: Qt.rgba(209 / 255, 209 / 255,
                                                     214 / 255, 0.28)
    readonly property color activeColor: "#335eea"
    readonly property string homePageUrl: "qrc:///ui/page/T_home.qml"
    readonly property string explorePageUrl: "qrc:///ui/page/T_explore.qml"
    readonly property string libraryPageUrl: "qrc:///ui/page/T_library.qml"
    property Item stackView
    property string topPageUrl
    height: 60
    blurRadius: 100
    target: stackView
    Row {
        spacing: 5
        anchors {
            left: parent.left
            leftMargin: 65
            verticalCenter: parent.verticalCenter
        }
        IconButton {
            id: btn_back
            width: 40
            height: 40
            hoverColor: backgroundColor
            iconWidth: 22
            iconHeight: 22
            iconUrl: "qrc:///res/img/arrow-left.svg"
            onClicked: stackView.popPage()
        }
        IconButton {
            id: btn_redo
            width: 40
            height: 40
            hoverColor: backgroundColor
            iconWidth: 22
            iconHeight: 22
            iconUrl: "qrc:///res/img/arrow-right.svg"
            onClicked: stackView.redoPage()
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: 30

        TextButton {
            id: btn_home
            text: "HOME"
            textColor: topPageUrl === homePageUrl ? activeColor : "#000"
            onClicked: stackView.pushPage(homePageUrl)
        }

        TextButton {
            id: btn_explore
            text: "EXPLORE"
            textColor: topPageUrl === explorePageUrl ? activeColor : "#000"
            onClicked: stackView.pushPage(explorePageUrl)
        }

        TextButton {
            id: btn_library
            text: "LIBRARY"
            textColor: topPageUrl === libraryPageUrl ? activeColor : "#000"
            onClicked: stackView.pushPage(libraryPageUrl)
        }
    }

    SearchBox {
        anchors {
            right: avatar.left
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }
        onCommit: content => {// TODO: post search request
                  }
    }

    FluClip {
        id: avatar
        anchors {
            right: parent.right
            rightMargin: 65
            verticalCenter: parent.verticalCenter
        }
        width: 30
        height: 30
        radius: [15, 15, 15, 15]
        Image {
            anchors.fill: parent
            source: "qrc:///res/img/avatar.svg"
            fillMode: Image.PreserveAspectFit
        }

        Rectangle {
            anchors.fill: parent
            color: item_mouse.containsMouse ? Qt.rgba(46 / 255,
                                                      46 / 255, 41 / 255,
                                                      0.28) : "transparent"
        }

        MouseArea {
            id: item_mouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                menu.popup()
            }
        }
    }

    RadiusMenu {
        id: menu
        radius: 15
        RadiusMenuItem {
            iconSize: 20
            iconUrl: "qrc:///res/img/settings.svg"
            text: "Settings"
            font.family: "Barlow-Bold"
            font.bold: true
        }
        RadiusMenuItem {
            iconSize: 20
            iconUrl: "qrc:///res/img/logout.svg"
            text: "Logout"
            font.family: "Barlow-Bold"
            font.bold: true
        }
        MenuSeparator {}
        RadiusMenuItem {
            iconSize: 20
            iconUrl: "qrc:///res/img/github.svg"
            text: "GitHub Repo"
            font.family: "Barlow-Bold"
            font.bold: true
            onClicked: {
                Qt.openUrlExternally(
                            "https://github.com/NJUPT-SAST-Cpp/sast-music")
            }
        }
    }

    Pane {
        z: -1
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }
}
