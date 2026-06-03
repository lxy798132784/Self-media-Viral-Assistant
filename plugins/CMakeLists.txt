# CMakeLists.txt for All Plugins
# 所有插件的 CMake 配置文件

cmake_minimum_required(VERSION 3.20)

project(MediaAssistantPlugins VERSION 1.0.0 LANGUAGES CXX)

# Data Source Plugins
# 数据源插件
add_subdirectory(datasources)

# AI Provider Plugins
# AI 提供商插件
add_subdirectory(ai_providers)

# Install plugin directories
# 安装插件目录
install(DIRECTORY 
    "${CMAKE_BINARY_DIR}/plugins/datasources/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/media-assistant/plugins/datasources"
    FILES_MATCHING PATTERN "*.so" PATTERN "*.dll" PATTERN "*.dylib"
)

install(DIRECTORY 
    "${CMAKE_BINARY_DIR}/plugins/ai_providers/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/media-assistant/plugins/ai_providers"
    FILES_MATCHING PATTERN "*.so" PATTERN "*.dll" PATTERN "*.dylib"
)
