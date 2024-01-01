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
    }
}
