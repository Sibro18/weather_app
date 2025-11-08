import QtQuick 6.5
import QtQuick.Controls
import QtQuick.Layouts
import "../" as MainDir

Item {
    id: root
    width: 300
    height: 400

    // Properties
    property var currentPath: []
    property var countryList: []
    property var currentListModel: []
    property var searchSavedData: []

    property string currentPathText: "Hub"
    property bool backButtonEnabled: false
    property string searchText: ""
    property bool searchActive: false

    // Style properties
    readonly property int preferButtonHeight: 40
    readonly property int defaultSpacing: 10
    readonly property int buttonRadius: 8
    readonly property color primaryColor: "#3f51b5"
    readonly property color primaryDarkColor: "#5e7ce2"
    readonly property color successColor: "#4caf50"
    readonly property color successDarkColor: "#66bb6a"
    readonly property color disabledColor: "#cccccc"
    readonly property color backgroundColor: "#f0f0f0"
    readonly property color textColor: "#666666"

    // Signals
    signal citySelected(var leafData, var leafPath)
    signal countrySelected(var countryName)

    // Functions
    function goDeeper(key) {
        currentPath.push(key);
        updatePathText();
        clearSearch();
        geoNamesController.fetchDataByRequestAsync({countryCode: key});
    }

    function goBack() {
        if (currentPath.length > 0) {
            currentPath.pop()
            updatePathText()
            clearSearch()
            setCurrentListData(countryList)
        }
    }

    function performSearch() {
        searchActive = searchText.length > 0

        if (!searchActive) {
            restorePreviousData()
            return
        }

        const searchLower = searchText.toLowerCase()
        const filteredData = searchSavedData.filter(item =>
            item.mainTitle.toLowerCase().startsWith(searchLower)
        )

        setCurrentListData(filteredData)
    }

    function setCurrentListData(data) {
        currentListModel = data
    }

    function updatePathText() {
        backButtonEnabled = currentPath.length > 0
        currentPathText = backButtonEnabled ?
            "Hub > " + currentPath.join(" > ") : "Hub"
    }

    function clearSearch() {
        searchText = ""
        searchField.text = ""
        searchSavedData = []
        searchActive = false;
    }

    function restorePreviousData() {
        setCurrentListData(searchSavedData.length > 0 ? searchSavedData : countryList)
    }

    function handleLocationsFetched(dataList) {
        if (dataList.errorString) {
            console.log("ERROR: ", dataList.errorString)
            return
        }

        const currentKey = currentPath[currentPath.length - 1]
        const receivedKey = dataList.key ? dataList.key.trim() : ""

        if (receivedKey && !countryList.some(item => item.key === receivedKey)) {
            countryList.push(dataList)
            if (!currentKey) {
                setCurrentListData(countryList)
            }
        } else if (receivedKey === (currentKey ? currentKey.trim() : "")) {
            searchSavedData = dataList.items
            setCurrentListData(dataList.items)
        }
    }

    // Component initialization
    Component.onCompleted: {
        countryList = geoNamesController.getCountryList()
        console.log(countryList);
        setCurrentListData(countryList)
    }

    // Main layout
    Column {
        anchors.fill: parent
        spacing: defaultSpacing

        // Navigation buttons
        RowLayout {
            width: parent.width
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
                    color: backButton.enabled ?
                        (backButton.down ? primaryDarkColor : primaryColor) :
                        disabledColor
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
                onClicked: searchDialog.open()

                background: Rectangle {
                    color: addButton.down ? successDarkColor : successColor
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

        // Search section
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
                    searchText = text
                }
                onAccepted: performSearch()
            }

            Button {
                id: searchButton
                Layout.preferredWidth: 60
                Layout.preferredHeight: 40
                text: "Найти"
                onClicked: performSearch()

                background: Rectangle {
                    color: searchButton.down ? primaryDarkColor : primaryColor
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

        // Search info display (visible only when searching)
        Rectangle {
            width: parent.width
            height: 30
            visible: searchActive
            color: "#e3f2fd"
            radius: 5

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: `Поиск: "${searchText}" (найдено: ${currentListModel.length})`
                font.pixelSize: 12
                color: "#1976d2"
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }

        // List view
        Item {
            width: parent.width
            height: parent.height - (searchActive ? 170 : 140)

            ListView {
                id: currentListView
                anchors.fill: parent
                model: currentListModel
                spacing: defaultSpacing / 2
                clip: true

                delegate: Loader {
                    width: parent.width
                    height: 50

                    sourceComponent: modelData.items ? dirDelegate : cityDelegate

                    property string key: modelData.key
                    property string mainTitle: modelData.mainTitle
                    property string secondTitle: modelData.secondTitle || ""
                    property var itemData: modelData.data || {}
                }
            }
        }

        // Path display (visible only when not searching)
        Rectangle {
            width: parent.width
            height: 30
            visible: !searchActive
            color: backgroundColor
            radius: 5

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: currentPathText
                font.pixelSize: 12
                color: textColor
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // Delegates
    Component {
        id: dirDelegate
        FallingListDirItem {
            title: mainTitle
            onClicked: root.goDeeper(key)
        }
    }

    Component {
        id: cityDelegate
        FallingListItem {
            title: mainTitle
            smallTitle: secondTitle
            onClicked: root.citySelected(itemData, currentPath)
        }
    }

    // Search dialog
    MainDir.SearchDialog {
        id: searchDialog
        onSearchRequested: function(requestData) {
            console.log("Запуск поиска:", requestData)
            geoNamesController.fetchDataByRequestAsync(requestData)
        }
    }

    // Controller connections
    Connections {
        target: geoNamesController
        function onLocationsFetched(dataList) {
            root.handleLocationsFetched(dataList)
        }
    }
}
