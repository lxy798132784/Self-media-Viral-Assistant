# Development Guide

## Architecture Overview

### Plugin System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Application Core                          │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │PluginManager│  │ EventBus    │  │ ConfigManager       │  │
│  └──────┬──────┘  └──────┬──────┘  └──────────┬──────────┘  │
│         │                │                     │             │
│  ┌──────▼────────────────▼─────────────────────▼──────────┐  │
│  │              IDataSourcePlugin Interface               │  │
│  └─────────────────────────┬──────────────────────────────┘  │
│                            │                                  │
│  ┌─────────────────────────▼──────────────────────────────┐  │
│  │              IAiProviderPlugin Interface               │  │
│  └────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
┌───────▼───────┐  ┌───────▼───────┐  ┌───────▼───────┐
│ JizhilePlugin │  │ XinBangPlugin │  │ OpenAiPlugin  │
│ (DataSource)  │  │ (DataSource)  │  │ (AI Provider) │
└───────────────┘  └───────────────┘  └───────────────┘
```

## Creating a Data Source Plugin

### Step 1: Define Plugin Class

```cpp
// my_provider.h
#ifndef MY_PROVIDER_H
#define MY_PROVIDER_H

#include "core/base/plugin_interface.h"
#include <QObject>

namespace MediaViralAssistant::Plugins {

class MyProvider : public QObject, public IDataSourcePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "media-assistant.datasource.myprovider" FILE "plugin.json")
    Q_INTERFACES(MediaViralAssistant::Plugins::IDataSourcePlugin)

public:
    explicit MyProvider(QObject* parent = nullptr);
    ~MyProvider() override;
    
    QString name() const override;
    
    QFuture<std::expected<QList<ContentItem>, ApiError>> 
    fetchViralContent(const QString& platform, int limit, int min_views) override;
    
    // ... other methods
};

} // namespace
#endif
```

### Step 2: Implement Methods

```cpp
// my_provider.cpp
#include "my_provider.h"
#include <QtConcurrent>

namespace MediaViralAssistant::Plugins {

MyProvider::MyProvider(QObject* parent) : QObject(parent) {}

QString MyProvider::name() const {
    return QStringLiteral("MyProvider");
}

QFuture<std::expected<QList<ContentItem>, ApiError>>
MyProvider::fetchViralContent(const QString& platform, int limit, int min_views) {
    return QtConcurrent::run([=]() -> std::expected<QList<ContentItem>, ApiError> {
        // Your implementation here
        QList<ContentItem> items;
        // ... fetch data
        return items;
    });
}

} // namespace
```

### Step 3: Create plugin.json

```json
{
    "name": "MyProvider",
    "version": "1.0.0",
    "type": "datasource",
    "iid": "media-assistant.datasource.myprovider",
    "capabilities": ["fetchViralContent", "fetchContentDetail", "fetchContentMetrics"]
}
```

### Step 4: Add to CMakeLists.txt

```cmake
add_library(my_provider MODULE
    my_provider.h
    my_provider.cpp
)

target_link_libraries(my_provider PRIVATE
    Qt6::Core
    Qt6::Network
    media_assistant_core
)
```

## Creating an AI Provider Plugin

### Key Methods to Implement

```cpp
class MyAiProvider : public QObject, public IAiProviderPlugin {
public:
    // Content analysis
    QFuture<std::expected<AiAnalysisResult, ApiError>>
    analyzeContent(const QString& content, 
                   const ContentMetrics& metrics,
                   const QStringList& dimensions) override;
    
    // Topic suggestions
    QFuture<std::expected<QList<TopicSuggestion>, ApiError>>
    generateTopicSuggestions(const AiAnalysisResult& analyzedData,
                             const QString& platform,
                             int count) override;
    
    // Writing SOP
    QFuture<std::expected<WritingSOP, ApiError>>
    generateWritingSOP(const QString& topic,
                       const QString& targetAudience,
                       const QString& style) override;
};
```

## Testing Plugins

### Unit Test Example

```cpp
#include <QTest>
#include "plugins/datasources/jizhile_provider.h"

class JizhileProviderTest : public QObject {
    Q_OBJECT
    
private slots:
    void testFetchViralContent() {
        JizhileProvider provider;
        provider.setToken("test-token");
        
        auto future = provider.fetchViralContent("wechat", 10, 1000);
        auto result = future.result();
        
        QVERIFY(result.has_value());
        QVERIFY(!result.value().isEmpty());
    }
};

QTEST_APPLESS_MAIN(JizhileProviderTest)
#include "jizhile_provider_test.moc"
```

## Coding Standards

### Comment Style (Bilingual)

```cpp
/**
 * @brief Function description in Chinese
 *        Function description in English
 *
 * @param param Parameter description / 参数说明
 * @return Return value description / 返回值说明
 */
```

### Error Handling

```cpp
if (error_condition) {
    return std::unexpected(ApiError{
        ApiError::Type::NetworkError,
        QStringLiteral("Error message"),
        error_code
    });
}
```

## Build & Debug

### Debug Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_LOGGING=ON
cmake --build . --parallel
```

### Plugin Loading Debug

```cpp
qDebug() << "Loading plugin from:" << pluginPath;
qDebug() << "Plugin IID:" << plugin->iid();
qDebug() << "Plugin name:" << plugin->name();
```
