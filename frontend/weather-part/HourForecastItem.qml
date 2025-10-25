import QtQuick 6.5
import QtQuick.Controls

Rectangle {
    property var forecastData: ({})
    property bool expanded: false

    width: parent.width
    height: expanded
     ? 350
     : 80
    color: expanded
     ? "#f8f9fa"
     : "#ffffff"
    radius: 8
    border.color: expanded
     ? "#4a90e2"
     : "#e0e0e0"
    border.width: expanded
     ? 2
     : 1

    Behavior on height {
        NumberAnimation { duration: 200 }
    }

    // Компактный вид (свёрнутый)
    CompactWeather {
        anchors.fill: parent
        anchors.margins: 4
        visible: !expanded
        weatherData: forecastData
    }

    // Подробный вид (развёрнутый) - компактный режим
    CurrentWeather {
        anchors.fill: parent
        anchors.margins: 4
        visible: expanded
        weatherData: forecastData
        compactMode: true  // Включаем компактный режим для списка
    }

    MouseArea {
        anchors.fill: parent
        onClicked: expanded = !expanded
        cursorShape: Qt.PointingHandCursor
    }

    Text {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 6
        }
        text: expanded ? "▲" : "▼"
        font.pixelSize: 10
        color: "#999999"
        visible: !expanded
    }
}
