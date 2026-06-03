/**
 * @file plugin_manager.cpp
 * @brief 插件管理器实现
 *        Plugin Manager Implementation
 */

#include "plugin_manager.h"
#include <QDir>
#include <QFileInfo>
#include <QLoggingCategory>
#include <ctkPluginException.h>

Q_LOGGING_CATEGORY(pluginManagerLog, "MediaViralAssistant.PluginManager")

namespace MediaViralAssistant::Base {

// =============================================================================
// Singleton Instance / 单例实例
// =============================================================================

PluginManager& PluginManager::instance() {
    static PluginManager instance;
    return instance;
}

// =============================================================================
// Constructor & Destructor / 构造函数与析构函数
// =============================================================================

PluginManager::PluginManager() 
    : m_pluginContext(nullptr)
    , m_frameworkFactory(nullptr)
    , m_initialized(false) {
    qCInfo(pluginManagerLog) << "PluginManager constructed";
}

PluginManager::~PluginManager() {
    shutdown();
    qCInfo(pluginManagerLog) << "PluginManager destroyed";
}

// =============================================================================
// Initialization & Shutdown / 初始化与关闭
// =============================================================================

bool PluginManager::initialize(const QString& storagePath) {
    QWriteLocker locker(&m_mutex);
    
    if (m_initialized) {
        qCWarning(pluginManagerLog) << "PluginManager already initialized";
        return true;
    }
    
    try {
        // Create plugin framework factory / 创建插件框架工厂
        m_frameworkFactory = std::make_unique<ctkPluginFrameworkFactory>();
        
        // Get plugin context / 获取插件上下文
        m_pluginContext = m_frameworkFactory->getFramework()->getPluginContext();
        
        // Start the framework / 启动框架
        m_frameworkFactory->getFramework()->start();
        
        qCInfo(pluginManagerLog) << "Plugin framework initialized with storage path:" << storagePath;
        m_initialized = true;
        
        return true;
    } catch (const ctkPluginException& e) {
        qCCritical(pluginManagerLog) << "Failed to initialize plugin framework:" << e.what();
        return false;
    }
}

void PluginManager::shutdown() {
    QWriteLocker locker(&m_mutex);
    
    if (!m_initialized) {
        return;
    }
    
    qCInfo(pluginManagerLog) << "Shutting down plugin framework...";
    
    // Unload all plugins / 卸载所有插件
    auto pluginIds = m_plugins.keys();
    for (const auto& pluginId : pluginIds) {
        unloadPlugin(pluginId);
    }
    
    // Stop framework / 停止框架
    if (m_frameworkFactory) {
        try {
            m_frameworkFactory->getFramework()->stop();
        } catch (const ctkPluginException& e) {
            qCCritical(pluginManagerLog) << "Error stopping framework:" << e.what();
        }
    }
    
    m_plugins.clear();
    m_initialized = false;
    
    qCInfo(pluginManagerLog) << "Plugin framework shut down complete";
}

// =============================================================================
// Plugin Loading / 插件加载
// =============================================================================

int PluginManager::loadPluginsFromDirectory(const QString& directory, bool recursive) {
    QWriteLocker locker(&m_mutex);
    
    if (!m_initialized) {
        qCCritical(pluginManagerLog) << "PluginManager not initialized";
        return 0;
    }
    
    QDir dir(directory);
    if (!dir.exists()) {
        qCWarning(pluginManagerLog) << "Plugin directory does not exist:" << directory;
        return 0;
    }
    
    int loadedCount = 0;
    QStringList filters;
    filters << "*.so" << "*.dll" << "*.dylib";
    
    QFileInfoList fileInfoList;
    
    if (recursive) {
        // Recursive search / 递归搜索
        QStringList allFiles = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
        for (const auto& file : allFiles) {
            fileInfoList << QFileInfo(dir.absoluteFilePath(file));
        }
        
        // Search subdirectories / 搜索子目录
        QFileInfoList subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const auto& subDir : subDirs) {
            fileInfoList += QDir(subDir.absoluteFilePath())
                .entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
        }
    } else {
        fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
    }
    
    locker.unlock(); // Release lock for individual loading / 释放锁以进行单独加载
    
    for (const auto& fileInfo : fileInfoList) {
        // Try to load each plugin / 尝试加载每个插件
        if (loadPlugin(fileInfo.absoluteFilePath())) {
            loadedCount++;
        }
    }
    
    qCInfo(pluginManagerLog) << "Loaded" << loadedCount << "plugins from" << directory;
    return loadedCount;
}

bool PluginManager::loadPlugin(const QString& pluginPath) {
    QWriteLocker locker(&m_mutex);
    
    if (!m_initialized) {
        qCCritical(pluginManagerLog) << "PluginManager not initialized";
        return false;
    }
    
    // Check if already loaded / 检查是否已加载
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (it->path == pluginPath) {
            qCWarning(pluginManagerLog) << "Plugin already loaded:" << pluginPath;
            return true;
        }
    }
    
    try {
        // Install plugin / 安装插件
        QFile pluginFile(pluginPath);
        if (!pluginFile.open(QIODevice::ReadOnly)) {
            qCCritical(pluginManagerLog) << "Cannot open plugin file:" << pluginPath;
            return false;
        }
        
        auto plugin = m_pluginContext->installPlugin(&pluginFile);
        if (!plugin) {
            qCCritical(pluginManagerLog) << "Failed to install plugin:" << pluginPath;
            return false;
        }
        
        // Create plugin info / 创建插件信息
        PluginInfo info;
        info.path = pluginPath;
        info.plugin = plugin;
        info.loadTime = QDateTime::currentMSecsSinceEpoch();
        info.loadCount = 1;
        info.status = PluginStatus::Loading;
        
        // Get metadata from plugin / 从插件获取元数据
        auto dictionary = plugin->getHeaders();
        info.metadata.id = dictionary.value("Plugin-Id", pluginPath).toString();
        info.metadata.name = dictionary.value("Plugin-Name", "").toString();
        info.metadata.version = dictionary.value("Plugin-Version", "1.0.0").toString();
        info.metadata.description = dictionary.value("Plugin-Description", "").toString();
        info.metadata.author = dictionary.value("Plugin-Author", "").toString();
        
        m_plugins[info.metadata.id] = info;
        
        locker.unlock();
        
        // Start plugin / 启动插件
        try {
            plugin->start(ctkPlugin::START_ACTIVATION_POLICY);
            
            QWriteLocker innerLocker(&m_mutex);
            auto& updatedInfo = m_plugins[info.metadata.id];
            updatedInfo.status = PluginStatus::Active;
            
            qCInfo(pluginManagerLog) << "Plugin loaded successfully:" 
                                     << info.metadata.name 
                                     << "(v" << info.metadata.version << ")";
            
            emit pluginLoaded(info.metadata.id, true);
            emit pluginStatusChanged(info.metadata.id, PluginStatus::Active);
            
            return true;
        } catch (const ctkPluginException& e) {
            QWriteLocker innerLocker(&m_mutex);
            auto& updatedInfo = m_plugins[info.metadata.id];
            updatedInfo.status = PluginStatus::Error;
            updatedInfo.errorMessage = e.what();
            
            qCCritical(pluginManagerLog) << "Failed to start plugin:" 
                                        << info.metadata.name 
                                        << ":" << e.what();
            
            emit pluginLoaded(info.metadata.id, false);
            emit pluginError(info.metadata.id, e.what());
            
            return false;
        }
        
    } catch (const ctkPluginException& e) {
        qCCritical(pluginManagerLog) << "Exception while loading plugin:" 
                                    << pluginPath << ":" << e.what();
        return false;
    }
}

bool PluginManager::unloadPlugin(const QString& pluginId) {
    QWriteLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        qCWarning(pluginManagerLog) << "Plugin not found:" << pluginId;
        return false;
    }
    
    try {
        // Stop plugin if active / 如果激活则停止插件
        if (it->status == PluginStatus::Active || it->status == PluginStatus::Loaded) {
            it->plugin->stop();
        }
        
        // Uninstall plugin / 卸载插件
        it->plugin->uninstall();
        
        qCInfo(pluginManagerLog) << "Plugin unloaded:" << pluginId;
        
        m_plugins.erase(it);
        
        emit pluginUnloaded(pluginId);
        
        return true;
    } catch (const ctkPluginException& e) {
        qCCritical(pluginManagerLog) << "Failed to unload plugin:" 
                                    << pluginId << ":" << e.what();
        
        emit pluginError(pluginId, e.what());
        
        return false;
    }
}

bool PluginManager::setPluginEnabled(const QString& pluginId, bool enabled) {
    QWriteLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end()) {
        qCWarning(pluginManagerLog) << "Plugin not found:" << pluginId;
        return false;
    }
    
    it->metadata.enabled = enabled;
    
    if (!enabled && it->status == PluginStatus::Active) {
        // Deactivate plugin / 停用插件
        try {
            it->plugin->stop();
            it->status = PluginStatus::Disabled;
            
            qCInfo(pluginManagerLog) << "Plugin disabled:" << pluginId;
            emit pluginStatusChanged(pluginId, PluginStatus::Disabled);
        } catch (const ctkPluginException& e) {
            qCCritical(pluginManagerLog) << "Failed to disable plugin:" 
                                        << pluginId << ":" << e.what();
            return false;
        }
    } else if (enabled && it->status == PluginStatus::Disabled) {
        // Activate plugin / 激活插件
        try {
            it->plugin->start(ctkPlugin::START_ACTIVATION_POLICY);
            it->status = PluginStatus::Active;
            
            qCInfo(pluginManagerLog) << "Plugin enabled:" << pluginId;
            emit pluginStatusChanged(pluginId, PluginStatus::Active);
        } catch (const ctkPluginException& e) {
            qCCritical(pluginManagerLog) << "Failed to enable plugin:" 
                                        << pluginId << ":" << e.what();
            return false;
        }
    }
    
    return true;
}

// =============================================================================
// Plugin Access / 插件访问
// =============================================================================

IDataSourcePlugin* PluginManager::getDataSourcePlugin(const QString& pluginId) {
    return instantiateInterface<IDataSourcePlugin>(pluginId);
}

IAIProviderPlugin* PluginManager::getAIProviderPlugin(const QString& pluginId) {
    return instantiateInterface<IAIProviderPlugin>(pluginId);
}

IAnalyzerPlugin* PluginManager::getAnalyzerPlugin(const QString& pluginId) {
    return instantiateInterface<IAnalyzerPlugin>(pluginId);
}

QList<PluginInfo> PluginManager::getAllPlugins() const {
    QReadLocker locker(&m_mutex);
    return m_plugins.values();
}

bool PluginManager::isPluginLoaded(const QString& pluginId) const {
    QReadLocker locker(&m_mutex);
    return m_plugins.contains(pluginId);
}

PluginStatus PluginManager::getPluginStatus(const QString& pluginId) const {
    QReadLocker locker(&m_mutex);
    auto it = m_plugins.find(pluginId);
    return (it != m_plugins.end()) ? it->status : PluginStatus::Unloaded;
}

QString PluginManager::getPluginError(const QString& pluginId) const {
    QReadLocker locker(&m_mutex);
    auto it = m_plugins.find(pluginId);
    return (it != m_plugins.end()) ? it->errorMessage : QString();
}

void PluginManager::refreshPluginCache() {
    // Implementation for cache refresh / 缓存刷新实现
    qCInfo(pluginManagerLog) << "Refreshing plugin cache...";
    // TODO: Implement cache refresh logic
}

bool PluginManager::resolveDependencies(const QString& pluginId) {
    // TODO: Implement dependency resolution / 实现依赖解析
    Q_UNUSED(pluginId);
    return true;
}

} // namespace MediaViralAssistant::Base
