import QtQuick 6.5

Rectangle {
    id: root
    width: parent ? parent.width : 200
    height: 50
    color: "#f0f0f0"
    border.color: "#ccc"

    property string title: ""

    signal clicked()

    Text {
        anchors.centerIn: parent
        text: title + " 📁"
        font.pixelSize: 16
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
