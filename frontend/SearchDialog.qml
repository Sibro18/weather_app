import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5

Popup {
    id: searchDialog
    width: 400
    height: 500
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    signal searchRequested(var requestData)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Search Settlements"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        ColumnLayout {
            Layout.fillWidth: true

            Label {
                text: "Locality name"
                font.bold: true
            }

            TextField {
                id: nameField
                placeholderText: "Input locality name..."
                Layout.fillWidth: true
                onAccepted: searchButton.clicked()
            }
        }

        ColumnLayout {
            Layout.fillWidth: true

            Label {
                text: "Country code"
                font.bold: true
            }

            TextField {
                id: countryCodeField
                placeholderText: "Input country code..."
                Layout.fillWidth: true
                onAccepted: searchButton.clicked()
            }
        }

        CheckBox {
            id: startsWithCheckbox
            text: "Search by the beginning of the name"
            checked: true
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            Button {
                text: "Cancel"
                onClicked: searchDialog.close()
            }

            Button {
                id: searchButton
                text: "Search"
                highlighted: true
                onClicked: {
                    const requestData = {
                        name: nameField.text,
                        countryCode: countryCodeField.text,
                        startWith: startsWithCheckbox.checked
                    };

                    searchDialog.searchRequested(requestData);
                    searchDialog.close();
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#eeeeee"
        }

        Text {
            text: "💡 Leave the name field empty to get first 100 localities in the country"
            font.pixelSize: 12
            color: "#666666"
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
    }

    onOpened: {
        nameField.forceActiveFocus();
    }
}
