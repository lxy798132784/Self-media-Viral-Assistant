/**
 * @file plugin_manager.cpp
 * @brief 插件管理器实现
 *        Plugin manager implementation
 * 
 * @details 实现基于 CTK Plugin Framework 的插件管理系统。
 *          Implements plugin management system based on CTK Plugin Framework.
 */

#include "core/plugin_manager.h"
#include "utils/logger.h"

#include <QDir>
#include <QPluginLoader>
#include <QJsonDocument>
#include <QJsonObject>

namespace mva::core {

/**
 * @brief 私有实现结构体
 *        Private implementation structure
 */
struct PluginManager::Impl {
    QMap<QString, std::unique_ptr<QPluginLoader>> pluginLoaders;  ///< 插件加载器映射 / Plugin loader map
    QStringList searchPaths;                                        ///< 插件搜索路径 / Plugin search paths
    bool initialized = false;                                       ///< 初始化标志 / Initialization flag
};

PluginManager::PluginManager(QObject* parent)
    : QObject(parent), d_(new Impl) {
}

PluginManager::~PluginManager() {
    unloadAll();
}

bool PluginManager::initialize(const QStringList& pluginPaths) {
    if (d_->initialized) {
        LOG_WARNING << "Plugin system already initialized";
        return true;
    }
    
    d_->searchPaths = pluginPaths;
    
    // 验证路径 / Validate paths
    for (const QString& path : pluginPaths) {
        if (!QDir(path).exists()) {
            LOG_WARNING << "Plugin path does not exist: " << path;
        }
    }
    
    d_->initialized = true;
    LOG_INFO << "Plugin system initialized with " << pluginPaths.size() << " search paths";
    return true;
}

bool PluginManager::loadPlugin(const QString& pluginId) {
    if (!d_->initialized) {
        LOG_ERROR << "Plugin system not initialized";
        return false;
    }
    
    if (d_->pluginLoaders.contains(pluginId)) {
        LOG_WARNING << "Plugin already loaded: " << pluginId;
        return true;
    }
    
    // 在所有搜索路径中查找插件文件 / Search for plugin file in all search paths
    QString pluginFile;
    for (const QString& path : d_->searchPaths) {
        QDir dir(path);
        QStringList filters;
        filters << "*.dll" << "*.so" << "*.dylib";
        
        const QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
        for (const QFileInfo& fileInfo : files) {
            // 简单匹配：检查文件名是否包含插件 ID
            // Simple match: check if filename contains plugin ID
            if (fileInfo.fileName().contains(pluginId, Qt::CaseInsensitive)) {
                pluginFile = fileInfo.absoluteFilePath();
                break;
            }
        }
        if (!pluginFile.isEmpty()) {
            break;
        }
    }
    
    if (pluginFile.isEmpty()) {
        LOG_ERROR << "Plugin file not found: " << pluginId;
        emit pluginError(pluginId, "Plugin file not found");
        return false;
    }
    
    auto loader = std::make_unique<QPluginLoader>(pluginFile);
    
    if (!loader->load()) {
        LOG_ERROR << "Failed to load plugin: " << pluginId << ", error: " << loader->errorString();
        emit pluginError(pluginId, loader->errorString());
        return false;
    }
    
    // 获取插件元数据 / Get plugin metadata
    QJsonObject meta = loader->metaData();
    LOG_INFO << "Loaded plugin: " << pluginId << " from " << pluginFile;
    
    d_->pluginLoaders[pluginId] = std::move(loader);
    emit pluginLoaded(pluginId);
    
    return true;
}

bool PluginManager::unloadPlugin(const QString& pluginId) {
    if (!d_->pluginLoaders.contains(pluginId)) {
        LOG_WARNING << "Plugin not loaded: " << pluginId;
        return false;
    }
    
    auto& loader = d_->pluginLoaders[pluginId];
    
    if (!loader->unload()) {
        LOG_ERROR << "Failed to unload plugin: " << pluginId << ", error: " << loader->errorString();
        return false;
    }
    
    d_->pluginLoaders.remove(pluginId);
    LOG_INFO << "Unloaded plugin: " << pluginId;
    emit pluginUnloaded(pluginId);
    
    return true;
}

QStringList PluginManager::getLoadedPlugins() const {
    return d_->pluginLoaders.keys();
}

QVariantMap PluginManager::getPluginInfo(const QString& pluginId) const {
    QVariantMap info;
    
    if (!d_->pluginLoaders.contains(pluginId)) {
        return info;
    }
    
    const auto& loader = d_->pluginLoaders[pluginId];
    QJsonObject metaData = loader->metaData();
    
    // 转换 JSON 为 QVariantMap / Convert JSON to QVariantMap
    info["id"] = pluginId;
    info["fileName"] = loader->fileName();
    info["isLoaded"] = loader->isLoaded();
    info["metaData"] = QJsonDocument(metaData).toVariant();
    
    return info;
}

bool PluginManager::isPluginLoaded(const QString& pluginId) const {
    return d_->pluginLoaders.contains(pluginId);
}

void PluginManager::discoverPlugins() {
    if (!d_->initialized) {
        return;
    }
    
    int discoveredCount = 0;
    
    for (const QString& path : d_->searchPaths) {
        QDir dir(path);
        if (!dir.exists()) {
            continue;
        }
        
        QStringList filters;
        filters << "*.dll" << "*.so" << "*.dylib";
        
        const QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
        for (const QFileInfo& fileInfo : files) {
            QPluginLoader loader(fileInfo.absoluteFilePath());
            QJsonObject metaData = loader.metaData();
            
            if (!metaData.isEmpty()) {
                QString pluginId = metaData["IID"].toString();
                if (!pluginId.isEmpty() && !d_->pluginLoaders.contains(pluginId)) {
                    discoveredCount++;
                    LOG_DEBUG << "Discovered plugin: " << pluginId << " at " << fileInfo.filePath();
                }
            }
        }
    }
    
    LOG_INFO << "Discovered " << discoveredCount << " plugins";
}

void PluginManager::unloadAll() {
    QStringList loadedPlugins = getLoadedPlugins();
    for (const QString& pluginId : loadedPlugins) {
        unloadPlugin(pluginId);
    }
}

} // namespace mva::core
