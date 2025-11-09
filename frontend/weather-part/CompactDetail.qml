import QtQuick 6.5

Row {
    property string icon: ""
    property string value: ""
    property bool highlighted: false

    spacing: 4
    layoutDirection: Qt.LeftToRight

    Text {
        text: icon
        font.pixelSize: 12
        color: highlighted ? "#e53935" : "#666666"
    }

    Text {
        text: value
        font.pixelSize: 12
        font.weight: highlighted ? Font.Bold : Font.Medium
        color: highlighted ? "#e53935" : "#333333"
    }
}
