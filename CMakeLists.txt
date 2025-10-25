cmake_minimum_required(VERSION 3.16)

project(weather-app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    backend/utils/dbus-service/dbus-service.h backend/utils/dbus-service/dbus-service.cpp
    backend/utils/file-service/file-service.h backend/utils/file-service/file-service.cpp

    backend/api-controllers/weather-api-controller/weather-api-controller.h backend/api-controllers/weather-api-controller/weather-api-controller.cpp
    backend/api-controllers/geo-names-api-controller/geo-names-api-controller.h backend/api-controllers/geo-names-api-controller/geo-names-api-controller.cpp

    backend/managers/geo-names-manager/geo-names-manager.h backend/managers/geo-names-manager/geo-names-manager.cpp
    backend/managers/weather-forecast-manager/weather-forecast-manager.h backend/managers/weather-forecast-manager/weather-forecast-manager.cpp

    backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h backend/view-models/weather-forecast-view-model/weather-forecast-view-model.cpp

    frontend/main.qml
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core Network)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        main.cpp
        frontend/weather-part/CurrentWeather.qml
        frontend/weather-part/WeatherDetail.qml
        frontend/weather-part/HourForecastItem.qml
        frontend/weather-part/CompactDetail.qml
        frontend/weather-part/CompactWeather.qml
        frontend/falling-list/FallingList.qml
        frontend/falling-list/FallingListDirItem.qml
        frontend/falling-list/FallingListItem.qml
        frontend/falling-list/falling-list-logic.js
        frontend/SearchDialog.qml
        frontend/weather-part/WeatherMain.qml

        backend/data-structs/geo-names/fetch-result/fetch-result.h backend/data-structs/geo-names/fetch-result/fetch-result.cpp
        backend/data-structs/geo-names/request-data/request-data.h backend/data-structs/geo-names/request-data/request-data.cpp
        backend/data-structs/weather-forecast/request-data/request-data.h backend/data-structs/weather-forecast/request-data/request-data.cpp
        backend/data-structs/weather-forecast/weather-data/weather-data.h backend/data-structs/weather-forecast/weather-data/weather-data.cpp
        backend/data-structs/weather-forecast/fetch-result/fetch-result.h backend/data-structs/weather-forecast/fetch-result/fetch-result.cpp
        backend/data-structs/geo-names/geo-parsing-data/geo-parsing-data.h backend/data-structs/geo-names/geo-parsing-data/geo-parsing-data.cpp
        backend/data-structs/geo-names/feature-class/feature-class.h backend/data-structs/geo-names/feature-class/feature-class.cpp
        backend/data-structs/weather-forecast/weather-sub-structs/wind-data/wind-data.h backend/data-structs/weather-forecast/weather-sub-structs/wind-data/wind-data.cpp
        backend/data-structs/weather-forecast/weather-sub-structs/temperature-data/temperature-data.h backend/data-structs/weather-forecast/weather-sub-structs/temperature-data/temperature-data.cpp
        backend/data-structs/weather-forecast/weather-sub-structs/pressure-data/pressure-data.h backend/data-structs/weather-forecast/weather-sub-structs/pressure-data/pressure-data.cpp
        backend/data-structs/weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.h backend/data-structs/weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.cpp

        backend/data-storage/weather-forecast-data-storage/weather-forecast-data-storage.h backend/data-storage/weather-forecast-data-storage/weather-forecast-data-storage.cpp
        backend/data-storage/geo-names-data-storage/geo-names-data-storage.h backend/data-storage/geo-names-data-storage/geo-names-data-storage.cpp

        backend/view-models/geo-names-view-model/geo-names-view-model.h backend/view-models/geo-names-view-model/geo-names-view-model.cpp
        backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h backend/view-models/weather-forecast-view-model/weather-forecast-view-model.cpp

        backend/utils/task-manager/task-manager.h backend/utils/task-manager/task-manager.cpp

        backend/utils/json-serializable/json-serializable.h backend/utils/json-serializable/json-serializable.cpp
    )


target_link_libraries(weather-app PRIVATE Qt6::Core)
target_link_libraries(weather-app PRIVATE Qt6::Core)
target_link_libraries(weather-app PRIVATE Qt6::Core)
target_link_libraries(weather-app PRIVATE Qt6::Core)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
    )
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        main.cpp
    )

    target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt5::Core Qt5::Widgets Qt5::Network
    )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_precompile_headers(${PROJECT_NAME} PRIVATE <QWidget> <QString> <QDebug>)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
