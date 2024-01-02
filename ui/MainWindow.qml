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

    readonly property color activeColor: "#335eea"
    readonly property string homePageUrl: "qrc:///ui/page/T_home.qml"
    readonly property string explorePageUrl: "qrc:///ui/page/T_explore.qml"
    readonly property string libraryPageUrl: "qrc:///ui/page/T_library.qml"
    property string topPageUrl
    property var undoStack: []
    property var redoStack: []

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
            topPageUrl = pageUrl
        }

        function pushPage(url) {
            pushOrPopToPage(url, url2Name(url))
            undoStack.push(url)
        }

        function popPage() {
            if (undoStack.length >= 2) {
                redoStack.push(undoStack[undoStack.length - 1])
                undoStack.pop()
                topPageUrl = undoStack[undoStack.length - 1]
                stackView.pushOrPopToPage(topPageUrl, url2Name(topPageUrl))
            }
        }

        function redoPage() {
            if (redoStack.length >= 1) {
                topPageUrl = redoStack[redoStack.length - 1]
                redoStack.pop()
                stackView.pushOrPopToPage(topPageUrl, url2Name(topPageUrl))
            }
        }

        function url2Name(url) {
            if (url === homePageUrl) return "home"
            if (url === explorePageUrl) return "explore"
            if (url === libraryPageUrl) return "library"
        }

        Component.onCompleted: {
            pushPage(homePageUrl);
        }
    }

    BlurRectangle {
        id: navigationBar
        width: parent.width
        height: 60
        blurRadius: 100
        color: Qt.rgba(1, 1, 1, 0.99)
        target: stackView
        Row {
            spacing: 5
            anchors {
                left: parent.left
                leftMargin: 65
                verticalCenter: parent.verticalCenter
            }
            FluIconButton {
                id: btn_back
                radius: 6
                hoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                iconSource: FluentIcons.ChevronLeftSmall
                onClicked: stackView.popPage()
            }
            FluIconButton {
                id: btn_redo
                radius: 6
                hoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                iconSource: FluentIcons.ChevronRightSmall
                onClicked: stackView.redoPage()
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
                textColor: topPageUrl === homePageUrl ? activeColor : "#000"
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: stackView.pushPage(homePageUrl)
            }

            FluTextButton {
                id: btn_explore
                text: "EXPLORE"
                font.family: "Barlow-Bold"
                font.pixelSize: 18
                font.weight: 700
                textColor: topPageUrl === explorePageUrl ? activeColor : "#000"
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: stackView.pushPage(explorePageUrl)
            }

            FluTextButton {
                id: btn_library
                text: "LIBRARY"
                font.family: "Barlow-Bold"
                font.pixelSize: 18
                font.weight: 700
                textColor: topPageUrl === libraryPageUrl ? activeColor : "#000"
                implicitHeight: 30
                normalColor: Qt.rgba(0, 0, 0, 1)
                backgroundHoverColor: Qt.rgba(209/255, 209/255, 214/255, 0.28)
                onClicked: stackView.pushPage(libraryPageUrl)
            }
        }

        SearchBox {
            anchors {
                right: avatar.left
                rightMargin: 15
                verticalCenter: parent.verticalCenter
            }
            onCommit: (content) => {
                        // TODO: post search request
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
                onClicked: {
                    Qt.openUrlExternally("https://github.com/NJUPT-SAST-Cpp/sast-music")
                }
            }
        }

        Pane {
            z: -1
            anchors.fill: parent
            focusPolicy: Qt.ClickFocus
        }
    }
}
