cmake_minimum_required(VERSION 3.16)

project(weather-app VERSION 0.1 LANGUAGES CXX)

# Настройки компилятора и кэша
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

# Автоматизация Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Стандарт C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Поиск Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network Gui Qml Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network Gui Qml Quick)

# Группировка исходных файлов по логическим секциям
set(BACKEND_UTILS_SOURCES
    backend/utils/file-service/file-service.h
    backend/utils/file-service/file-service.cpp
    backend/utils/task-manager/task-manager.h
    backend/utils/task-manager/task-manager.cpp
    backend/utils/backend-config/backend-config.h
    backend/utils/backend-config/backend-config.cpp
)

set(CORE_LAYER_SOURCES
    backend/core/entities/geo-names/feature-class/feature-class.h
    backend/core/entities/geo-names/feature-class/feature-class.cpp
    backend/core/entities/geo-names/fetch-result/fetch-result.h
    backend/core/entities/geo-names/fetch-result/fetch-result.cpp
    backend/core/entities/geo-names/location-data/location-data.h
    backend/core/entities/geo-names/location-data/location-data.cpp
    backend/core/entities/geo-names/request-data/request-data.h
    backend/core/entities/geo-names/request-data/request-data.cpp

    backend/core/entities/weather-forecast/fetch-result/fetch-result.h
    backend/core/entities/weather-forecast/fetch-result/fetch-result.cpp
    backend/core/entities/weather-forecast/forecast-data/forecast-data.h
    backend/core/entities/weather-forecast/forecast-data/forecast-data.cpp
    backend/core/entities/weather-forecast/request-data/request-data.h
    backend/core/entities/weather-forecast/request-data/request-data.cpp
    backend/core/entities/weather-forecast/weather-data/weather-data.h
    backend/core/entities/weather-forecast/weather-data/weather-data.cpp

    backend/core/entities/weather-forecast/weather-sub-structs/pressure-data/pressure-data.h
    backend/core/entities/weather-forecast/weather-sub-structs/pressure-data/pressure-data.cpp
    backend/core/entities/weather-forecast/weather-sub-structs/temperature-data/temperature-data.h
    backend/core/entities/weather-forecast/weather-sub-structs/temperature-data/temperature-data.cpp
    backend/core/entities/weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.h
    backend/core/entities/weather-forecast/weather-sub-structs/weather-short-description/weather-short-description.cpp
    backend/core/entities/weather-forecast/weather-sub-structs/wind-data/wind-data.h
    backend/core/entities/weather-forecast/weather-sub-structs/wind-data/wind-data.cpp

    backend/core/entities/db-requests/pagination-params.h

    backend/core/interfaces/i-api-controller.h
    backend/core/interfaces/i-cache-service.h
    backend/core/interfaces/i-repository.h

    backend/core/base/json-serializable.h
)

set(INFRASTRUCTURE_LAYER_SOURCES
    backend/infrastructure/cache/interfaces/i-geo-names-cache-service.h

    backend/infrastructure/cache/geo-names-cache-service/geo-names-cache-service.h
    backend/infrastructure/cache/geo-names-cache-service/geo-names-cache-service.cpp

    backend/infrastructure/cache/weather-forecast-cache-service/weather-forecast-cache-service.h
    backend/infrastructure/cache/weather-forecast-cache-service/weather-forecast-cache-service.cpp

    backend/infrastructure/api/weather-api-controller/weather-api-controller.h
    backend/infrastructure/api/weather-api-controller/weather-api-controller.cpp

    backend/infrastructure/api/geo-names-api-controller/geo-names-api-controller.h
    backend/infrastructure/api/geo-names-api-controller/geo-names-api-controller.cpp

    backend/infrastructure/api/interfaces/i-locations-api-controller.h
    backend/infrastructure/api/interfaces/i-weather-api-controller.h
)

set(APPLICATION_SOURCES
    backend/application/data-providers/geo-names-data-provider/geo-names-data-provider.h
    backend/application/data-providers/geo-names-data-provider/geo-names-data-provider.cpp
    backend/application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.h
    backend/application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.cpp

    backend/application/services/geo-names/i-geo-names-service.h
    backend/application/services/geo-names/geo-names-file-service/geo-names-file-service.h
    backend/application/services/geo-names/geo-names-file-service/geo-names-file-service.cpp
)

set(VIEW_MODELS_SOURCES
    backend/view-models/geo-names-view-model/geo-names-view-model.h
    backend/view-models/geo-names-view-model/geo-names-view-model.cpp
    backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h
    backend/view-models/weather-forecast-view-model/weather-forecast-view-model.cpp
)

set(QML_SOURCES
    frontend/main.qml
    frontend/weather-part/CurrentWeather.qml
    frontend/weather-part/WeatherDetail.qml
    frontend/weather-part/HourForecastItem.qml
    frontend/weather-part/CompactDetail.qml
    frontend/weather-part/CompactWeather.qml
    frontend/weather-part/WeatherMain.qml
    frontend/falling-list/FallingList.qml
    frontend/falling-list/FallingListDirItem.qml
    frontend/falling-list/FallingListItem.qml
    frontend/falling-list/falling-list-logic.js
    frontend/SearchDialog.qml
)

# Ресурсы
set(RESOURCE_FILES
    resources.qrc
)

# Объединение всех исходных файлов
set(PROJECT_SOURCES
    ${BACKEND_UTILS_SOURCES}
    ${CORE_LAYER_SOURCES}
    ${INFRASTRUCTURE_LAYER_SOURCES}
    ${APPLICATION_SOURCES}
    ${VIEW_MODELS_SOURCES}
    ${QML_SOURCES}
    ${RESOURCE_FILES}
    main.cpp
)

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

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
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Network
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
    )
endif()

# Предкомпилированные заголовки
target_precompile_headers(${PROJECT_NAME}
    PRIVATE
    <QWidget>
    <QString>
    <QDebug>
    <QObject>
    <QList>
    <QPair>
    <QVariantMap>
    <memory>
    <optional>
)

# Установка
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финальная настройка для Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
