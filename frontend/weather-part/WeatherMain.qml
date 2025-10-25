import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: weatherMain
    property var currentHour: ({})
    property var currentDayModel: []
    property var dayList: []
    property var mockDayList: [
        {
            name: "Сегодня",
            items: [
                {
                    date: 1755702000,
                    temperatureData: { temperature: 22, min: 18, max: 25, feelsLike: 24 },
                    windData: { speed: 3, degrees: 180, gust: 5 },
                    pressureData: { seaLevel: 760, groundLevel: 758 },
                    shortDescription: "Clear",
                    humidity: 45,
                    visibility: 10,
                    clouds: 10,
                    rain: { quantity: 0, metric: "mm" },
                    snow: { quantity: 0, metric: "mm" }
                },
                {
                    date: 1755712800,
                    temperatureData: { temperature: 19, min: 16, max: 21, feelsLike: 18 },
                    windData: { speed: 12, degrees: 150, gust: 18 },
                    pressureData: { seaLevel: 755, groundLevel: 752 },
                    shortDescription: "Cloudy",
                    humidity: 65,
                    visibility: 8,
                    clouds: 70,
                    rain: { quantity: 0, metric: "mm" },
                    snow: { quantity: 0, metric: "mm" }
                },
                {
                    date: 1755723600,
                    temperatureData: { temperature: -5, min: -8, max: -2, feelsLike: -10 },
                    windData: { speed: 7, degrees: 300, gust: 12 },
                    pressureData: { seaLevel: 780, groundLevel: 778 },
                    shortDescription: "Snow",
                    humidity: 75,
                    visibility: 3,
                    clouds: 100,
                    rain: { quantity: 0, metric: "mm" },
                    snow: { quantity: 1.8, metric: "mm" }
                }
            ]
        },
        {
            name: "Завтра",
            items: [
                {
                    date: 1755734400,
                    temperatureData: { temperature: 15, min: 12, max: 17, feelsLike: 14 },
                    windData: { speed: 5, degrees: 240, gust: 8 },
                    pressureData: { seaLevel: 745, groundLevel: 742 },
                    shortDescription: "Rainy",
                    humidity: 85,
                    visibility: 6,
                    clouds: 95,
                    rain: { quantity: 2.5, metric: "mm" },
                    snow: { quantity: 0, metric: "mm" }
                }
            ]
        }
    ]

    Component.onCompleted: {
        //setDayList(mockDayList); // Инициализация моками
    }

    function setDayList(p_dayList) {
        dayList = p_dayList;
    }

    function setCurrentHour(p_currentHour) {
        currentHour = p_currentHour;
    }

    function getMedianTemperature(index) {
        const items = dayList[index].items;

        return weatherForecastController.getAvgTemperatureOffTheDay(dayList[index]?.items || []);
    }

    function getTemperatureRangeString(index) {
        const temperatureData = weatherForecastController.getAvgTemperatureRange(dayList[index]?.items || []);

        return temperatureData.min + "°C - " + temperatureData.max + "°C";
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Меню с карточками дней
        Rectangle {
            id: forecastMenu
            Layout.fillWidth: true
            Layout.preferredHeight: 120  // Увеличиваем высоту контейнера
            color: "#f9f9f9"
            border.color: "#cccccc"
            border.width: 1
            radius: 5

            Row {
                anchors.centerIn: parent
                spacing: 15  // Увеличиваем расстояние между карточками

                ListView {
                    id: dayListView
                    width: childrenRect.width
                    height: 100
                    orientation: ListView.Horizontal
                    spacing: 15  // Увеличиваем расстояние здесь тоже
                    model: dayList
                    interactive: false

                    delegate: Rectangle {
                        width: 160  // ← МЕНЯЕМ ШИРИНУ КАРТОЧКИ ЗДЕСЬ (было 100)
                        height: 100
                        radius: 5
                        color: ListView.isCurrentItem ? "#d0eaff" : "#ffffff"
                        border.color: ListView.isCurrentItem ? "#4a90e2" : "#999"
                        border.width: ListView.isCurrentItem ? 2 : 1

                        property bool hovered: false  // ← ЭТО свойство hovered

                        Column {
                            anchors.centerIn: parent
                            spacing: 8  // Увеличиваем расстояние между текстом
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.name
                                font.bold: true
                                font.pixelSize: 14  // Можно увеличить шрифт
                            }
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: hovered  // ← ИСПОЛЬЗУЕМ свойство hovered ПРИ НАВЕДЕНИИ
                                     ? weatherMain.getTemperatureRangeString(index)
                                     : weatherMain.getMedianTemperature(index) + "°C"
                                font.pixelSize: 16
                                color: "#333"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true  // ← ВКЛЮЧАЕМ ОБНАРУЖЕНИЕ НАВЕДЕНИЯ
                            onClicked: {
                                dayListView.currentIndex = index;
                                currentDayModel = modelData.items;
                            }
                            onEntered: parent.hovered = true   // ← УСТАНАВЛИВАЕМ true ПРИ НАВЕДЕНИИ
                            onExited: parent.hovered = false   // ← УСТАНАВЛИВАЕМ false ПРИ УХОДЕ
                        }
                    }
                }
            }
        }
        // Область контента
        Rectangle {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffffff"
            border.color: "#cccccc"
            radius: 5

            Loader {
                id: contentLoader
                anchors.fill: parent
                anchors.margins: 10
                sourceComponent: threeHourForecastComponent
            }
        }
    }

    Component {
        id: threeHourForecastComponent

        Column {
            width: contentArea.width - 20
            height: contentArea.height - 20
            spacing: 10

            Text {
                text: "Прогноз на день " + selectedDay
                font.pixelSize: 20
                font.bold: true
                color: "#333"
            }

            ListView {
                width: parent.width
                height: parent.height - 40
                model: currentDayModel
                spacing: 5
                clip: true
                delegate: HourForecastItem {
                    width: parent?.width || 40
                    forecastData: modelData
                }
            }
        }
    }

    Connections {
        target: weatherForecastController
        function onWeatherFetched(fetchResult) {
            setDayList(fetchResult.fiveDaysForecast);

            if (fetchResult.fiveDaysForecast && fetchResult.fiveDaysForecast.length > 0) {
                dayListView.currentIndex = 0;
                currentDayModel = fetchResult.fiveDaysForecast[0].items;
            }
        }
    }
}
