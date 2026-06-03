# Media Viral Content Assistant / 自媒体爆款助手

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6.x-green.svg)](https://www.qt.io/)
[![QML](https://img.shields.io/badge/QML-2.15-green.svg)](https://doc.qt.io/qt-6/qmlapplications.html)
[![CTK](https://img.shields.io/badge/CTK-PluginFramework-orange.svg)](https://github.com/CommThevege/ctk)
[![Platform](https://img.shields.io/badge/platform-Linux%7CWindows%7CmacOS-lightgrey.svg)]()
[![Architecture](https://img.shields.io/badge/arch-x86_64%7Carm64-lightgrey.svg)]()

## English

### Overview

Media Viral Content Assistant is an enterprise-grade cross-platform application designed to help content creators and marketers discover, analyze, and replicate viral content across multiple social media platforms. Built with C++20, Qt6, QML, CTK Plugin Framework, and PostgreSQL, it offers a modular, extensible architecture with AI-powered content analysis capabilities.

### Key Features

- **Multi-Platform Discovery**: Automatically discovers high-engagement viral content from Alibaba, ByteDance, PDD, Kuaishou, Tencent, Weibo, Bilibili, and more
- **Data Metrics Collection**: Gathers comprehensive metrics including views, plays, likes, comments, shares, and favorites
- **Content Detail Extraction**: Retrieves full article text, video descriptions, and comments
- **AI-Powered Analysis**: Uses LLMs to decompose and analyze viral content from multiple dimensions
- **Smart Recommendations**: Generates topic suggestions and writing SOPs based on data analysis and AI insights
- **Plugin Architecture**: Extensible plugin system for new data sources, AI providers, and features
- **Cross-Platform**: Supports Linux, Windows, macOS with x86_64 and ARM64 architectures

### Technology Stack

- **Backend**: C++20 with Qt6 Core
- **Frontend**: QML 2.15 with modern UI/UX design
- **Plugin Framework**: CTK (Common Toolkit) Plugin Framework
- **Database**: PostgreSQL with async driver
- **AI Integration**: Support for LLMs, Agents, Skills, MCP (Model Context Protocol)
- **Build System**: CMake 3.20+
- **Data Sources**: Extreme Data API, Xinbang API, Xigua Data, Qingbo Intelligence, WeWe RSS, WeChat Official API

### Quick Start

#### Prerequisites

- C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- Qt 6.5+
- CMake 3.20+
- PostgreSQL 14+
- CTK Plugin Framework

#### Build Instructions

```bash
# Clone the repository
git clone https://github.com/your-org/media-viral-assistant.git
cd media-viral-assistant

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --parallel $(nproc)

# Install
sudo cmake --install .
```

#### Configuration

1. Configure database connection in `config/database.conf`
2. Set up API keys for data providers in `config/api_keys.conf`
3. Configure AI providers in `config/ai_providers.conf`

### Project Structure

```
media-viral-assistant/
├── src/                    # Source code
│   ├── core/               # Core business logic
│   ├── plugins/            # Plugin implementations
│   ├── ui/                 # QML UI components
│   ├── api/                # API clients for data sources
│   ├── ai/                 # AI integration modules
│   ├── database/           # Database access layer
│   └── utils/              # Utility functions
├── include/                # Public headers
├── resources/              # QML files, icons, translations
├── docs/                   # Documentation (EN/ZH)
├── examples/               # Usage examples
├── tests/                  # Unit and integration tests
└── plugins/                # Runtime plugin directory
```

### Documentation

- [User Guide (English)](docs/en/user_guide.md)
- [Developer Guide (English)](docs/en/developer_guide.md)
- [API Reference (English)](docs/en/api_reference.md)
- [Deployment Guide (English)](docs/en/deployment.md)

### License

MIT License - see [LICENSE](LICENSE) for details

---

## 中文

### 项目概述

自媒体爆款助手是一款企业级跨平台应用程序，旨在帮助内容创作者和营销人员发现、分析和复制多个社交媒体平台上的爆款内容。本项目采用 C++20、Qt6、QML、CTK 插件框架和 PostgreSQL 构建，提供模块化、可扩展的架构和 AI 驱动的内容分析能力。

### 核心功能

- **多平台发现**：自动从阿里系、字节系、PDD 系、快手系、腾讯系、微博系、B 站系等平台发现高互动爆款内容
- **数据指标采集**：收集阅读量、播放量、点赞、评论、转发、收藏等全面指标
- **内容详情提取**：获取文章全文、视频描述、评论等详细信息
- **AI 智能分析**：使用大语言模型从多维度拆解分析爆款内容
- **智能推荐**：基于数据分析和 AI 洞察生成选题建议和写作 SOP
- **插件架构**：可扩展的插件系统，支持新增数据源、AI 提供商和功能模块
- **跨平台支持**：支持 Linux、Windows、macOS，兼容 x86_64 和 ARM64 架构

### 技术栈

- **后端**：C++20 + Qt6 Core
- **前端**：QML 2.15 + 现代化 UI/UX 设计
- **插件框架**：CTK (Common Toolkit) Plugin Framework
- **数据库**：PostgreSQL + 异步驱动
- **AI 集成**：支持大模型、Agent、Skills、MCP (Model Context Protocol)
- **构建系统**：CMake 3.20+
- **数据源**：极致了数据 API、新榜 API、西瓜数据、清博智能、WeWe RSS、微信公众号官方 API

### 快速开始

#### 前置要求

- 支持 C++20 的编译器（GCC 11+、Clang 14+、MSVC 2022+）
- Qt 6.5+
- CMake 3.20+
- PostgreSQL 14+
- CTK 插件框架

#### 构建说明

```bash
# 克隆仓库
git clone https://github.com/your-org/media-viral-assistant.git
cd media-viral-assistant

# 创建构建目录
mkdir build && cd build

# CMake 配置
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . --parallel $(nproc)

# 安装
sudo cmake --install .
```

#### 配置说明

1. 在 `config/database.conf` 中配置数据库连接
2. 在 `config/api_keys.conf` 中设置数据提供商的 API 密钥
3. 在 `config/ai_providers.conf` 中配置 AI 提供商

### 项目结构

```
media-viral-assistant/
├── src/                    # 源代码
│   ├── core/               # 核心业务逻辑
│   ├── plugins/            # 插件实现
│   ├── ui/                 # QML UI 组件
│   ├── api/                # 数据源 API 客户端
│   ├── ai/                 # AI 集成模块
│   ├── database/           # 数据库访问层
│   └── utils/              # 工具函数
├── include/                # 公共头文件
├── resources/              # QML 文件、图标、翻译
├── docs/                   # 文档（中英文）
├── examples/               # 使用示例
├── tests/                  # 单元测试和集成测试
└── plugins/                # 运行时插件目录
```

### 文档链接

- [用户指南（中文）](docs/zh/user_guide.md)
- [开发者指南（中文）](docs/zh/developer_guide.md)
- [API 参考（中文）](docs/zh/api_reference.md)
- [部署指南（中文）](docs/zh/deployment.md)

### 开源协议

MIT 许可证 - 详见 [LICENSE](LICENSE)
