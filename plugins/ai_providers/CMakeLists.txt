# CMakeLists.txt for AI Provider Plugins
# AI 提供商插件 CMake 配置文件

cmake_minimum_required(VERSION 3.20)

# OpenAI Provider Plugin
# OpenAI 提供商插件
add_library(openai_provider MODULE
    openai_provider.h
    openai_provider.cpp
)

target_link_libraries(openai_provider PRIVATE
    Qt6::Core
    Qt6::Network
    media_assistant_core
)

set_target_properties(openai_provider PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/ai_providers"
    PREFIX ""
)

# Install rules
# 安装规则
install(TARGETS openai_provider
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/media-assistant/plugins/ai_providers"
)
