# Self-media Viral Content Assistant

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)]()
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)]()
[![Qt](https://img.shields.io/badge/Qt-6.5+-green.svg)]()

## Overview

A professional enterprise-grade tool for discovering, analyzing, and replicating viral content across multiple self-media platforms. Built with C++20, Qt6/QML, and plugin architecture.

## Features

### Core Capabilities
- **Multi-platform Discovery**: Automatically discover viral content from WeChat, Douyin, Bilibili, Weibo, Kuaishou, and more
- **Data Metrics Collection**: Fetch views, likes, comments, shares, favorites in real-time
- **AI-Powered Analysis**: LLM-based multi-dimensional content breakdown
- **Smart Suggestions**: Generate topic recommendations and writing SOPs
- **Plugin Architecture**: Extensible data sources and AI providers

### Supported Data Sources
| Provider | Type | Authentication |
|----------|------|----------------|
| Jizhile | Precise采集 | Bearer Token |
| XinBang | Estimated | API Key (Header) |
| Xigua Data | Monitored | API Key |
| Qingbo | Monitored | MD5 Signature |
| WeWe RSS | RSS Feed | None |
| WeChat Official | Official API | OAuth 2.0 |

### AI Providers
- OpenAI (GPT-4, GPT-3.5-turbo)
- Claude (Anthropic)
- Alibaba Qwen
- Baidu ERNIE Bot

## Quick Start

### Prerequisites
- CMake 3.20+
- Qt 6.5+
- C++20 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)
- PostgreSQL 14+ (optional)

### Build Instructions

```bash
# Clone repository
git clone https://github.com/lxy798132784/Self-media-Viral-Assistant.git
cd Self-media-Viral-Assistant

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --parallel

# Install (optional)
cmake --install . --prefix /opt/media-assistant
```

### Configuration

Create `config.json` in the application directory:

```json
{
    "data_sources": {
        "jizhile": { "token": "your-token-here" },
        "xinbang": { "api_key": "your-key-here" }
    },
    "ai_providers": {
        "openai": { "api_key": "sk-...", "model": "gpt-4" }
    },
    "database": {
        "host": "localhost",
        "port": 5432,
        "name": "media_assistant",
        "user": "postgres",
        "password": "your-password"
    }
}
```

## Project Structure

```
Self-media-Viral-Assistant/
├── include/              # Header files
│   ├── core/            # Core modules
│   ├── api/             # API interfaces
│   ├── ai/              # AI modules
│   ├── database/        # Database layer
│   └── ui/              # UI bridge
├── src/                 # Source files
├── plugins/             # Plugin modules
│   ├── datasources/     # Data source plugins
│   └── ai_providers/    # AI provider plugins
├── resources/           # QML and assets
│   └── qml/            # QML UI files
├── docs/               # Documentation
└── tests/              # Unit tests
```

## Usage Examples

### Fetching Viral Content

```cpp
auto* provider = pluginManager->getDataSource("Jizhile");
auto result = co_await provider->fetchViralContent("wechat", 50, 10000);

if (result) {
    for (const auto& item : result.value()) {
        qDebug() << item.title << item.views;
    }
}
```

### AI Content Analysis

```cpp
auto* aiProvider = pluginManager->getAiProvider("OpenAI");
auto analysis = co_await aiProvider->analyzeContent(
    contentText, 
    metrics,
    {"structure", "emotion", "engagement"}
);
```

## Documentation

- [Development Guide](docs/en/DEVELOPMENT.md)
- [API Reference](docs/en/API.md)
- [User Manual](docs/en/USER_GUIDE.md)

## License

MIT License - see [LICENSE](LICENSE) for details.

## Contributing

Contributions welcome! Please read our contributing guidelines first.
