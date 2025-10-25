import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: compactWeather
    property var weatherData: ({})

    width: 300
    height: 120

    function getTime(unixDate) {
        const date = new Date(unixDate * 1000);
        const hours = date.getHours().toString().padStart(2, '0');
        const minutes = date.getMinutes().toString().padStart(2, '0');
        const seconds = date.getSeconds().toString().padStart(2, '0');

        return `${hours}:${minutes}:${seconds}`;
    }

    Rectangle {
        anchors.fill: parent
        radius: 12
        color: "#ffffff"
        border.color: "#e0e0e0"
        border.width: 1

        // Мини-градиент вверху
        Rectangle {
            width: parent.width
            height: 4
            radius: 2
            gradient: Gradient {
                GradientStop { position: 0.0; color: getGradientTopColor() }
                GradientStop { position: 1.0; color: getGradientBottomColor() }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 15

            // Левая часть: время и иконка
            Column {
                spacing: 4
                Layout.alignment: Qt.AlignVCenter

                Text {
                    text: compactWeather.getTime(weatherData.date)
                    font.pixelSize: 14
                    font.bold: true
                    color: "#333333"
                }

                Text {
                    text: getWeatherIcon()
                    font.pixelSize: 24
                }
            }

            // Центральная часть: температура
            Text {
                text: weatherData.temperatureData.temperature + "°"
                font.pixelSize: 28
                font.bold: true
                color: "#e63946"
                Layout.alignment: Qt.AlignVCenter
            }

            // Правая часть: детали в компактном виде
            GridLayout {
                columns: 2
                columnSpacing: 8
                rowSpacing: 4
                Layout.alignment: Qt.AlignVCenter

                CompactDetail { icon: "💧"; value: weatherData.humidity + "%" }
                CompactDetail { icon: "💨"; value: weatherData.windData.speed + "м/с" }
                CompactDetail { icon: "📊"; value: weatherData.pressureData.seaLevel }
                CompactDetail { icon: "👁️"; value: weatherData.visibility }
            }
        }
    }

    function getGradientTopColor() {
        if (weatherData.description && weatherData.description.toLowerCase().includes("rain"))
            return "#4a6572";
        if (weatherData.description && weatherData.description.toLowerCase().includes("cloud"))
            return "#78909c";
        return "#81d4fa";
    }

    function getGradientBottomColor() {
        if (weatherData.description && weatherData.description.toLowerCase().includes("rain"))
            return "#263238";
        if (weatherData.description && weatherData.description.toLowerCase().includes("cloud"))
            return "#546e7a";
        return "#29b6f6";
    }

    function getWeatherIcon() {
        if (!weatherData.description) return "🌤️";

        const desc = weatherData.description.toLowerCase();
        if (desc.includes("clear")) return "☀️";
        if (desc.includes("cloud")) return "⛅";
        if (desc.includes("rain")) return "🌧️";
        if (desc.includes("thunder")) return "⛈️";
        if (desc.includes("snow")) return "❄️";
        return "🌤️";
    }
}
