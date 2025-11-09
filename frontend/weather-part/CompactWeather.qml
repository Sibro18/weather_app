import QtQuick 6.5
import QtQuick.Layouts

Item {
    id: compactWeather
    property var weatherData: ({})

    anchors.fill: parent

    function getTime(isoDate) {
        return isoDate ? isoDate.split(" ")[1].substring(0, 5) : "--:--";
    }

    function getWeatherIcon() {
        if (!weatherData.description) return "🌤️";
        const desc = weatherData.description.toLowerCase();
        if (desc.includes("clear")) return "☀️";
        if (desc.includes("cloud")) return "⛅";
        if (desc.includes("rain")) return "🌧️";
        if (desc.includes("thunder")) return "⛈️";
        if (desc.includes("snow")) return "❄️";
        if (desc.includes("fog") || desc.includes("mist")) return "🌫️";
        return "🌤️";
    }

    Rectangle {
        anchors.fill: parent
        radius: 12
        color: "#ffffff"
        border.color: "#e0e0e0"
        border.width: 1

        layer.enabled: true

        // Weather accent bar
        Rectangle {
            width: parent.width
            height: 4
            radius: 2
            gradient: Gradient {
                GradientStop {
                    position: 0.0;
                    color: {
                        if (!weatherData.description) return "#81d4fa";
                        const desc = weatherData.description.toLowerCase();
                        if (desc.includes("rain")) return "#4a6572";
                        if (desc.includes("cloud")) return "#78909c";
                        if (desc.includes("clear")) return "#ffb74d";
                        return "#81d4fa";
                    }
                }
                GradientStop {
                    position: 1.0;
                    color: {
                        if (!weatherData.description) return "#29b6f6";
                        const desc = weatherData.description.toLowerCase();
                        if (desc.includes("rain")) return "#263238";
                        if (desc.includes("cloud")) return "#546e7a";
                        if (desc.includes("clear")) return "#ff9800";
                        return "#29b6f6";
                    }
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            // Weather icon and time
            Column {
                spacing: 4
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 60

                Text {
                    text: getWeatherIcon()
                    font.pixelSize: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: getTime(weatherData.date_iso)
                    font.pixelSize: 13
                    font.bold: true
                    color: "#333333"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            // Temperature (main focus)
            Text {
                text: (weatherData.temperatureData?.temperature || "0") + "°"
                font.pixelSize: 28
                font.bold: true
                color: "#e63946"
                Layout.alignment: Qt.AlignVCenter
            }

            // Separator
            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: 1
                color: "#f0f0f0"
                Layout.topMargin: 8
                Layout.bottomMargin: 8
            }

            // Weather details in compact grid
            GridLayout {
                columns: 2
                columnSpacing: 12
                rowSpacing: 4
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true

                CompactDetail {
                    icon: "💧"
                    value: (weatherData.humidity || "0") + "%"
                    highlighted: (weatherData.humidity || 0) > 70
                }

                CompactDetail {
                    icon: "💨"
                    value: (weatherData.windData?.speed || "0") + " m/s"
                    highlighted: (weatherData.windData?.speed || 0) > 5
                }

                CompactDetail {
                    icon: "📊"
                    value: (weatherData.pressureData?.seaLevel || "0")
                }

                CompactDetail {
                    icon: "👁️"
                    value: (weatherData.visibility || "0") + " km"
                }
            }
        }
    }
}
