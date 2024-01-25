import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import "../component"

ScrollablePage {
    objectName: "login"

    FluClip {
        id: neteaseicon
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 25
        width: 80
        height: 80
        Image {
            source: "qrc:///res/img/netease-music.png"
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            cache: true
        }
    }

    Text {
        id: text_LoginToNetease
        text: "Login to Netease"
        font.family: "Barlow-Bold"
        font.pixelSize: 24
        font.bold: true
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 40
    }

    Rectangle {
        id: background_qcode
        color: "#f2f4ff"
        radius: 20
        width: 350
        height: 350
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 40

        FluQRCode {
            id: qrcode_login
            text: "login"
            color: "#4169E1"
            bgColor: "#f2f4ff"
            size: 280
            anchors.centerIn: parent
        }
    }

    Text {
        id: text_LoginTip
        text: "打开网易云音乐APP扫码登录"
        font.family: "Barlow"
        font.pixelSize: 18
        font.bold: false
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 15
    }
}
