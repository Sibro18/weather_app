import QtQuick 6.5
import QtQuick.Controls

Rectangle {
    property var forecastData: ({})

    width: parent.width
    height: 80
    radius: 12
    color: mouseArea.containsMouse ? "#f8f9fa" : "#ffffff"
    border.color: mouseArea.containsMouse ? "#4a90e2" : "#e0e0e0"
    border.width: 1

    layer.enabled: true

    CompactWeather {
        anchors.fill: parent
        anchors.margins: 4
        weatherData: forecastData
    }

    // Details modal
    DetailedWeather {
        id: detailsModal
        weatherData: forecastData
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: detailsModal.open()
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true
    }

    Behavior on color {
        ColorAnimation { duration: 200 }
    }

    Behavior on border.color {
        ColorAnimation { duration: 200 }
    }
}
