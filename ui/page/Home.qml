import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"

ScrollablePage {
    objectName: "home"

    FluImage {
        Layout.topMargin: 100
        Layout.alignment: Qt.AlignHCenter
        source: "qrc:///res/img/sast_music.svg"
        height: 200
        fillMode: Image.PreserveAspectFit
    }
}
