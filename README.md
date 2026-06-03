# Media Viral Content Assistant

## 自媒体爆款助手

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-green.svg)](https://en.cppreference.com/w/cpp/20)
[![Qt6](https://img.shields.io/badge/Qt-6.x-blue.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Linux%7CWindows%7CmacOS-lightgrey.svg)]()

---

## Overview / 项目概述

**English:**  
Media Viral Content Assistant is a cross-platform desktop application that helps content creators and media practitioners discover, analyze, and learn from viral content across multiple social media platforms. Built with C++20, Qt6, QML, and powered by an extensible plugin architecture.

**中文：**  
自媒体爆款助手是一款跨平台桌面应用，帮助内容创作者和媒体从业者发现、分析和学习多个社交媒体平台的爆款内容。采用 C++20、Qt6、QML 构建，支持可扩展的插件架构。

---

## Features / 核心功能

### 🎯 Viral Content Discovery / 爆款内容发现
- **EN:** Automatically discover high-engagement content from multiple platforms  
- **CN:** 自动从多个平台发现高互动内容

### 📊 Data Metrics Analysis / 数据指标分析
- **EN:** Fetch detailed metrics including views, likes, comments, shares, and saves  
- **CN:** 获取阅读量、点赞、评论、转发、收藏等详细数据

### 🤖 AI-Powered Analysis / AI 智能分析
- **EN:** Deconstruct viral content using LLMs from multiple dimensions  
- **CN:** 使用大语言模型多维度拆解分析爆款内容

### 💡 Topic Recommendations / 选题推荐
- **EN:** Generate topic suggestions based on data analysis and AI insights  
- **CN:** 基于数据分析和 AI 拆解生成选题建议

### 📝 SOP Generation / SOP 生成
- **EN:** Automatically create writing Standard Operating Procedures  
- **CN:** 自动生成写作标准流程

---

## Supported Platforms / 支持平台

| Platform Group / 平台系 | Platforms / 平台 |
|------------------------|------------------|
| **Alibaba / 阿里系** | Taobao, Tmall, UC Browser |
| **ByteDance / 字节系** | Douyin, Toutiao, Xigua Video |
| **Tencent / 腾讯系** | WeChat Official Accounts, Channels, QQ |
| **Kuaishou / 快手系** | Kuaishou |
| **Weibo / 微博系** | Sina Weibo |
| **Bilibili / B 站系** | Bilibili |
| **PDD / 拼多多系** | Pinduoduo |

---

## Technology Stack / 技术栈

### Core Technologies / 核心技术
- **C++20**: Modern C++ with concepts, coroutines, std::expected
- **Qt 6.x**: Cross-platform application framework
- **QML**: Declarative UI development
- **CTK Plugin Framework**: Dynamic plugin system
- **PostgreSQL**: Relational database storage

### Build System / 构建系统
- **CMake 3.24+**: Cross-platform build configuration
- **Ninja**: Fast build tool

### Data Sources / 数据源
- 极致了数据 API (Jizhile Data)
- 新榜 API (Xinbang)
- 西瓜数据 API (Xigua Data)
- 清博智能 API (Qingbo Intelligence)
- WeWe RSS
- WeChat Official API

---

## Architecture / 架构设计

```
┌─────────────────────────────────────────┐
│           QML Frontend (UI)             │
│  Dashboard | Library | Analysis | Topics│
└─────────────────────────────────────────┘
                    │
                    │ Qt Quick Controls 2
                    ▼
┌─────────────────────────────────────────┐
│         C++ Backend Core                │
│  ┌──────────┬──────────┬──────────┐    │
│  │ Plugin   │   AI     │ Database │    │
│  │ Manager  │ Manager  │ Manager  │    │
│  └──────────┴──────────┴──────────┘    │
│                                         │
│  ┌─────────────────────────────────┐   │
│  │    CTK Plugin Framework         │   │
│  │  ┌──────┐ ┌──────┐ ┌──────┐    │   │
│  │  │Data  │ │ AI   │ │Analysis│   │   │
│  │  │Source│ │Prov. │ │Plugins │   │   │
│  └──└──────┘ └──────┘ └──────┘    │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────┐
│      External APIs & Services           │
└─────────────────────────────────────────┘
```

---

## Quick Start / 快速开始

### Prerequisites / 前置要求

#### Ubuntu/Debian Linux
```bash
# Install Qt 6
sudo apt install qt6-base-dev qt6-declarative-dev qt6-svg-dev

# Install PostgreSQL
sudo apt install postgresql libpq-dev

# Install build tools
sudo apt install cmake ninja-build g++-11

# Install CTK (build from source)
git clone https://github.com/CommuniTree/CTK.git
cd CTK && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

#### Windows (PowerShell)
```powershell
# Using vcpkg
vcpkg install qt6-base qt6-declarative qt6-svg
vcpkg install postgresql
vcpkg install ctk
```

### Build / 构建

```bash
# Clone repository
git clone <repository-url>
cd MediaViralAssistant

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
ninja

# Install (optional)
sudo ninja install
```

### Run / 运行

```bash
./MediaViralAssistant
```

---

## Project Structure / 项目结构

```
MediaViralAssistant/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── docs/                   # Documentation
│   ├── en/                 # English docs
│   └── zh/                 # Chinese docs
├── core/                   # Core libraries
│   ├── base/               # Base module (plugin system)
│   ├── ai/                 # AI integration
│   ├── db/                 # Database layer
│   └── utils/              # Utilities
├── plugins/                # Plugins
│   ├── datasources/        # Data source plugins
│   ├── ai_providers/       # AI provider plugins
│   └── analyzers/          # Analyzer plugins
├── ui/                     # User interface
│   ├── qml/                # QML files
│   ├── components/         # QML components
│   └── models/             # Data models
├── src/                    # Main application
└── tests/                  # Unit tests
```

---

## Plugin Development / 插件开发

### Creating a Data Source Plugin / 创建数据源插件

```cpp
#include "core/base/plugin_interface.h"

class MyDataSource : public QObject, 
                     public MediaViralAssistant::Base::IDataSourcePlugin {
    Q_OBJECT
    Q_INTERFACES(MediaViralAssistant::Base::IDataSourcePlugin)
    Q_PLUGIN_METADATA(IID "MediaViralAssistant.IDataSourcePlugin/1.0" 
                      FILE "plugin.json")

public:
    PluginMetadata metadata() const override {
        PluginMetadata meta;
        meta.id = "my-provider";
        meta.name = "My Data Provider";
        meta.version = "1.0.0";
        return meta;
    }
    
    QStringList supportedPlatforms() const override {
        return {"wechat", "douyin"};
    }
    
    // Implement other required methods...
};
```

For detailed plugin development guide, see [docs/zh/DEVELOPMENT.md](docs/zh/DEVELOPMENT.md).

---

## Configuration / 配置

Create `config.json` in the application directory:

```json
{
  "database": {
    "host": "localhost",
    "port": 5432,
    "name": "media_viral_db",
    "user": "your_user",
    "password": "your_password"
  },
  "plugins": {
    "jizhile": {
      "enabled": true,
      "api_token": "your_token_here"
    },
    "xinbang": {
      "enabled": true,
      "api_key": "your_key_here"
    }
  },
  "ai": {
    "default_provider": "openai",
    "models": {
      "openai": "gpt-4",
      "anthropic": "claude-3"
    }
  }
}
```

---

## Documentation / 文档

- **[Development Guide / 开发指南](docs/zh/DEVELOPMENT.md)** - Comprehensive development documentation
- **API Reference / API 参考** - Available in code comments
- **User Manual / 用户手册** - Coming soon

---

## Examples / 示例

### Discovering Viral Content / 发现爆款内容

```cpp
auto& manager = PluginManager::instance();
auto* dataSource = manager.getDataSourcePlugin("jizhile-provider");

if (dataSource) {
    qint64 now = QDateTime::currentSecsSinceEpoch();
    qint64 yesterday = now - 86400;
    
    auto result = co_await dataSource->discoverViralContent(
        "wechat", 
        yesterday, 
        now, 
        50
    );
    
    if (result) {
        qDebug() << "Found" << result->size() << "viral articles";
    }
}
```

### AI Content Analysis / AI 内容分析

```cpp
auto* aiProvider = manager.getAIProviderPlugin("openai-provider");

QList<QVariantMap> messages = {
    {{"role", "system"}, {"content", "Analyze this viral article..."}},
    {{"role", "user"}, {"content", articleContent}}
};

auto analysis = co_await aiProvider->chat(messages, "gpt-4");
```

---

## Testing / 测试

```bash
cd build
ctest --output-on-failure
```

---

## Contributing / 贡献

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please ensure your code follows Google C++ Style Guide and includes bilingual comments.

---

## License / 许可证

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments / 致谢

- Qt Company for the excellent Qt framework
- CTK community for the plugin framework
- All data source providers
- Contributors and users

---

## Contact / 联系方式

- **Website**: [Your Website]
- **Email**: support@example.com
- **Issues**: [GitHub Issues](https://github.com/your-org/media-viral-assistant/issues)

---

**Version / 版本**: 1.0.0  
**Last Updated / 最后更新**: 2024
