import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"

ScrollablePage {
    objectName: "home"

    Component.onCompleted: {
        UserProfileViewModel.loadUserProfile()
    }

    Connections {
        target: UserProfileViewModel
        function onLoadUserProfileFailed(message) {
            showError(message)
        }
    }

    FluImage {
        Layout.topMargin: 100
        Layout.alignment: Qt.AlignHCenter
        source: "qrc:///res/img/sast_music.svg"
        height: 200
        fillMode: Image.PreserveAspectFit
        cache: true
    }
}
