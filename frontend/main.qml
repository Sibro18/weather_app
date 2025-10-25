import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts
import "falling-list" as FL
import "weather-part" as WP

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "Погодный клиент"

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // 🅰️ Левая панель — навигация
        Rectangle {
            id: leftPanel
            color: "#f5f5f5"
            SplitView.preferredWidth: 300
            SplitView.minimumWidth: 200
            SplitView.maximumWidth: 400

            FL.FallingList {
                id: fallingList
                anchors.fill: parent
                anchors.margins: 10

                onCitySelected: function(leafData, leafPath) {
                    console.log("Выбран город:", leafData.name);
                    console.log("region: ", leafData.adminName);
                    console.log("lat: ", leafData.latitude);
                    console.log("lon: ", leafData.longitude);

                    weatherForecastController.fetchDataAsync({
                        "resource": "forecast",
                        "latitude": leafData.latitude,
                        "longitude": leafData.longitude
                    });
                }
            }
        }

        // 🅱️ Правая панель — прогноз
        Rectangle {
            id: rightPanel
            color: "#fafafa"
            SplitView.fillWidth: true

            WP.WeatherMain {
                id: weatherMain
                anchors.fill: parent
                anchors.margins: 10
            }
        }
    }

    property int selectedDay: 0
    property int counter: 0

    property var mockCurrentWeather: ({
        shortDescription: "",
        longDescription: "",
        clouds: 0, // int
        humidity: 0, // int
        visibility: 0, // int
        date: 0, // unix date
        rain: ({
            quantity: 0.0, // double,
            metric: "" // string
        }),
        snow: ({
            quantity: 0.0, // double,
            metric: "" // string
        }),
        windData: ({
            speed: 0.0, // double
            degrees: 0.0, // double
            gust: 0.0 // double
        }),
        temperatureData: ({
            temperature: 0.0, // double
            feelsLike: 0.0, // double
            min: 0.0, // double
            max: 0.0 // double
        }),
        pressureData: ({
            groundLevel: 0.0, // double
            seaLevel: 0.0 // double
        })
    });

    property var currentWeather: ({
        temperature: "22",
        description: "Солнечно",
        feelsLike: "24",
        humidity: "45",
        wind: "3",
        pressure: "760",
        visibility: "10",
        uvIndex: "5"
    })

    ListModel {
        id: threeHourForecastModel
        ListElement {
            time: "09:00-12:00";
            temperature: "20";
            description: "Солнечно";
            humidity: "40";
            wind: "2";
            feelsLike: "22";
            pressure: "758";
            visibility: "12";
            uvIndex: "4";
        }
        ListElement {
            time: "12:00-15:00";
            temperature: "24";
            description: "Облачно";
            humidity: "50";
            wind: "3";
            feelsLike: "25";
            pressure: "759";
            visibility: "10";
            uvIndex: "6";
        }
        ListElement {
            time: "15:00-18:00";
            temperature: "23";
            description: "Небольшой дождь";
            humidity: "65";
            wind: "4";
            feelsLike: "24";
            pressure: "761";
            visibility: "8";
            uvIndex: "3";
        }
        ListElement {
            time: "18:00-21:00";
            temperature: "21";
            description: "Дождь";
            humidity: "75";
            wind: "5";
            feelsLike: "22";
            pressure: "763";
            visibility: "5";
            uvIndex: "1";
        }
    }
}
