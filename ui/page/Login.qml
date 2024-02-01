import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import FluentUI
import sast_music
import "../component"
import "../"

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
        font.family: "MiSans"
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
            text: LoginViewModel.qrCodeData
            color: "#4169E1"
            bgColor: "#f2f4ff"
            size: 280
            anchors.centerIn: parent
        }
    }

    Text {
        id: text_LoginTip
        text: {
            switch (LoginViewModel.status) {
            case 0:
                return "打开网易云音乐APP扫码登录"
            case 1:
                return "扫码成功，请在手机上确认登录"
            case 2:
            {
                UserProfileViewModel.loadUserProfile()
                return "登录成功"
            }
            case 4:
                return "二维码已失效，请刷新"
            default:
                return "未知错误"
            }
        }
        font.family: "MiSans"
        font.pixelSize: 18
        font.bold: false
        Layout.alignment: Qt.AlignHCenter
        Layout.topMargin: 15
    }

    Connections {
        target: LoginViewModel
        function onLoginQRCodeNewFailed(message) {
            showError(message, 4000)
        }
    }

    Connections {
        target: LoginViewModel
        function onLoginQRCodePollingFailed(message) {
            showError(message, 4000)
        }
    }

    Connections {
        target: UserProfileViewModel
        function onLoadUserProfileSuccess() {
            returnPage()
        }
    }

    Component.onCompleted: {
        LoginViewModel.newLoginQRCode()
    }

    function returnPage() {
        pushPage("qrc:///ui/page/Home.qml")
    }
}
