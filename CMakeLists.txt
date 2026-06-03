cmake_minimum_required(VERSION 3.20)
project(MediaViralAssistant 
    VERSION 1.0.0
    DESCRIPTION "Media Viral Content Assistant - Cross-platform viral content discovery and analysis tool"
    LANGUAGES CXX
)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Google C++ Style guidelines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Qml 
    Quick 
    QuickControls2 
    Widgets
    Network
    Sql
    Concurrent
)

# Enable Qt6 automoc, autouic, autorcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find PostgreSQL
find_package(PostgreSQL REQUIRED)

# CTK Plugin Framework
option(USE_CTK_PLUGIN "Enable CTK Plugin Framework" ON)
if(USE_CTK_PLUGIN)
    find_package(CTKPluginFramework REQUIRED)
endif()

# Platform detection
if(WIN32)
    set(PLATFORM_NAME "Windows")
    add_definitions(-DPLATFORM_WINDOWS)
elseif(APPLE)
    set(PLATFORM_NAME "macOS")
    add_definitions(-DPLATFORM_MACOS)
else()
    set(PLATFORM_NAME "Linux")
    add_definitions(-DPLATFORM_LINUX)
endif()

# Architecture detection
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)")
    set(ARCH_NAME "ARM64")
    add_definitions(-DARCH_ARM64)
else()
    set(ARCH_NAME "x86_64")
    add_definitions(-DARCH_X86_64)
endif()

message(STATUS "Building for ${PLATFORM_NAME} on ${ARCH_NAME}")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${PostgreSQL_INCLUDE_DIRS}
)

# Source files
set(CORE_SOURCES
    src/core/application.cpp
    src/core/plugin_manager.cpp
    src/core/config_manager.cpp
    src/core/event_bus.cpp
    src/core/thread_pool.cpp
)

set(CORE_HEADERS
    include/core/application.h
    include/core/plugin_manager.h
    include/core/config_manager.h
    include/core/event_bus.h
    include/core/thread_pool.h
)

set(API_SOURCES
    src/api/data_source_base.cpp
    src/api/extreme_data_api.cpp
    src/api/xinbang_api.cpp
    src/api/xigua_data_api.cpp
    src/api/qingbo_api.cpp
    src/api/wewe_rss_api.cpp
    src/api/wechat_official_api.cpp
    src/api/api_fallback_chain.cpp
)

set(API_HEADERS
    include/api/data_source_base.h
    include/api/extreme_data_api.h
    include/api/xinbang_api.h
    include/api/xigua_data_api.h
    include/api/qingbo_api.h
    include/api/wewe_rss_api.h
    include/api/wechat_official_api.h
    include/api/api_fallback_chain.h
)

set(AI_SOURCES
    src/ai/ai_provider_base.cpp
    src/ai/llm_manager.cpp
    src/ai/agent_framework.cpp
    src/ai/skill_registry.cpp
    src/ai/mcp_client.cpp
    src/ai/content_analyzer.cpp
)

set(AI_HEADERS
    include/ai/ai_provider_base.h
    include/ai/llm_manager.h
    include/ai/agent_framework.h
    include/ai/skill_registry.h
    include/ai/mcp_client.h
    include/ai/content_analyzer.h
)

set(DATABASE_SOURCES
    src/database/database_connection.cpp
    src/database/database_manager.cpp
    src/database/models/article_model.cpp
    src/database/models/metrics_model.cpp
    src/database/models/user_model.cpp
)

set(DATABASE_HEADERS
    include/database/database_connection.h
    include/database/database_manager.h
    include/database/models/article_model.h
    include/database/models/metrics_model.h
    include/database/models/user_model.h
)

set(UTILS_SOURCES
    src/utils/logger.cpp
    src/utils/http_client.cpp
    src/utils/json_utils.cpp
    src/utils/crypto_utils.cpp
    src/utils/platform_utils.cpp
)

set(UTILS_HEADERS
    include/utils/logger.h
    include/utils/http_client.h
    include/utils/json_utils.h
    include/utils/crypto_utils.h
    include/utils/platform_utils.h
)

set(UI_SOURCES
    src/ui/main_window.cpp
    src/ui/qml_bridge.cpp
)

set(UI_HEADERS
    include/ui/main_window.h
    include/ui/qml_bridge.h
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${API_SOURCES}
    ${AI_SOURCES}
    ${DATABASE_SOURCES}
    ${UTILS_SOURCES}
    ${UI_SOURCES}
)

set(ALL_HEADERS
    ${CORE_HEADERS}
    ${API_HEADERS}
    ${AI_HEADERS}
    ${DATABASE_HEADERS}
    ${UTILS_HEADERS}
    ${UI_HEADERS}
)

# Resource files
qt6_add_resources(RESOURCE_FILES resources/resources.qrc)

# Main executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${ALL_SOURCES}
    ${ALL_HEADERS}
    ${RESOURCE_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
    Qt6::Concurrent
    ${PostgreSQL_LIBRARIES}
)

if(USE_CTK_PLUGIN AND TARGET CTKPluginFramework)
    target_link_libraries(${PROJECT_NAME} PRIVATE CTKPluginFramework)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_CTK_PLUGIN)
endif()

# Plugin system
add_subdirectory(plugins)

# Tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY resources/ DESTINATION share/${PROJECT_NAME}/resources)
install(DIRECTORY config/ DESTINATION etc/${PROJECT_NAME})

# Generate config files
configure_file(
    ${CMAKE_SOURCE_DIR}/config/database.conf.in
    ${CMAKE_BINARY_DIR}/config/database.conf
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/config/api_keys.conf.in
    ${CMAKE_BINARY_DIR}/config/api_keys.conf
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/config/ai_providers.conf.in
    ${CMAKE_BINARY_DIR}/config/ai_providers.conf
    @ONLY
)

# CPack configuration for packaging
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "Media Viral Assistant Team")
set(CPACK_PACKAGE_CONTACT "support@mediaviralassistant.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
else()
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
endif()

include(CPack)

# Print summary
message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Configuration Summary ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Platform: ${PLATFORM_NAME}")
message(STATUS "Architecture: ${ARCH_NAME}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "CTK Plugin: ${USE_CTK_PLUGIN}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "")
