import QtQuick 6.5

Rectangle {
    id: root
    width: parent ? parent.width : 200
    height: 50
    color: "#f0f0f0"
    border.color: "#ccc"

    property string title: ""
    property string smallTitle: ""
    property var value: ({})

    signal clicked()

    Text {
        anchors.centerIn: parent
        text: title + " 🏙️"
        font.pixelSize: 16
    }

    Text {
        anchors.margins: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: smallTitle
        font.pixelSize: 10
        color: "#666666"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}

