import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: weatherModal
    property var weatherData: ({})

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    width: 500
    height: 600
    anchors.centerIn: Overlay.overlay

    background: Rectangle {
        radius: 20
        color: "#ffffff"
        border.color: "#e0e0e0"
        border.width: 1
    }

    function getGradientTopColor() {
        if (!weatherData.shortDescription) return "#81d4fa";
        const desc = weatherData.shortDescription.toLowerCase();
        if (desc.includes("rain")) return "#4a6572";
        if (desc.includes("cloud")) return "#78909c";
        if (desc.includes("clear")) return "#ffb74d";
        return "#81d4fa";
    }

    function getGradientBottomColor() {
        if (!weatherData.shortDescription) return "#29b6f6";
        const desc = weatherData.shortDescription.toLowerCase();
        if (desc.includes("rain")) return "#263238";
        if (desc.includes("cloud")) return "#546e7a";
        if (desc.includes("clear")) return "#ff9800";
        return "#29b6f6";
    }

    function getWeatherIcon() {
        if (!weatherData.shortDescription) return "🌤️";
        const desc = weatherData.shortDescription.toLowerCase();
        if (desc.includes("clear")) return "☀️";
        if (desc.includes("cloud")) return "⛅";
        if (desc.includes("rain")) return "🌧️";
        if (desc.includes("thunder")) return "⛈️";
        if (desc.includes("snow")) return "❄️";
        if (desc.includes("fog") || desc.includes("mist")) return "🌫️";
        return "🌤️";
    }

    function getFormattedDate() {
        if (!weatherData.date_iso) return "No date";
        const date = new Date(weatherData.date_iso);
        if (isNaN(date.getTime())) return "Invalid date";

        const days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
        const months = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];

        const dayName = days[date.getDay()];
        const monthName = months[date.getMonth()];
        const day = date.getDate();
        const hours = date.getHours().toString().padStart(2, '0');
        const minutes = date.getMinutes().toString().padStart(2, '0');

        return `${dayName}, ${monthName} ${day} • ${hours}:${minutes}`;
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: parent.width
            height: 120
            radius: 20
            gradient: Gradient {
                GradientStop { position: 0.0; color: weatherModal.getGradientTopColor() }
                GradientStop { position: 1.0; color: weatherModal.getGradientBottomColor() }
            }

            Column {
                anchors.centerIn: parent
                spacing: 8

                Text {
                    text: weatherModal.getWeatherIcon()
                    font.pixelSize: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: weatherModal.getFormattedDate()
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Button {
                anchors {
                    top: parent.top
                    right: parent.right
                    margins: 12
                }
                text: "✕"
                font.pixelSize: 18
                implicitWidth: 40
                implicitHeight: 40
                flat: true

                background: Rectangle {
                    radius: 20
                    color: parent.down ? "#40000000" : "transparent"
                }

                onClicked: weatherModal.close()
            }
        }

        Flickable {
            width: parent.width
            height: parent.height - 120
            contentWidth: width
            contentHeight: contentColumn.height
            clip: true

            Column {
                id: contentColumn
                width: parent.width
                spacing: 20
                padding: 20

                Column {
                    width: parent.width - 40
                    spacing: 4
                    anchors.horizontalCenter: parent.horizontalCenter

                    Text {
                        text: (weatherData.temperatureData?.temperature || "0") + "°"
                        font.pixelSize: 48
                        font.bold: true
                        color: "#263238"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: weatherData.longDescription || weatherData.shortDescription || "No description"
                        font.pixelSize: 18
                        font.weight: Font.Medium
                        color: "#546e7a"
                        anchors.horizontalCenter: parent.horizontalCenter
                        wrapMode: Text.WordWrap
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        text: "Feels like " + (weatherData.temperatureData?.feelsLike || "0") + "°"
                        font.pixelSize: 16
                        color: "#78909c"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                Grid {
                    columns: 2
                    columnSpacing: 20
                    rowSpacing: 16
                    width: parent.width - 40
                    anchors.horizontalCenter: parent.horizontalCenter

                    WeatherDetail {
                        icon: "💧"
                        label: "Humidity"
                        value: (weatherData.humidity || "0") + "%"
                        highlighted: (weatherData.humidity || 0) > 70
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "💨"
                        label: "Wind Speed"
                        value: (weatherData.windData?.speed || "0") + " m/s"
                        highlighted: (weatherData.windData?.speed || 0) > 5
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "🧭"
                        label: "Wind Direction"
                        value: (weatherData.windData?.degrees || "0") + "°"
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "📊"
                        label: "Pressure"
                        value: (weatherData.pressureData?.seaLevel || "0") + " hPa"
                        highlighted: (weatherData.pressureData?.seaLevel || 0) < 1000 || (weatherData.pressureData?.seaLevel || 0) > 1020
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "👁️"
                        label: "Visibility"
                        value: (weatherData.visibility || "0") + " km"
                        highlighted: (weatherData.visibility || 0) < 5
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "☁️"
                        label: "Cloudiness"
                        value: (weatherData.clouds || "0") + "%"
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "🌡️"
                        label: "Min/Max"
                        value: (weatherData.temperatureData?.min || "0") + "° / " + (weatherData.temperatureData?.max || "0") + "°"
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }

                    WeatherDetail {
                        icon: "💨"
                        label: "Wind Gust"
                        value: (weatherData.windData?.gust || "0") + " m/s"
                        itemWidth: (parent.width - parent.columnSpacing) / 2
                    }
                }
            }
        }
    }
}
