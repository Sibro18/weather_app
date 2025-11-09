import QtQuick 6.5

Rectangle {
    id: root
    width: parent ? parent.width : 200
    height: 50
    radius: 8
    color: mouseArea.pressed ? "#e0e0e0" : (mouseArea.containsMouse ? "#f5f5f5" : "#f0f0f0")
    border.color: mouseArea.containsMouse ? "#3f51b5" : "#ccc"
    border.width: mouseArea.containsMouse ? 2 : 1

    property string title: ""
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
        color: "#3f51b5"
        visible: isSelected
    }

    Text {
        anchors.centerIn: parent
        text: title + " 📁"
        font.pixelSize: 16
        font.bold: true
        color: isSelected ? "#3f51b5" : "#333333"
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
