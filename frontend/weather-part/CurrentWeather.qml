import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var weatherData: ({})
    property bool compactMode: false  // Новое свойство для компактного режима

    anchors.fill: parent

    function getGradientTopColor() {
        if (weatherData.description && weatherData.description.toLowerCase().includes("дождь"))
            return "#4a6572";
        if (weatherData.description && weatherData.description.toLowerCase().includes("облачно"))
            return "#78909c";
        return "#81d4fa";
    }

    function getGradientBottomColor() {
        if (weatherData.description && weatherData.description.toLowerCase().includes("дождь"))
            return "#263238";
        if (weatherData.description && weatherData.description.toLowerCase().includes("облачно"))
            return "#546e7a";
        return "#29b6f6";
    }

    function getWeatherIcon() {
        if (!weatherData.description) return "";

        const desc = weatherData.description.toLowerCase();
        if (desc.includes("солнечно")) return "☀️";
        if (desc.includes("облачно")) return "⛅";
        if (desc.includes("дождь")) return "🌧️";
        if (desc.includes("гроза")) return "⛈️";
        if (desc.includes("снег")) return "❄️";
        return "🌤️";
    }

    function getCurrentTime() {
        return new Date().toLocaleTimeString(Qt.locale(), "hh:mm");
    }

    // Размеры в зависимости от режима
    property int titleFontSize: compactMode ? 16 : 24
    property int tempFontSize: compactMode ? 32 : 48
    property int descFontSize: compactMode ? 14 : 18
    property int detailFontSize: compactMode ? 12 : 14
    property int cardRadius: compactMode ? 10 : 16
    property int margins: compactMode ? 12 : 20
    property int spacing: compactMode ? 8 : 15

    // Фон с градиентом
    Rectangle {
        anchors.fill: parent
        radius: cardRadius
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.getGradientTopColor() }
            GradientStop { position: 1.0; color: root.getGradientBottomColor() }
        }
    }

    // Карточка с прозрачностью
    Rectangle {
        id: card
        anchors.fill: parent
        anchors.margins: 2
        radius: cardRadius - 2
        color: "#ffffff"
        opacity: 0.95
        border.color: "#ffffff"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: margins
            spacing: spacing

            // Заголовок с иконкой
            RowLayout {
                spacing: 8
                Layout.alignment: Qt.AlignHCenter

                Text {
                    text: root.getWeatherIcon()
                    font.pixelSize: titleFontSize + 4
                }

                Text {
                    text: "Сейчас • " + root.getCurrentTime()
                    font.pixelSize: titleFontSize - 4
                    font.weight: Font.Medium
                    color: "#37474f"
                }
            }

            // Основная температура
            Text {
                text: (weatherData.temperature || "0") + "°"
                font.pixelSize: tempFontSize
                font.bold: true
                color: "#263238"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: compactMode ? 5 : 10

                Text {
                    anchors {
                        top: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                    text: weatherData.description || ""
                    font.pixelSize: descFontSize
                    font.weight: Font.Medium
                    color: "#546e7a"
                }
            }

            // Ощущается как
            Text {
                text: "Ощущается как " + (weatherData.feelsLike || "0") + "°"
                font.pixelSize: descFontSize - 2
                color: "#78909c"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            // Разделитель
            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#e3f2fd"
                opacity: 0.6
                Layout.topMargin: compactMode ? 6 : 12
                Layout.bottomMargin: compactMode ? 6 : 12
            }

            // Детали в две колонки
            GridLayout {
                columns: 2
                columnSpacing: compactMode ? 15 : 25
                rowSpacing: compactMode ? 8 : 12
                Layout.alignment: Qt.AlignHCenter

                WeatherDetail {
                    icon: "💧"
                    label: "Влажность"
                    value: (weatherData.humidity || "0") + "%"
                    mainValue: (weatherData.humidity || 0) > 70
                    compactMode: root.compactMode
                }

                WeatherDetail {
                    icon: "💨"
                    label: "Ветер"
                    value: (weatherData.wind || "0") + " м/с"
                    mainValue: (weatherData.wind || 0) > 5
                    compactMode: root.compactMode
                }

                WeatherDetail {
                    icon: "📊"
                    label: "Давление"
                    value: (weatherData.pressure || "0") + " мм"
                    mainValue: (weatherData.pressure || 0) < 740 || (weatherData.pressure || 0) > 780
                    compactMode: root.compactMode
                }

                WeatherDetail {
                    icon: "👁️"
                    label: "Видимость"
                    value: (weatherData.visibility || "0") + " км"
                    mainValue: (weatherData.visibility || 0) < 5
                    compactMode: root.compactMode
                }

                WeatherDetail {
                    icon: "☀️"
                    label: "UV индекс"
                    value: weatherData.uvIndex || "0"
                    mainValue: (weatherData.uvIndex || 0) > 6
                    compactMode: root.compactMode
                }

                WeatherDetail {
                    icon: "🌡️"
                    label: "Точка росы"
                    value: ((weatherData.temperature || 0) - 5) + "°"
                    mainValue: false
                    compactMode: root.compactMode
                }
            }
        }
    }

    // Тень
    Rectangle {
        anchors.fill: card
        anchors.margins: -4
        radius: card.radius + 4
        color: "#20000000"
        z: -1
    }
}
