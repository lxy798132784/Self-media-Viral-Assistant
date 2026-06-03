# 自媒体爆款助手 - 开发文档

# Media Viral Content Assistant - Development Documentation

## 目录 / Table of Contents

1. [项目概述 / Project Overview](#1-项目概述)
2. [架构设计 / Architecture Design](#2-架构设计)
3. [技术栈 / Technology Stack](#3-技术栈)
4. [开发环境配置 / Development Environment Setup](#4-开发环境配置)
5. [插件开发指南 / Plugin Development Guide](#5-插件开发指南)
6. [代码示例 / Code Examples](#6-代码示例)
7. [API 参考 / API Reference](#7-api-参考)
8. [最佳实践 / Best Practices](#8-最佳实践)

---

## 1. 项目概述 / Project Overview

### 1.1 项目简介 / Introduction

**自媒体爆款助手** 是一款跨平台的内容分析工具，帮助自媒体从业者发现、分析和学习各平台的爆款内容。

**Media Viral Content Assistant** is a cross-platform content analysis tool that helps media practitioners discover, analyze, and learn from viral content across various platforms.

### 1.2 核心功能 / Core Features

| 功能 / Feature | 描述 / Description |
|---------------|-------------------|
| 爆款发现 / Viral Discovery | 自动从多个平台发现高互动内容 |
| 数据指标 / Data Metrics | 获取阅读量、点赞、评论等详细数据 |
| AI 拆解分析 / AI Analysis | 使用大语言模型多维度分析内容 |
| 选题推荐 / Topic Recommendation | 基于数据分析生成选题建议 |
| SOP 生成 / SOP Generation | 自动生成写作标准流程 |

### 1.3 支持平台 / Supported Platforms

- **阿里系**：淘宝、天猫、UC 等
- **字节系**：抖音、今日头条、西瓜视频
- **腾讯系**：微信公众号、视频号、QQ
- **快手系**：快手主站
- **微博系**：新浪微博
- **B 站系**：哔哩哔哩
- **PDD 系**：拼多多

---

## 2. 架构设计 / Architecture Design

### 2.1 整体架构 / Overall Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                      QML Frontend                           │
│  ┌───────────┬───────────┬───────────┬───────────┐         │
│  │ Dashboard │  Content  │ Analysis  │  Topics   │         │
│  │   Page    │  Library  │  Report   │Recommend  │         │
│  └───────────┴───────────┴───────────┴───────────┘         │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ Qt Quick Controls 2
                              │ Signal/Slot
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                   C++ Backend Core                          │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐         │
│  │   Plugin    │  │     AI      │  │  Database   │         │
│  │   Manager   │  │   Manager   │  │   Manager   │         │
│  └─────────────┘  └─────────────┘  └─────────────┘         │
│                                                              │
│  ┌─────────────────────────────────────────────────────┐    │
│  │              Plugin Framework (CTK)                 │    │
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐            │    │
│  │  │ DataSource│ │  AI Prov │ │ Analyzer │            │    │
│  │  │ Plugins  │ │  Plugins │ │ Plugins  │            │    │
│  │  └──────────┘ └──────────┘ └──────────┘            │    │
│  └─────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ REST API / SDK
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    External Services                        │
│  极致了数据 │ 新榜 │ 西瓜数据 │ 清博 │ WeWe RSS │ 微信官方 │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 目录结构 / Directory Structure

```
MediaViralAssistant/
├── CMakeLists.txt              # CMake 配置 / CMake Configuration
├── README.md                   # 项目说明 / Project Overview
├── docs/                       # 文档 / Documentation
│   ├── en/                     # 英文文档 / English Docs
│   └── zh/                     # 中文文档 / Chinese Docs
├── core/                       # 核心库 / Core Libraries
│   ├── base/                   # 基础模块 / Base Module
│   │   ├── plugin_interface.h  # 插件接口定义
│   │   ├── plugin_manager.h    # 插件管理器头文件
│   │   └── plugin_manager.cpp  # 插件管理器实现
│   ├── ai/                     # AI 模块 / AI Module
│   ├── db/                     # 数据库模块 / Database Module
│   └── utils/                  # 工具模块 / Utilities
├── plugins/                    # 插件目录 / Plugins
│   ├── datasources/            # 数据源插件
│   ├── ai_providers/           # AI 提供商插件
│   └── analyzers/              # 分析器插件
├── ui/                         # 界面 / UI
│   ├── qml/                    # QML 文件
│   ├── components/             # QML 组件
│   └── models/                 # QML 数据模型
├── src/                        # 主程序 / Main Application
└── tests/                      # 测试 / Tests
```

### 2.3 设计模式 / Design Patterns

| 模式 / Pattern | 应用场景 / Use Case |
|---------------|-------------------|
| 单例模式 / Singleton | PluginManager, ConfigManager |
| 工厂模式 / Factory | 插件创建 / Plugin Creation |
| 策略模式 / Strategy | 不同数据源切换 |
| 观察者模式 / Observer | 信号槽机制 |
| 责任链模式 / Chain of Responsibility | Fallback 机制 |
| 适配器模式 / Adapter | 统一不同 API 接口 |

---

## 3. 技术栈 / Technology Stack

### 3.1 核心技术 / Core Technologies

- **C++20**: 使用最新标准特性 (concepts, coroutines, std::expected)
- **Qt 6.x**: GUI 框架和基础库
- **QML**: 声明式 UI 开发
- **CTK**: 插件框架 (Common Tool Kit)
- **PostgreSQL**: 数据存储

### 3.2 构建系统 / Build System

- **CMake 3.24+**: 跨平台构建
- **Ninja**: 快速构建工具

### 3.3 依赖管理 / Dependency Management

```cmake
find_package(Qt6 REQUIRED COMPONENTS 
    Core Gui Qml Quick Sql Network Concurrent
)
find_package(CTK REQUIRED)
find_package(PostgreSQL REQUIRED)
```

---

## 4. 开发环境配置 / Development Environment Setup

### 4.1 系统要求 / System Requirements

| 系统 / OS | 最低版本 / Min Version | 推荐版本 / Recommended |
|----------|---------------------|----------------------|
| Windows | 10 | 11 |
| Linux | Ubuntu 20.04 | Ubuntu 22.04 LTS |
| macOS | 11.0 | 12.0+ |

### 4.2 安装依赖 / Install Dependencies

#### Ubuntu/Debian

```bash
# Qt 6 / Qt 6
sudo apt install qt6-base-dev qt6-declarative-dev qt6-svg-dev

# CTK
git clone https://github.com/CommuniTree/CTK.git
cd CTK && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
sudo make install

# PostgreSQL
sudo apt install postgresql libpq-dev

# 编译工具 / Build tools
sudo apt install cmake ninja-build g++-11
```

#### Windows (PowerShell)

```powershell
# 使用 vcpkg 安装依赖 / Install dependencies via vcpkg
vcpkg install qt6-base qt6-declarative qt6-svg
vcpkg install postgresql
vcpkg install ctk
```

### 4.3 克隆项目 / Clone Project

```bash
git clone <repository-url>
cd MediaViralAssistant
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja
```

---

## 5. 插件开发指南 / Plugin Development Guide

### 5.1 创建数据源插件 / Create Data Source Plugin

```cpp
// jizhile_provider.h
#ifndef JIZHILE_PROVIDER_H
#define JIZHILE_PROVIDER_H

#include "core/base/plugin_interface.h"
#include <QObject>
#include <QNetworkAccessManager>

/**
 * @brief 极致了数据 API 插件
 *        Jizhile Data API Plugin
 */
class JizhileProvider : public QObject, 
                        public MediaViralAssistant::Base::IDataSourcePlugin {
    Q_OBJECT
    Q_INTERFACES(MediaViralAssistant::Base::IDataSourcePlugin)
    Q_PLUGIN_METADATA(IID "MediaViralAssistant.IDataSourcePlugin/1.0" 
                      FILE "plugin.json")

public:
    explicit JizhileProvider(QObject* parent = nullptr);
    
    // IDataSourcePlugin interface implementation
    PluginMetadata metadata() const override;
    QStringList supportedPlatforms() const override;
    
    QFuture<std::expected<QList<QVariantMap>, Error>> 
    discoverViralContent(const QString& platform, 
                         qint64 startTime, 
                         qint64 endTime, 
                         int limit) override;
    
    // ... other methods
    
private:
    QNetworkAccessManager m_networkManager;
    QString m_apiToken;
};

#endif // JIZHILE_PROVIDER_H
```

### 5.2 插件元数据 / Plugin Metadata

```json
// plugin.json
{
    "Plugin-Id": "jizhile-provider",
    "Plugin-Name": "Jizhile Data Provider",
    "Plugin-Version": "1.0.0",
    "Plugin-Description": "Data provider for Jizhile API",
    "Plugin-Author": "Your Team",
    "Plugin-ContactEmail": "support@example.com"
}
```

### 5.3 实现示例 / Implementation Example

```cpp
// jizhile_provider.cpp
#include "jizhile_provider.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

JizhileProvider::JizhileProvider(QObject* parent)
    : QObject(parent) {
    // Initialize API token from config / 从配置初始化 API token
}

PluginMetadata JizhileProvider::metadata() const {
    PluginMetadata meta;
    meta.id = "jizhile-provider";
    meta.name = "Jizhile Data Provider";
    meta.version = "1.0.0";
    meta.description = "Provides access to Jizhile data API";
    meta.platforms = {"wechat", "douyin", "xiaohongshu"};
    return meta;
}

QStringList JizhileProvider::supportedPlatforms() const {
    return {"wechat", "douyin", "xiaohongshu"};
}

QFuture<std::expected<QList<QVariantMap>, Error>> 
JizhileProvider::discoverViralContent(const QString& platform,
                                       qint64 startTime,
                                       qint64 endTime,
                                       int limit) {
    return QtConcurrent::run([=]() -> std::expected<QList<QVariantMap>, Error> {
        // Build request / 构建请求
        QUrl url("https://api.jizhile.com/viral-content");
        QUrlQuery query;
        query.addQueryItem("platform", platform);
        query.addQueryItem("start_time", QString::number(startTime));
        query.addQueryItem("end_time", QString::number(endTime));
        query.addQueryItem("limit", QString::number(limit));
        url.setQuery(query);
        
        // Send request / 发送请求
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, 
                         "application/json");
        request.setRawHeader("Authorization", 
                            ("Bearer " + m_apiToken).toUtf8());
        
        auto reply = m_networkManager.get(request);
        
        // Wait for response / 等待响应
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        
        if (reply->error() != QNetworkReply::NoError) {
            return std::unexpected(Error(
                reply->error(),
                reply->errorString(),
                "jizhile-provider"
            ));
        }
        
        // Parse response / 解析响应
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        
        QList<QVariantMap> results;
        // ... parse JSON and populate results
        
        return results;
    });
}
```

---

## 6. 代码示例 / Code Examples

### 6.1 使用插件管理器 / Using Plugin Manager

```cpp
#include "core/base/plugin_manager.h"

using namespace MediaViralAssistant::Base;

// Initialize plugin manager / 初始化插件管理器
auto& pluginManager = PluginManager::instance();
pluginManager.initialize("./plugin_cache");

// Load all plugins / 加载所有插件
int loaded = pluginManager.loadPluginsFromDirectory("./plugins", true);
qDebug() << "Loaded" << loaded << "plugins";

// Get data source plugin / 获取数据源插件
auto* dataSource = pluginManager.getDataSourcePlugin("jizhile-provider");
if (dataSource) {
    // Discover viral content / 发现爆款内容
    qint64 now = QDateTime::currentSecsSinceEpoch();
    qint64 yesterday = now - 24 * 60 * 60;
    
    auto future = dataSource->discoverViralContent("wechat", yesterday, now, 50);
    
    // Use co_await in coroutine / 在协程中使用 co_await
    auto result = co_await future;
    
    if (result) {
        qDebug() << "Found" << result->size() << "viral articles";
    } else {
        qDebug() << "Error:" << result.error().message;
    }
}
```

### 6.2 QML 与 C++ 交互 / QML-C++ Interaction

```cpp
// application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QQmlApplicationEngine>

class Application : public QObject {
    Q_OBJECT
    Q_PROPERTY(int viralCount READ viralCount NOTIFY viralCountChanged)
    
public:
    explicit Application(QObject* parent = nullptr);
    
    Q_INVOKABLE void discoverViralContent(const QString& platform);
    Q_INVOKABLE QVariantMap analyzeArticle(const QString& url);
    
    int viralCount() const { return m_viralCount; }
    
signals:
    void viralCountChanged();
    void discoveryFinished(const QVariantList& articles);
    
private:
    int m_viralCount = 0;
};

#endif // APPLICATION_H
```

```qml
// main.qml
import QtQuick
import QtQuick.Controls
import MediaViralAssistant 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: qsTr("自媒体爆款助手")
    
    Column {
        anchors.fill: parent
        padding: 20
        
        Button {
            text: qsTr("发现爆款 / Discover Viral")
            onClicked: application.discoverViralContent("wechat")
        }
        
        ListView {
            id: articleList
            model: viralModel
            delegate: Rectangle {
                width: parent.width
                height: 100
                
                Text {
                    text: model.title
                }
                
                Text {
                    text: qsTr("阅读 / Reads: ") + model.readCount
                }
            }
        }
    }
    
    Connections {
        target: application
        function onDiscoveryFinished(articles) {
            viralModel.append(articles)
        }
    }
}
```

### 6.3 错误处理与 Fallback / Error Handling and Fallback

```cpp
/**
 * @brief 带 Fallback 机制的数据获取
 *        Data fetching with fallback mechanism
 */
template<typename T>
QFuture<std::expected<T, Error>> fetchWithFallback(
    const QString& url,
    const QString& platform,
    const QStringList& providerChain
) {
    return QtConcurrent::run([=]() -> std::expected<T, Error> {
        Error lastError;
        
        for (const auto& providerId : providerChain) {
            auto* provider = PluginManager::instance()
                .getDataSourcePlugin(providerId);
            
            if (!provider) {
                lastError = Error(-1, "Provider not found", providerId);
                continue;
            }
            
            auto result = co_await provider->fetchContentMetrics(url, platform);
            
            if (result) {
                return result;  // Success / 成功
            }
            
            lastError = result.error();
            qWarning() << "Provider" << providerId << "failed:" 
                      << lastError.message;
        }
        
        // All providers failed / 所有提供商失败
        return std::unexpected(lastError);
    });
}

// Usage / 使用示例
auto result = co_await fetchWithFallback<QVariantMap>(
    url, 
    "wechat",
    {"jizhile-provider", "xinbang-provider", "wewe-rss-provider"}
);
```

---

## 7. API 参考 / API Reference

### 7.1 数据源插件接口 / Data Source Plugin Interface

| 方法 / Method | 描述 / Description | 返回 / Returns |
|--------------|-------------------|---------------|
| `metadata()` | 获取插件元数据 / Get plugin metadata | `PluginMetadata` |
| `supportedPlatforms()` | 支持的平台 / Supported platforms | `QStringList` |
| `discoverViralContent()` | 发现爆款内容 / Discover viral content | `QFuture<expected>` |
| `fetchContentMetrics()` | 获取内容指标 / Fetch metrics | `QFuture<expected>` |
| `fetchFullContent()` | 获取正文 / Fetch full text | `QFuture<expected>` |
| `fetchComments()` | 获取评论 / Fetch comments | `QFuture<expected>` |
| `testConnection()` | 测试连接 / Test connection | `QFuture<bool>` |

### 7.2 AI 提供商插件接口 / AI Provider Plugin Interface

| 方法 / Method | 描述 / Description |
|--------------|-------------------|
| `chat()` | 发送聊天请求 / Send chat request |
| `chatStream()` | 流式聊天 / Streaming chat |
| `listModels()` | 列出可用模型 / List available models |
| `testConnection()` | 测试连接 / Test connection |

---

## 8. 最佳实践 / Best Practices

### 8.1 编码规范 / Coding Standards

- 遵循 Google C++ Style Guide
- 所有函数必须有中英文双语注释
- 使用 `std::expected` 处理错误
- 异步操作使用 `QFuture` 和协程

### 8.2 并发控制 / Concurrency Control

```cpp
// 使用信号量限制并发数 / Use semaphore to limit concurrency
QSemaphore semaphore(5);  // Max 5 concurrent requests

auto task = QtConcurrent::run([&semaphore, url]() {
    QSemaphoreGuard guard(semaphore);
    // Process request / 处理请求
});
```

### 8.3 内存管理 / Memory Management

- 使用智能指针 (`std::unique_ptr`, `std::shared_ptr`)
- Qt 对象使用父子关系管理
- 避免循环引用

### 8.4 日志记录 / Logging

```cpp
Q_LOGGING_CATEGORY(myCategory, "MediaViralAssistant.MyModule")

qCInfo(myCategory) << "Operation completed";
qCWarning(myCategory) << "Potential issue detected";
qCCritical(myCategory) << "Critical error occurred";
```

---

## 附录 / Appendix

### A. 常见问题 / FAQ

**Q: 如何调试插件加载问题？**
**A: 启用详细日志：**
```bash
export QT_LOGGING_RULES="MediaViralAssistant.PluginManager.debug=true"
```

**Q: 如何添加新的数据源？**
**A: 继承 `IDataSourcePlugin` 接口，实现所有虚函数，编译为共享库。**

### B. 参考资料 / References

- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [CTK Plugin Framework](https://github.com/CommuniTree/CTK)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [C++20 Features](https://en.cppreference.com/w/cpp/20)

---

*文档版本 / Document Version: 1.0.0*  
*最后更新 / Last Updated: 2024*
