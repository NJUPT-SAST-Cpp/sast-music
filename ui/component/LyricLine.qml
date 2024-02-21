import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    // required property int index
    required property int timeStamp
    required property string content
    property bool isCurrent: ListView.isCurrentItem
    signal lyricTapped(int time)

    height: row.height
    color: "#88000000"
    radius: 10
    TapHandler {
        onTapped: {
            lyricTapped(timeStamp);
        }
    }
    HoverHandler {
        id: hover
    }
    Row {
        id: row
        width: parent.width
        spacing: 5
        Text {
            anchors.verticalCenter: parent.verticalCenter
            id: timeText
            text: root.milsec2Time(timeStamp)
            font.pointSize: 16
            color: hover.hovered ? "red" : "white"
        }
        Text {
            id: contentText
            text: content
            font.pointSize: isCurrent ? 24 : 16
            color: "white"
            opacity: 1
        }
    }
    function milsec2Time(milsec) {
        var seconds = parseInt(milsec / 1000);
        var minutes = parseInt(seconds / 60);
        seconds %= 60;
        if (seconds < 10) {
            seconds = "0" + String(seconds);
        }
        var result = "%1:%2";
        return result.arg(minutes).arg(seconds);
    }
}
