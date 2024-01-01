import QtQuick 2.15
import QtQuick.Controls 2.15
import FluentUI
import "./component"

ApplicationWindow {
    id: window
    visible: true
    width: 1400
    height: 850
    title: "SAST Music"

    readonly property string activeColor: "#335eea"
    readonly property string homePageUrl: "qrc:///ui/page/T_home.qml"
    readonly property string explorePageUrl: "qrc:///ui/page/T_explore.qml"
    readonly property string libraryPageUrl: "qrc:///ui/page/T_library.qml"

    StackView {
        id: stackView
        anchors.fill: parent

        function isPageInStack(pageName) {
            for (var i = 0; i < stackView.depth; ++i) {
                if (stackView.get(i).objectName === pageName) {
                    return true;
                }
            }
            return false;
        }

        // This function pops pages until the target page is on top
        function popToTargetPage(targetPageName) {
            while (stackView.depth > 1 && stackView.currentItem.objectName !== targetPageName) {
                stackView.pop();
            }
        }

        // This function pushes a new page or pops to an existing instance of the page
        function pushOrPopToPage(pageUrl, pageName) {
            if (isPageInStack(pageName)) {
                popToTargetPage(pageName);
            } else {
                stackView.push(pageUrl, { objectName: pageName });
            }
        }

        Component.onCompleted: {
            pushOrPopToPage(homePageUrl, "home");
        }
    }

    Blur {
        id: navigationBar
        width: parent.width
        height: 60
        blurRadius: 100
        color: Qt.rgba(1, 1, 1, 0.99)
        target: stackView
        FluIconButton {
            id: btn_back
            anchors {
                left: parent.left
                leftMargin: 65
                verticalCenter: parent.verticalCenter
            }
            radius: 6
            hoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
            iconSource: FluentIcons.ChevronLeftSmall
            onClicked: {
                stackView.pop()
            }
        }

        Row {
            anchors.centerIn: parent
            spacing: 30

            FluTextButton {
                id: btn_home
                text: "HOME"
                font.family: "Barlow-Bold"
                font.pixelSize: 18
                font.weight: 700
                textColor: activeColor
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: {
                    stackView.pushOrPopToPage(homePageUrl, "home")
                    btn_home.textColor = activeColor
                    btn_explore.textColor = "#000"
                    btn_library.textColor = "#000"
                }
            }

            FluTextButton {
                id: btn_explore
                text: "EXPLORE"
                font.family: "Barlow-Bold"
                font.pixelSize: 18
                font.weight: 700
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: {
                    stackView.pushOrPopToPage(explorePageUrl, "explore")
                    btn_home.textColor = "#000"
                    btn_explore.textColor = activeColor
                    btn_library.textColor = "#000"
                }
            }

            FluTextButton {
                id: btn_library
                text: "LIBRARY"
                font.family: "Barlow-Bold"
                font.pixelSize: 18
                font.weight: 700
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: {
                    stackView.pushOrPopToPage(libraryPageUrl, "library")
                    btn_home.textColor = "#000"
                    btn_explore.textColor = "#000"
                    btn_library.textColor = activeColor
                }
            }
        }

        Rectangle {
            anchors {
                right: avatar.left
                rightMargin: 15
                verticalCenter: parent.verticalCenter
            }
            radius: 8
            width: 200
            height: 30
            color: "#F2F2F4"
            FluIcon {
                id: icon_search
                anchors {
                    left: parent.left
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                iconSource: FluentIcons.Search
                iconColor: "#767676"
            }
            FluText {
                anchors {
                    left: icon_search.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                text: "Search"
                font.pixelSize: 16
                font.family: "Barlow-Medium"
                font.weight: 550
                color: "#767676"
            }
        }

        Clip {
            id: avatar
            anchors {
                right: parent.right
                rightMargin: 65
                verticalCenter: parent.verticalCenter
            }
            width: 30
            height: 30
            radius: 15
            Image {
                anchors.fill: parent
                source: "qrc:///res/img/avatar.svg"
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {
                anchors.fill: parent
                color: item_mouse.containsMouse ? Qt.rgba(46/255, 46/255, 41/255, 0.28) : "transparent"
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
                font.family: "Barlow-Medium"
                font.bold: true
            }
            RadiusMenuItem {
                iconSize: 20
                iconUrl: "qrc:///res/img/logout.svg"
                text: "Logout"
                font.family: "Barlow-Medium"
                font.bold: true
            }
            MenuSeparator{}
            RadiusMenuItem {
                iconSize: 20
                iconUrl: "qrc:///res/img/github.svg"
                text: "GitHub Repo"
                font.family: "Barlow-Medium"
                font.bold: true
            }
        }
    }
}
