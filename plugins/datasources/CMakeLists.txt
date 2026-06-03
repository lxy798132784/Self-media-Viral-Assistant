# CMakeLists.txt for Data Source Plugins
# 数据源插件 CMake 配置文件

cmake_minimum_required(VERSION 3.20)

# Jizhile Provider Plugin
# 极致了数据提供商插件
add_library(jizhile_provider MODULE
    jizhile_provider.h
    jizhile_provider.cpp
)

target_link_libraries(jizhile_provider PRIVATE
    Qt6::Core
    Qt6::Network
    media_assistant_core
)

set_target_properties(jizhile_provider PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/datasources"
    PREFIX ""
)

# XinBang Provider Plugin
# 新榜数据提供商插件
add_library(xinbang_provider MODULE
    xinbang_provider.h
    xinbang_provider.cpp
)

target_link_libraries(xinbang_provider PRIVATE
    Qt6::Core
    Qt6::Network
    media_assistant_core
)

set_target_properties(xinbang_provider PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/datasources"
    PREFIX ""
)

# Install rules
# 安装规则
install(TARGETS jizhile_provider xinbang_provider
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/media-assistant/plugins/datasources"
)
