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
            "resource": "forecast",         // forecast || weather
            "latitude": 37.61556,
            "longitude": 55.75222
        });
        currentCity = "Moscow"
        currentRegion = "Moscow"
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
                        "resource": "forecast",         // forecast || weather
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
}
