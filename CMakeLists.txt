# CMakeLists.txt - Media Viral Content Assistant
# CMake 配置文件 - 自媒体爆款助手

cmake_minimum_required(VERSION 3.24)
project(MediaViralAssistant 
    VERSION 1.0.0
    DESCRIPTION "Cross-platform media viral content analysis assistant"
    LANGUAGES CXX
)

# =============================================================================
# Project Configuration / 项目配置
# =============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Build type / 构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler warnings / 编译器警告
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# =============================================================================
# Find Dependencies / 查找依赖
# =============================================================================

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Sql Network Concurrent)
find_package(CTK REQUIRED)
find_package(PostgreSQL REQUIRED)

# =============================================================================
# Include Directories / 包含目录
# =============================================================================

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/plugins
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Qml_INCLUDE_DIRS}
    ${CTK_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
)

# =============================================================================
# Core Library / 核心库
# =============================================================================

# Base module / 基础模块
add_library(core_base STATIC
    core/base/plugin_manager.cpp
    core/base/plugin_interface.h
    core/base/config_manager.cpp
    core/base/error_handler.cpp
    core/base/logger.cpp
)
target_link_libraries(core_base PRIVATE
    Qt6::Core
    Qt6::Concurrent
)

# Database module / 数据库模块
add_library(core_db STATIC
    core/db/database_manager.cpp
    core/db/models/article_model.cpp
    core/db/models/metrics_model.cpp
    core/db/models/user_model.cpp
)
target_link_libraries(core_db PRIVATE
    core_base
    Qt6::Sql
    PostgreSQL::PostgreSQL
)

# AI module / AI 模块
add_library(core_ai STATIC
    core/ai/llm_manager.cpp
    core/ai/agent_framework.cpp
    core/ai/skill_registry.cpp
    core/ai/mcp_client.cpp
    core/ai/prompt_engine.cpp
)
target_link_libraries(core_ai PRIVATE
    core_base
    Qt6::Network
    Qt6::Concurrent
)

# Utils module / 工具模块
add_library(core_utils STATIC
    core/utils/http_client.cpp
    core/utils/json_parser.cpp
    core/utils/crypto_utils.cpp
    core/utils/thread_pool.cpp
)
target_link_libraries(core_utils PRIVATE
    core_base
    Qt6::Network
    Qt6::Concurrent
)

# =============================================================================
# Plugin System / 插件系统
# =============================================================================

# Data source plugins / 数据源插件
add_library(plugin_jizhile SHARED plugins/datasources/jizhile_provider.cpp)
add_library(plugin_xinbang SHARED plugins/datasources/xinbang_provider.cpp)
add_library(plugin_xigua SHARED plugins/datasources/xigua_provider.cpp)
add_library(plugin_qingbo SHARED plugins/datasources/qingbo_provider.cpp)
add_library(plugin_wewe_rss SHARED plugins/datasources/wewe_rss_provider.cpp)
add_library(plugin_wechat_official SHARED plugins/datasources/wechat_official_provider.cpp)

foreach(plugin jizhile xinbang xigua qingbo wewe_rss wechat_official)
    target_link_libraries(plugin_${plugin} PRIVATE
        core_base
        core_utils
        Qt6::Network
    )
endforeach()

# AI provider plugins / AI 提供商插件
add_library(plugin_ai_openai SHARED plugins/ai_providers/openai_provider.cpp)
add_library(plugin_ai_anthropic SHARED plugins/ai_providers/anthropic_provider.cpp)
add_library(plugin_ai_local SHARED plugins/ai_providers/local_llm_provider.cpp)

foreach(plugin openai anthropic local)
    target_link_libraries(plugin_ai_${plugin} PRIVATE
        core_ai
        core_utils
        Qt6::Network
    )
endforeach()

# Analyzer plugins / 分析器插件
add_library(plugin_content_analyzer SHARED plugins/analyzers/content_analyzer.cpp)
add_library(plugin_trend_analyzer SHARED plugins/analyzers/trend_analyzer.cpp)
add_library(plugin_sop_generator SHARED plugins/analyzers/sop_generator.cpp)

foreach(plugin content_analyzer trend_analyzer sop_generator)
    target_link_libraries(plugin_${plugin} PRIVATE
        core_ai
        core_db
        Qt6::Concurrent
    )
endforeach()

# =============================================================================
# Main Application / 主应用程序
# =============================================================================

qt_add_executable(MediaViralAssistant
    src/main.cpp
    src/application.cpp
    src/application.h
)

target_link_libraries(MediaViralAssistant PRIVATE
    core_base
    core_db
    core_ai
    core_utils
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
    Qt6::Concurrent
    Qt6::Sql
    CTK::PluginFramework
    PostgreSQL::PostgreSQL
)

# =============================================================================
# QML Resources / QML 资源
# =============================================================================

qt_add_qml_module(MediaViralAssistant
    URI MediaViralAssistant
    VERSION 1.0
    QML_FILES
        ui/qml/main.qml
        ui/qml/pages/DashboardPage.qml
        ui/qml/pages/ContentLibraryPage.qml
        ui/qml/pages/AnalysisReportPage.qml
        ui/qml/pages/TopicRecommendationPage.qml
        ui/qml/pages/SettingsPage.qml
        ui/qml/pages/platforms/AliPlatformPage.qml
        ui/qml/pages/platforms/ByteDancePlatformPage.qml
        ui/qml/pages/platforms/PDDPlatformPage.qml
        ui/qml/pages/platforms/KuaishouPlatformPage.qml
        ui/qml/pages/platforms/TencentPlatformPage.qml
        ui/qml/pages/platforms/WeiboPlatformPage.qml
        ui/qml/pages/platforms/BilibiliPlatformPage.qml
    RESOURCES
        ui/resources.qrc
)

# =============================================================================
# Installation / 安装
# =============================================================================

install(TARGETS MediaViralAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY plugins/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins
    PATTERN "*.cpp" EXCLUDE
    PATTERN "*.h" EXCLUDE
)

# =============================================================================
# Testing / 测试
# =============================================================================

enable_testing()
add_subdirectory(tests)

# =============================================================================
# Documentation / 文档
# =============================================================================

find_package(Doxygen OPTIONAL_COMPONENTS dot)
if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    doxygen_add_docs(docs ALL ${CMAKE_SOURCE_DIR})
endif()
