import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtCore
import FluentUI
import sast_music
import "../component"
import "../"

ScrollablePage {
    objectName: "settings"
    property color backgroundColor: "#f5f5f7"
    property color fontColor: Qt.rgba(0, 0, 0, 0.78)

    leftRightMargin: (width - 720) / 2

    Rectangle {
        radius: 15
        color: backgroundColor
        width: 720
        height: 96
        FluClip {
            id: avatar
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 20
            }
            radius: [32, 32, 32, 32]
            width: 64
            height: 64
            Image {
                source: UserProfileViewModel.avatarUrl
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                cache: true
            }
        }
        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: avatar.right
                leftMargin: 24
            }
            spacing: 2
            Text {
                text: UserProfileViewModel.nickname

                font.weight: 600
                font.pixelSize: 20
            }
            Loader {
                sourceComponent: null // FIXME
            }

            Component {
                id: com_svip_logo
                RowLayout {
                    spacing: 2
                    FluClip {
                        width: 24.69
                        height: 13
                        radius: [2, 2, 2, 2]
                        Layout.alignment: Qt.AlignVCenter
                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            source: "qrc:///res/img/svip.png"
                        }
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter
                        text: "黑胶SVIP"

                        font.pixelSize: 13
                    }
                }
            }

            Component {
                id: com_vip_logo
                RowLayout {
                    spacing: 2
                    FluClip {
                        width: 24.69
                        height: 13
                        radius: [2, 2, 2, 2]
                        Layout.alignment: Qt.AlignVCenter
                        Image {
                            anchors.fill: parent
                            fillMode: Image.PreserveAspectCrop
                            source: "qrc:///res/img/vip.png"
                        }
                    }
                    Text {
                        Layout.alignment: Qt.AlignVCenter
                        text: "黑胶VIP"

                        font.pixelSize: 13
                    }
                }
            }
        }
    }

    Text {
        Layout.topMargin: 48
        text: "Sound"

        font.weight: 600
        font.pixelSize: 26
    }
    Rectangle {
        Layout.topMargin: 12
        width: 720
        height: 2
        color: backgroundColor
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_music_quality.implicitWidth - comboBox_music_quality.implicitWidth
        Text {
            id: text_music_quality
            text: "Music Quality"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: comboBox_music_quality
            model: [" Low - 128Kbps ", " Medium - 192Kbps ", " High - 320Kbps ", " Lossless - FLAC ", " Hi - Res "]
            currentIndex: getSettingsValue("musicQualityIndex", 2)
            onCurrentIndexChanged: {
                setSettingsValue("musicQualityIndex", currentIndex)
            }
            Component.onCompleted: {
                if (getSettingsValue("musicQualityIndex", 2) === 2)
                    setSettingsValue("musicQualityIndex", 2)
            }
        }
    }

    Row {
        Layout.topMargin: 24
        spacing: 720 - text_output_device.implicitWidth - comboBox_output_device.width
        Text {
            id: text_output_device
            text: "Audio Output device"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: comboBox_output_device
            width: 450
            textRole: "name"
            model: OutputDeviceViewModel
            currentIndex: OutputDeviceViewModel.currentIndex
            Component.onCompleted: {
                OutputDeviceViewModel.loadAudioOutputDevices()
                currentIndex = find(
                            OutputDeviceViewModel.getDefaultDeviceName())
            }
            onCurrentIndexChanged: {
                OutputDeviceViewModel.currentIndex = currentIndex
            }
        }
    }

    Text {
        Layout.topMargin: 48
        text: "Cache"

        font.weight: 600
        font.pixelSize: 26
    }
    Rectangle {
        Layout.topMargin: 12
        width: 720
        height: 2
        color: backgroundColor
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_automatically_cache_songs.implicitWidth
                 - btn_automatically_cache_songs.width
        Text {
            id: text_automatically_cache_songs
            text: "Automatically cache songs"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ToggleSwitch {
            id: btn_automatically_cache_songs
            disabled: true
            onClicked: {
                setSettingsValue("cacheOption", checked)
            }
            checked: getSettingsValue("cacheOption", true) === "true"
            Component.onCompleted: {
                if (getSettingsValue("cacheOption", true) === true)
                    setSettingsValue("cacheOption", true)
            }
        }
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_cache_limit.implicitWidth - comboBox_cache_limit.implicitWidth
        Text {
            id: text_cache_limit
            text: "Songs Cache limit"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: comboBox_cache_limit
            model: [" None ", " 500 MiB ", " 1 GiB ", " 2 GiB ", " 4 GiB ", " 8 GiB "]
            currentIndex: getSettingsValue("cacheMemoryIndex", 5)
            onCurrentIndexChanged: {
                setSettingsValue("cacheMemoryIndex", currentIndex)
            }
            Component.onCompleted: {
                if (getSettingsValue("cacheMemoryIndex", 5) === 5)
                    setSettingsValue("cacheMemoryIndex", 5)
            }
        }
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_cache_num.implicitWidth - btn_clear_cache.width
        Text {
            id: text_cache_num
            text: "Cached 0 songs (0 GiB)"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        TextButton {
            id: btn_clear_cache
            text: "Clear Song Cache"
            backgroundNormalColor: backgroundColor
            fontSize: 13
            hoverAnimation: true
        }
    }

    Text {
        Layout.topMargin: 48
        text: "Lyrics"

        font.weight: 600
        font.pixelSize: 26
    }
    Rectangle {
        Layout.topMargin: 12
        width: 720
        height: 2
        color: backgroundColor
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_show_translation.implicitWidth - btn_show_translation.width
        Text {
            id: text_show_translation
            text: "Show lyrics translation"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ToggleSwitch {
            id: btn_show_translation
            checked: getSettingsValue("showTranslationOption", false) === "true"
            disabled: false
            onClicked: {
                setSettingsValue("showTranslationOption", checked)
            }
            Component.onCompleted: {
                if (getSettingsValue("showTranslationOption", false) === false)
                    setSettingsValue("showTranslationOption", false)
            }
        }
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_font_size.implicitWidth - comboBox_font_size.implicitWidth
        Text {
            id: text_font_size
            text: "Songs Cache limit"

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: comboBox_font_size
            model: [" Small - 16px ", " Medium - 22px ", " Large(Default) - 28px ", " X-Large - 36px "]
            currentIndex: getSettingsValue("fontSizeIndex", 2)
            onCurrentIndexChanged: {
                setSettingsValue("fontSizeIndex", currentIndex)
            }
            Component.onCompleted: {
                if (getSettingsValue("fontSizeIndex", 2) === 2)
                    setSettingsValue("fontSizeIndex", 2)
            }
        }
    }

    Text {
        Layout.topMargin: 48
        text: "Others"

        font.weight: 600
        font.pixelSize: 26
    }
    Rectangle {
        Layout.topMargin: 12
        width: 720
        height: 2
        color: backgroundColor
    }
    Row {
        Layout.topMargin: 24
        spacing: 720 - text_close_app.implicitWidth - comboBox_close_app.implicitWidth
        Text {
            id: text_close_app
            text: "Close App..."

            font.weight: 500
            font.pixelSize: 16
            color: fontColor
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: comboBox_close_app
            model: [" Ask ", " Exit ", " Minimize to tray "]
            currentIndex: getSettingsValue("closeAppIndex", 0)
            onCurrentIndexChanged: {
                setSettingsValue("closeAppIndex", currentIndex)
            }
            Component.onCompleted: {
                if (getSettingsValue("closeAppIndex", 2) === 0)
                    setSettingsValue("closeAppIndex", 0)
            }
        }
    }
}
