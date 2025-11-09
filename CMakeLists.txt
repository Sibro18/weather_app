cmake_minimum_required(VERSION 3.16)
project(weather-app VERSION 0.1 LANGUAGES CXX)

# =============================================================================
# Configuration and Compiler Settings
# =============================================================================

# Use ccache if available for faster rebuilds
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

# Qt Automation
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# =============================================================================
# Qt Dependencies
# =============================================================================

set(QT_COMPONENTS Core Widgets Network Gui Qml Quick)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})

# =============================================================================
# Source Files Organization
# =============================================================================

# Backend Utilities
set(BACKEND_UTILS_SOURCES
    backend/utils/file-service/file-service.h
    backend/utils/file-service/file-service.cpp
    backend/utils/task-manager/task-manager.h
    backend/utils/task-manager/task-manager.cpp
    backend/utils/backend-config/backend-config.h
    backend/utils/backend-config/backend-config.cpp
)

# Core Domain Entities
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

# Infrastructure Layer
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

# Application Layer
set(APPLICATION_SOURCES
    backend/application/data-providers/geo-names-data-provider/geo-names-data-provider.h
    backend/application/data-providers/geo-names-data-provider/geo-names-data-provider.cpp
    backend/application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.h
    backend/application/data-providers/weather-forecast-data-provider/weather-forecast-data-provider.cpp

    backend/application/services/geo-names/i-geo-names-service.h
    backend/application/services/geo-names/geo-names-file-service/geo-names-file-service.h
    backend/application/services/geo-names/geo-names-file-service/geo-names-file-service.cpp
)

# View Models
set(VIEW_MODELS_SOURCES
    backend/view-models/geo-names-view-model/geo-names-view-model.h
    backend/view-models/geo-names-view-model/geo-names-view-model.cpp
    backend/view-models/weather-forecast-view-model/weather-forecast-view-model.h
    backend/view-models/weather-forecast-view-model/weather-forecast-view-model.cpp
)

# Resources
set(RESOURCE_FILES
    resources.qrc
)

# =============================================================================
# Executable Target
# =============================================================================

# Combine all source files
set(PROJECT_SOURCES
    ${BACKEND_UTILS_SOURCES}
    ${CORE_LAYER_SOURCES}
    ${INFRASTRUCTURE_LAYER_SOURCES}
    ${APPLICATION_SOURCES}
    ${VIEW_MODELS_SOURCES}
    ${RESOURCE_FILES}
    main.cpp
)

# Create executable with modern Qt approach
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    # Link Qt6 libraries
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
    )

    # Finalize Qt6 executable
    qt_finalize_executable(${PROJECT_NAME})
else()
    # Qt5 approach
    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )

    # Link Qt5 libraries
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
    )
endif()

# =============================================================================
# Target Properties and Optimizations
# =============================================================================

# Precompiled headers for faster compilation
target_precompile_headers(${PROJECT_NAME}
    PRIVATE
    <QWidget>
    <QString>
    <QObject>
    <QList>
    <QPair>
    <QVariantMap>
    <memory>
    <optional>
)

# Compiler-specific optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O2
    )
endif()

# Platform-specific settings
if(UNIX AND NOT APPLE)
    # Linux-specific settings
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        LINUX_BUILD
    )
endif()

# =============================================================================
# Installation
# =============================================================================

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# =============================================================================
# Optional: Testing Configuration (if you add tests later)
# =============================================================================

# option(BUILD_TESTS "Build tests" OFF)
# if(BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()

# =============================================================================
# Optional: Packaging Configuration (for creating .deb or other packages)
# =============================================================================

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
