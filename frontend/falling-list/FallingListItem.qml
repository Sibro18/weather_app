import QtQuick 6.5

Rectangle {
    id: root
    width: parent ? parent.width : 200
    height: 50
    radius: 8
    color: mouseArea.pressed ? "#e0e0e0" : (mouseArea.containsMouse ? "#f5f5f5" : "#f0f0f0")
    border.color: mouseArea.containsMouse ? "#4caf50" : "#ccc"
    border.width: mouseArea.containsMouse ? 2 : 1

    property string title: ""
    property string smallTitle: ""
    property var value: ({})
    property bool isSelected: false

    signal clicked()

    Rectangle {
        id: selectionIndicator
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 5
        }
        width: 4
        radius: 2
        color: "#4caf50"
        visible: isSelected
    }

    Column {
        anchors.centerIn: parent
        spacing: 2

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: title + " 🏙️"
            font.pixelSize: 16
            font.bold: true
            color: isSelected ? "#4caf50" : "#333333"
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: smallTitle
            font.pixelSize: 11
            color: isSelected ? "#4caf50" : "#666666"
            font.weight: Font.Medium
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.clicked()
        }
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on color {
        ColorAnimation { duration: 200 }
    }

    Behavior on border.color {
        ColorAnimation { duration: 200 }
    }
}
