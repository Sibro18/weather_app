import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts
import "../" as MainDir;

Item {
    id: root
    width: 300
    height: 400

    property var currentPath: [];
    property var countryList: [];
    property var currentListModel: []

    property string currentPathText: "Hub";
    property bool backButtonEnabled: false;

    property string searchText: ""
    property bool searchActive: false
    property var searchSavedData: [];

    property int preferButtonHeight: 40;
    property int defaultSpacing: 10;
    property int buttonRadius: 8;

    signal citySelected(var leafData, var leafPath)
    signal countrySelected(var countryName);

    function goDeeper(key) {
        currentPath.push(key);
        updatePathText();

        searchText = "";

        geoNamesController.fetchDataFromFileSystem(key);
    }

    function goBack() {
        currentPath.pop();
        updatePathText();

        searchText = "";
        searchSavedData = [];

        setCurrentListData(countryList);
    }

    function performSearch() {
       searchActive = searchText.length > 0;
       if (!searchActive) {
           if (searchSavedData.length > 0) {
                setCurrentListData(searchSavedData);
            } else {
               setCurrentListData(countryList);
           }

           return;
       }

       const searchLower = searchText.toLowerCase();

       // Фильтрация данных по тексту поиска
       const filteredData = currentListModel.filter(item =>
            item.mainTitle.toLowerCase().startsWith(searchLower)
        );

       setCurrentListData(filteredData);
   }

    function setCurrentListData(data) {
        currentListModel = data;
    }

    function updatePathText() {
        backButtonEnabled = currentPath.length > 0

        currentPathText = backButtonEnabled
         ? "Hub > " + currentPath.join(" > ")
         : "Hub";
    }

    Component.onCompleted: {
        countryList = geoNamesController.getCountryList();
        setCurrentListData(countryList);
    }

    Column {
        anchors.fill: parent
        spacing: defaultSpacing

        RowLayout {
            width: parent?.width
            height: 50
            spacing: defaultSpacing

            Button {
                id: backButton
                Layout.fillWidth: true
                Layout.preferredHeight: preferButtonHeight
                text: "◀ Назад"
                enabled: backButtonEnabled
                onClicked: root.goBack()

                background: Rectangle {
                    color: backButton.enabled
                        ? (backButton.down ? "#5e7ce2" : "#3f51b5")
                        : "#cccccc"
                    radius: buttonRadius
                }

                contentItem: Text {
                    text: backButton.text
                    color: "white"
                    font: backButton.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                id: addButton
                Layout.fillWidth: true
                Layout.preferredHeight: preferButtonHeight
                text: "➕ Добавить"
                onClicked: {
                    searchDialog.open();
                }

                background: Rectangle {
                    color: addButton.down ? "#66bb6a" : "#4caf50"
                    radius: buttonRadius
                }

                contentItem: Text {
                    text: addButton.text
                    color: "white"
                    font: addButton.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        RowLayout {
                width: parent.width
                height: 40
                spacing: 5

                TextField {
                    id: searchField
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    placeholderText: "Поиск..."
                    onTextChanged: {
                        searchText = text;
                        // Для поиска по мере ввода раскомментируйте следующую строку:
                        // performSearch();
                    }
                    onAccepted: performSearch() // Поиск при нажатии Enter
                }

                Button {
                    id: searchButton
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 40
                    text: "Найти"
                    onClicked: performSearch()

                    background: Rectangle {
                        color: searchButton.down ? "#5e7ce2" : "#3f51b5"
                        radius: buttonRadius
                    }

                    contentItem: Text {
                        text: searchButton.text
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

        Item {
            width: parent?.width
            height: parent.height - 140

            ListView {
                id: currentListView
                anchors.fill: parent
                model: currentListModel
                spacing: defaultSpacing / 2
                clip: true

                delegate: Loader {
                    width: parent?.width
                    height: 50

                    sourceComponent: modelData.items
                     ? dirDelegate
                     : cityDelegate

                    property string key: modelData.key
                    property string mainTitle: modelData.mainTitle
                    property string secondTitle: modelData.secondTitle || ""
                    property var itemData: modelData.data || {}
                }
            }
        }

        Rectangle {
            width: parent?.width
            height: 30
            color: "#f0f0f0"
            radius: 5

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: root.currentPathText
                font.pixelSize: 12
                color: "#666666"
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Component {
        id: dirDelegate
        FallingListDirItem {
            title: mainTitle
            onClicked: {
                root.goDeeper(key);
                //root.countrySelected(key);
            }
        }
    }

    Component {
        id: cityDelegate
        FallingListItem {
            title: mainTitle
            smallTitle: secondTitle
            onClicked: {
                root.citySelected(itemData, currentPath);
            }
        }
    }

    MainDir.SearchDialog {
        id: searchDialog
        onSearchRequested: function(requestData) {
            console.log("Запуск поиска:", requestData);
            geoNamesController.fetchDataByRequestAsync(requestData);
        }
    }

    Connections {
        target: geoNamesController
        function onLocationsFetched(dataList) {
            if (dataList.errorString) {
                console.log("ERROR: ", dataList.errorString);

                return;
            }

            const currentKey = currentPath[currentPath.length - 1];
            const receivedKey = dataList.key?.trim();


            if (receivedKey && !countryList.some(function(item) {
                return item.key ===  receivedKey;
            })) {
                countryList.push(dataList);

                if (!currentKey) {
                    root.setCurrentListData(countryList);
                }
            } else if (dataList.key?.trim() === currentKey?.trim()) {
                searchSavedData = dataList.items;
                root.setCurrentListData(dataList.items);
            }
        }
    }
}
