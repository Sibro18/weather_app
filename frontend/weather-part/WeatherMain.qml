import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: weatherMain
    property var currentHour: ({})
    property var currentDayModel: []
    property var dayList: []
    property string in_currentCity: ""
    property string in_currentRegion: ""

    function setDayList(p_dayList) {
        dayList = p_dayList
    }

    function setCurrentHour(p_currentHour) {
        currentHour = p_currentHour
    }

    function getDayOfWeek(dateString) {
        const date = new Date(dateString)
        const days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday']
        let day = date.getDay();
        day = day + 1 === 7 ? 0 : day + 1;
        return days[day];
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            id: cityText
            text: in_currentCity
            font.pixelSize: 20
            font.bold: true
            color: "#333"
            Layout.alignment: Qt.AlignHCenter

            Behavior on text {
                PropertyAnimation { duration: 300 }
            }
        }

        Text {
            id: regionText
            text: in_currentRegion
            font.pixelSize: 16
            color: "#666"
            Layout.alignment: Qt.AlignHCenter

            Behavior on text {
                PropertyAnimation { duration: 300 }
            }
        }

        Rectangle {
            id: forecastMenu
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            color: "#f9f9f9"
            border.color: "#cccccc"
            border.width: 0
            radius: 5

            Row {
                anchors.centerIn: parent
                spacing: 15

                ListView {
                    id: dayListView
                    width: childrenRect.width
                    height: 100
                    orientation: ListView.Horizontal
                    spacing: 15
                    model: dayList
                    interactive: false

                    delegate: Rectangle {
                        id: dayDelegate
                        width: 160
                        height: 100
                        radius: 5
                        color: ListView.isCurrentItem ? "#d0eaff" : "#ffffff"
                        border.width: 0

                        scale: ListView.isCurrentItem ? 1.05 : 1.0
                        Behavior on scale {
                            NumberAnimation { duration: 200 }
                        }

                        Behavior on color {
                            ColorAnimation { duration: 300 }
                        }

                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: getDayOfWeek(modelData.name)
                                font.bold: true
                                font.pixelSize: 16
                                color: "#333"
                            }
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: modelData.name
                                font.pixelSize: 14
                                color: "#666"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                dayListView.currentIndex = index
                                currentDayModel = modelData.items
                            }
                        }
                    }

                    add: Transition {
                        NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 400 }
                    }
                }
            }
        }

        Rectangle {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffffff"
            border.color: "#cccccc"
            radius: 5

            ListView {
                id: hourListView
                anchors.fill: parent
                anchors.margins: 10
                model: currentDayModel
                spacing: 5
                clip: true

                add: Transition {
                    ParallelAnimation {
                        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500 }
                        NumberAnimation { property: "y"; from: 50; to: 0; duration: 400 }
                    }
                }

                displaced: Transition {
                    NumberAnimation { properties: "x,y"; duration: 300 }
                }

                delegate: HourForecastItem {
                    width: parent.width
                    forecastData: modelData
                    opacity: 0

                    Component.onCompleted: {
                        delayAppearTimer.start();
                    }

                    Timer {
                       id: delayAppearTimer
                       interval: index * 50
                       onTriggered: parent.opacity = 1
                   }

                    Behavior on opacity {
                        NumberAnimation { duration: 400 }
                    }
                }
            }
        }
    }

    Connections {
        target: weatherForecastController
        function onWeatherFetched(fetchResult) {
            setDayList(fetchResult.fiveDaysForecast)

            if (fetchResult.fiveDaysForecast && fetchResult.fiveDaysForecast.length > 0) {
                dayListView.currentIndex = 0
                currentDayModel = fetchResult.fiveDaysForecast[0].items

                in_currentCity = currentCity;
                in_currentRegion = currentRegion;
            }
        }
    }

    Rectangle {
        id: loadingOverlay
        anchors.fill: parent
        color: "#ffffff"
        visible: dayList.length === 0

        Text {
            anchors.centerIn: parent
            text: "Loading weather data..."
            font.pixelSize: 16
            color: "#666"
        }

        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }
    }
}
