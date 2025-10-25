import QtQuick 6.5

Row {
    property string icon: ""
    property string value: ""

    spacing: 2

    Text {
        text: icon
        font.pixelSize: 12
    }

    Text {
        text: value
        font.pixelSize: 11
        font.weight: Font.Medium
        color: "#666666"
    }
}
