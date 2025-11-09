import QtQuick 6.5

Row {
    property string icon: ""
    property string label: ""
    property string value: ""
    property bool highlighted: false
    property real itemWidth: 200

    width: itemWidth
    spacing: 8

    Text {
        text: icon
        font.pixelSize: 20
        width: 24
    }

    Column {
        width: parent.width - 32
        spacing: 2

        Text {
            text: label
            font.pixelSize: 14
            color: highlighted ? "#e53935" : "#78909c"
            font.weight: Font.Medium
            width: parent.width
            elide: Text.ElideRight
        }

        Text {
            text: value
            font.pixelSize: 16
            font.bold: true
            color: highlighted ? "#e53935" : "#37474f"
            width: parent.width
            elide: Text.ElideRight
        }
    }
}
