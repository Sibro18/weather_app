import QtQuick 6.5
import QtQuick.Layouts

RowLayout {
    property string icon: ""
    property string label: ""
    property string value: ""
    property bool mainValue: false
    property bool compactMode: false

    spacing: compactMode ? 6 : 10

    Text {
        text: icon
        font.pixelSize: compactMode ? 14 : 16
        Layout.preferredWidth: compactMode ? 20 : 24
    }

    Column {
        spacing: 2
        Layout.fillWidth: true

        Text {
            text: label
            font.pixelSize: compactMode ? 10 : 12
            font.weight: Font.Normal
            color: "#78909c"
        }

        Text {
            text: value
            font.pixelSize: compactMode ? 12 : 14
            font.weight: mainValue ? Font.Bold : Font.Medium
            color: mainValue ? "#d32f2f" : "#37474f"
        }
    }
}
