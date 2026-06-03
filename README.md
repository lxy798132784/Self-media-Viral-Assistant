# 自媒体爆款助手 / Media Viral Content Assistant

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-green.svg)](https://isocpp.org/)
[![Qt6](https://img.shields.io/badge/Qt-6.x-blue.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/platform-Linux%7CWindows%7CmacOS-lightgrey.svg)]()

## 项目简介 / Project Overview

**自媒体爆款助手** 是一款跨平台的自媒体内容分析与选题推荐工具，采用 C++20、Qt6/QML、CTK Plugin Framework 和 PostgreSQL 构建的企业级应用。

**Media Viral Content Assistant** is a cross-platform media content analysis and topic recommendation tool, built with C++20, Qt6/QML, CTK Plugin Framework, and PostgreSQL as an enterprise-grade application.

### 核心功能 / Core Features

- 🔥 **爆款发现** - 自动从多个自媒体平台发现高阅读/高播放/高互动的爆款内容
- 📊 **数据指标** - 获取内容的阅读量/播放量、点赞、评论、转发、收藏等详细数据
- 📝 **内容获取** - 获取爆款内容的正文、视频描述、评论等详细信息
- 🤖 **AI 拆解分析** - 用大语言模型对爆款内容从多维度拆解分析
- 💡 **智能推荐** - 基于数据分析和 AI 拆解，自动生成选题建议和写作 SOP

### 支持平台 / Supported Platforms

| 平台系 | 具体平台 |
|--------|----------|
| 阿里系 | 微信公众号、UC 大鱼号、阿里文学 |
| 字节系 | 抖音、今日头条、西瓜视频、火山小视频 |
| PDD 系 | 拼多多、多多视频 |
| 快手系 | 快手、快手极速版 |
| 腾讯系 | 微信公众号、视频号、QQ 看点、腾讯新闻 |
| 微博系 | 新浪微博、微博视频号 |
| B 站系 | 哔哩哔哩 |

### 支持的数据源 / Supported Data Sources

- **极致了数据 API** - 精确采集，设备集群真实访问
- **新榜 API** - 权威新媒体数据平台
- **西瓜数据 API** - 微信生态数据分析
- **清博智能 API** - 全网舆情监测
- **WeWe RSS** - 微信读书接口
- **微信公众号官方 API** - 官方数据接口

## 技术架构 / Technical Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      QML Frontend                           │
│  Dashboard │ Content Library │ Analysis │ Suggestions      │
├─────────────────────────────────────────────────────────────┤
│                     QmlBridge (C++/QML)                     │
├─────────────────────────────────────────────────────────────┤
│  Plugin Framework │ Event Bus │ Thread Pool │ Config Mgr   │
├─────────────────────────────────────────────────────────────┤
│     API Layer (Data Sources)    │    AI Layer (LLM/MCP)    │
│  Jizhile │ Xinbang │ Xigua │ ... │ OpenAI │ Claude │ Qwen │
├─────────────────────────────────────────────────────────────┤
│                   PostgreSQL Database                       │
└─────────────────────────────────────────────────────────────┘
```

## 快速开始 / Quick Start

### 环境要求 / Requirements

- **Compiler**: GCC 11+ / Clang 13+ / MSVC 2022+
- **CMake**: 3.20+
- **Qt**: 6.5+
- **PostgreSQL**: 14+
- **CTK Plugin Framework** (optional)

### 编译安装 / Build & Install

```bash
# Clone repository
git clone https://github.com/lxy798132784/Self-media-Viral-Assistant.git
cd Self-media-Viral-Assistant

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)

# Install
sudo cmake --install .
```

### Windows 编译 / Build on Windows

```powershell
# Using PowerShell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## 配置说明 / Configuration

### 数据库配置 / Database Configuration

编辑 `config/database.conf`:
```ini
host=localhost
port=5432
database=media_viral_db
user=postgres
password=your_password
```

### API 密钥配置 / API Keys Configuration

编辑 `config/api_keys.conf`:
```ini
jizhile_token=your_jizhile_token
xinbang_key=your_xinbang_key
xigua_key=your_xigua_key
qingbo_appid=your_qingbo_appid
qingbo_secret=your_qingbo_secret
```

### AI 提供商配置 / AI Providers Configuration

编辑 `config/ai_providers.conf`:
```ini
active_provider=openai
openai_key=sk-your-openai-key
claude_key=your-claude-key
qwen_key=your-qwen-key
ernie_key=your-ernie-key
```

## 开发文档 / Development Documentation

详细开发文档请参阅 [docs/](docs/) 目录:

- [开发指南 (中文)](docs/zh/DEVELOPMENT.md)
- [Development Guide (English)](docs/en/DEVELOPMENT.md)
- [API 文档](docs/API.md)
- [插件开发指南](docs/PLUGIN_DEV.md)

## 使用示例 / Usage Examples

### 获取爆款内容 / Fetch Viral Content

```qml
// QML example
qmlBridge.fetchViralContent("wechat", 50)
```

### 分析文章 / Analyze Article

```cpp
// C++ example
auto result = co_await qmlBridge->analyzeArticle("https://mp.weixin.qq.com/s/xxx");
if (result.hasValue()) {
    qDebug() << "Analysis score:" << result->score;
}
```

### 切换 AI 提供商 / Switch AI Provider

```qml
qmlBridge.switchAiProvider("claude")
```

## 项目结构 / Project Structure

```
Self-media-Viral-Assistant/
├── include/                 # Header files
│   ├── core/               # Core components
│   ├── api/                # Data source APIs
│   ├── ai/                 # AI providers
│   ├── database/           # Database layer
│   ├── ui/                 # UI components
│   └── utils/              # Utilities
├── src/                     # Source files
├── plugins/                 # Plugin modules
├── resources/               # Resources (QML, icons, translations)
├── config/                  # Configuration templates
├── docs/                    # Documentation
├── tests/                   # Unit tests
├── examples/                # Code examples
├── CMakeLists.txt          # Build configuration
└── README.md               # This file
```

## 许可证 / License

MIT License - See [LICENSE](LICENSE) for details.

## 贡献 / Contributing

欢迎贡献代码！请遵循以下步骤：

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 联系方式 / Contact

- **GitHub**: https://github.com/lxy798132784/Self-media-Viral-Assistant
- **Issues**: https://github.com/lxy798132784/Self-media-Viral-Assistant/issues

---

© 2024 Media Viral Assistant Team. All rights reserved.
