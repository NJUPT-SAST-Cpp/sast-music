import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import Qt.labs.platform
import QtCore
import FluentUI
import sast_music
import "./component"
import "./item"

ApplicationWindow {
    id: window
    visible: true
    width: 1400
    height: 850
    minimumWidth: 1050
    minimumHeight: 720
    title: "SAST Music"

    readonly property color activeColor: "#335eea"
    readonly property color backgroundColor: Qt.rgba(209 / 255, 209 / 255,
                                                     214 / 255, 0.28)
    readonly property string homePageUrl: "qrc:///ui/page/Home.qml"
    readonly property string explorePageUrl: "qrc:///ui/page/Explore.qml"
    readonly property string libraryPageUrl: "qrc:///ui/page/Library.qml"
    property string topPageUrl
    property var undoStack: []
    property var redoStack: []
    property int volumeValue: 100

    StackView {
        id: stackView
        anchors.fill: parent

        Component.onCompleted: {
            pushPage(homePageUrl)
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
                pushOrPopToPage(topPageUrl, url2Name(topPageUrl))
            }
        }

        function redoPage() {
            if (redoStack.length >= 1) {
                topPageUrl = redoStack[redoStack.length - 1]
                redoStack.pop()
                pushOrPopToPage(topPageUrl, url2Name(topPageUrl))
            }
        }
    }

    NavigationBar {
        stackView: stackView
        width: parent.width
        topPageUrl: window.topPageUrl
    }

    PlayerBar {
        id: playerBar
        stackView: stackView
        animationup: animationup
        volumeValue: window.volumeValue
        rec_lyrics: rec_lyrics
        width: parent.width
        anchors {
            bottom: parent.bottom
        }
    }

    MusicSlider {
        id: progress
        width: parent.width
        height: 2
        radius: 0
        active: true
        handleVisible: item_mouse_progress.containsMouse
        normalColor: "#e6e6e6"
        tooltipEnabled: true
        tipText: "1:32"
        anchors {
            bottom: playerBar.top
        }
    }

    MouseArea {
        id: item_mouse_progress
        hoverEnabled: true
        propagateComposedEvents: true
        height: 20
        z: progress.z + 1
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: 55
        }
        onClicked: mouse => mouse.accepted = false
        onPressAndHold: mouse => mouse.accepted = false
        onPressed: mouse => mouse.accepted = false
        onReleased: mouse => mouse.accepted = false
    }

    LyricsRect {
        id: rec_lyrics
        width: parent.width
        height: parent.height
        y: window.height
        windowHeight: window.height
        volumeValue: window.volumeValue
        animationdown: animationdown
    }

    NumberAnimation {
        id: animationdown
        target: rec_lyrics
        property: "y"
        from: 0
        to: window.height
        duration: 400
        easing.type: Easing.InOutQuad
    }
    NumberAnimation {
        id: animationup
        target: rec_lyrics
        property: "y"
        from: window.height
        to: 0
        duration: 400
        easing.type: Easing.InOutQuad
    }

    SystemTray {
        id: system_tray
        window: window
    }

    FluContentDialog {
        id: dialog_close
        title: "Confirmed Close?"
        buttonFlags: FluContentDialogType.NegativeButton | FluContentDialogType.NeutralButton
                     | FluContentDialogType.PositiveButton
        positiveText: "exit"
        negativeText: "minimize to tray"
        neutralText: "cancel"
        onPositiveClicked: {
            if (checkBox.checked)
                settings.setValue("closeAppIndex", 1)
            Qt.exit(0)
        }
        onNegativeClicked: {
            if (checkBox.checked)
                settings.setValue("closeAppIndex", 2)
            window.hide()
        }
        FluCheckBox {
            id: checkBox
            anchors {
                left: parent.left
                leftMargin: 20
                verticalCenter: parent.verticalCenter
            }
            text: "Remember my choice"
        }
    }

    onClosing: event => {
                   var closeApp = settings.value("closeAppIndex", 0)
                   if (closeApp == 0) {
                       settings.setValue("closeAppIndex", 0)
                       dialog_close.open()
                   } else if (closeApp == 1) {
                       Qt.exit(0)
                   } else if (closeApp == 2) {
                       window.hide()
                   }
                   event.accepted = false
               }

    function isPageInStack(pageName) {
        for (var i = 0; i < stackView.depth; ++i) {
            if (stackView.get(i).objectName === pageName) {
                return true
            }
        }
        return false
    }

    // This function pops pages until the target page is on top
    function popToTargetPage(targetPageName) {
        while (stackView.depth > 1
               && stackView.currentItem.objectName !== targetPageName) {
            stackView.pop()
        }
    }

    // This function pushes a new page or pops to an existing instance of the page
    function pushOrPopToPage(pageUrl, pageName) {
        if (isPageInStack(pageName)) {
            popToTargetPage(pageName)
        } else {
            stackView.push(pageUrl, {
                               "objectName": pageName
                           })
        }
        topPageUrl = pageUrl
    }

    function pushPage(url) {
        stackView.pushPage(url)
    }

    function url2Name(url) {
        if (url === homePageUrl)
            return "home"
        if (url === explorePageUrl)
            return "explore"
        if (url === libraryPageUrl)
            return "library"
        if (url === "qrc:///ui/page/Settings.qml")
            return "settings"
        if (url === "qrc:///ui/page/Login.qml")
            return "login"
        if (url === "qrc:///ui/page/SearchResult.qml")
            return "searchResult"
        if (url === "qrc:///ui/page/PlayList.qml")
            return "playList"
        if (url === "qrc:///ui/page/SongListInfo.qml")
            return "songListInfo"
        if (url === "qrc:///ui/page/PrivateRader.qml")
            return "privateRadar"
        if (url === "qrc:///ui/page/LikedSongs.qml")
            return "likedSongs"
    }

    function getSettingsValue(item, defaultItem) {
        return settings.value(item, defaultItem)
    }

    function setSettingsValue(item, itemVaue) {
        settings.setValue(item, itemVaue)
    }

    Settings {
        id: settings
        property int musicQualityIndex
        property bool showTranslationOption
        property bool cacheOption
        property int cacheMemoryIndex
        property int fontSizeIndex
        property int closeAppIndex
    }
}
