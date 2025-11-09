import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts
import "falling-list" as FL
import "weather-part" as WP

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Weather forecast client"
    property string currentCity: "";
    property string currentRegion: "";

    Component.onCompleted: {
        weatherForecastController.fetchDataAsync({
            "resource": "forecast",
            "latitude": 37.61556,
            "longitude": 55.75222
        });
        currentCity = "Moscow"
        currentRegion = "Moscow"
    }

    // Error alert
    Rectangle {
        id: errorAlert
        width: 280
        height: 90
        color: "#ff4444"
        radius: 8
        border.color: "#cc0000"
        border.width: 1

        anchors {
            top: parent.top
            left: parent.left
            margins: 10
        }

        visible: false
        opacity: 0
        z: 1000

        Text {
            id: errorText
            anchors.centerIn: parent
            anchors.margins: 10
            text: "Error message"
            color: "white"
            font.pixelSize: 12
            font.bold: true
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            width: parent.width - 20
        }

        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }

        function showError(message) {
            errorText.text = message;
            visible = true;
            opacity = 1;

            // Auto-hide after 5 seconds
            errorHideTimer.restart();
        }

        function hideError() {
            opacity = 0;
            visible = false;
        }

        Timer {
            id: errorHideTimer
            interval: 5000
            onTriggered: errorAlert.hideError()
        }

        // Close button
        Button {
            anchors {
                top: parent.top
                right: parent.right
                margins: 5
            }
            width: 20
            height: 20
            text: "✕"
            font.pixelSize: 10
            flat: true

            background: Rectangle {
                color: parent.down ? "#cc0000" : "transparent"
                radius: 10
            }

            onClicked: errorAlert.hideError()
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // navigation panel.
        Rectangle {
            id: navigationPanel
            color: "#f5f5f5"
            SplitView.preferredWidth: 300
            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 400

            FL.FallingList {
                id: fallingList
                anchors.fill: parent
                anchors.margins: 10

                onCitySelected: function(leafData, leafPath) {
                    currentCity = leafData.name;
                    currentRegion = leafData.adminName;

                    weatherForecastController.fetchDataAsync({
                        "resource": "forecast",
                        "latitude": leafData.latitude,
                        "longitude": leafData.longitude
                    });
                }
            }
        }

        // forecast panel.
        Rectangle {
            id: forecastPanel
            color: "#fafafa"
            SplitView.fillWidth: true

            WP.WeatherMain {
                id: weatherMain
                anchors.fill: parent
                anchors.margins: 10
            }
        }
    }

    // Error handlers
    Connections {
        target: geoNamesController
        function onErrorOccurred(errorMessage) {
            errorAlert.showError("GeoNames: " + errorMessage);
        }
    }
}
