#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

/**
 * @brief 插件管理器 - 负责插件的加载、卸载、生命周期管理
 *        Plugin Manager - Responsible for plugin loading, unloading, and lifecycle management
 *
 * @details 基于 CTK Plugin Framework 实现，支持动态加载、热插拔、依赖解析。
 *          提供统一的插件注册、发现、调用接口。
 *          Implemented based on CTK Plugin Framework, supports dynamic loading,
 *          hot-swapping, and dependency resolution. Provides unified interfaces
 *          for plugin registration, discovery, and invocation.
 *
 * @note 线程安全，支持并发访问
 *       Thread-safe, supports concurrent access
 *
 * @example
 *   auto& manager = PluginManager::instance();
 *   manager.loadPluginsFromDirectory("./plugins");
 *   
 *   auto dataSource = manager.getDataSourcePlugin("jizhile");
 *   if (dataSource) {
 *       auto result = co_await dataSource->discoverViralContent("wechat", startTime, endTime, 100);
 *   }
 */

#pragma once

#include "plugin_interface.h"
#include <QObject>
#include <QMap>
#include <QMutex>
#include <QReadWriteLock>
#include <ctkPluginContext.h>
#include <ctkPluginFrameworkFactory.h>
#include <memory>
#include <functional>

namespace MediaViralAssistant::Base {

/**
 * @brief 插件信息结构
 *        Plugin information structure
 */
struct PluginInfo {
    QString path;                      ///< 插件路径 / Plugin path
    PluginStatus status;               ///< 插件状态 / Plugin status
    PluginMetadata metadata;           ///< 插件元数据 / Plugin metadata
    std::shared_ptr<ctkPlugin> plugin; ///< CTK 插件对象 / CTK plugin object
    qint64 loadTime;                   ///< 加载时间戳 / Load timestamp
    int loadCount;                     ///< 加载次数 / Load count
    QString errorMessage;              ///< 错误消息 / Error message
    
    PluginInfo() : status(PluginStatus::Unloaded), loadTime(0), loadCount(0) {}
};

/**
 * @brief 插件管理器单例类
 *        Plugin Manager Singleton Class
 */
class PluginManager : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief 获取单例实例
     *        Get singleton instance
     * @return PluginManager 引用 / PluginManager reference
     */
    static PluginManager& instance();
    
    /**
     * @brief 初始化插件框架
     *        Initialize plugin framework
     *
     * @details 启动 CTK Plugin Framework，准备加载插件。
     *          Starts CTK Plugin Framework and prepares for plugin loading.
     *
     * @param storagePath 插件存储路径 / Plugin storage path
     * @return true 如果初始化成功 / true if initialization successful
     */
    bool initialize(const QString& storagePath = "./plugin_cache");
    
    /**
     * @brief 关闭插件框架
     *        Shutdown plugin framework
     *
     * @details 卸载所有插件并释放资源。
     *          Unloads all plugins and releases resources.
     */
    void shutdown();
    
    /**
     * @brief 从目录加载所有插件
     *        Load all plugins from directory
     *
     * @param directory 插件目录 / Plugin directory
     * @param recursive 是否递归子目录 / Whether to recurse subdirectories
     * @return 成功加载的插件数量 / Number of successfully loaded plugins
     */
    int loadPluginsFromDirectory(const QString& directory, bool recursive = false);
    
    /**
     * @brief 加载单个插件
     *        Load single plugin
     *
     * @param pluginPath 插件文件路径 / Plugin file path
     * @return true 如果加载成功 / true if loading successful
     */
    bool loadPlugin(const QString& pluginPath);
    
    /**
     * @brief 卸载插件
     *        Unload plugin
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return true 如果卸载成功 / true if unloading successful
     */
    bool unloadPlugin(const QString& pluginId);
    
    /**
     * @brief 启用/禁用插件
     *        Enable/Disable plugin
     *
     * @param pluginId 插件 ID / Plugin ID
     * @param enabled 是否启用 / Whether to enable
     * @return true 如果操作成功 / true if operation successful
     */
    bool setPluginEnabled(const QString& pluginId, bool enabled);
    
    /**
     * @brief 获取数据源插件
     *        Get data source plugin
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return 数据源插件指针 / Data source plugin pointer
     */
    IDataSourcePlugin* getDataSourcePlugin(const QString& pluginId);
    
    /**
     * @brief 获取 AI 提供商插件
     *        Get AI provider plugin
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return AI 提供商插件指针 / AI provider plugin pointer
     */
    IAIProviderPlugin* getAIProviderPlugin(const QString& pluginId);
    
    /**
     * @brief 获取分析器插件
     *        Get analyzer plugin
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return 分析器插件指针 / Analyzer plugin pointer
     */
    IAnalyzerPlugin* getAnalyzerPlugin(const QString& pluginId);
    
    /**
     * @brief 获取所有已加载的插件信息
     *        Get all loaded plugin information
     * @return 插件信息列表 / List of plugin information
     */
    QList<PluginInfo> getAllPlugins() const;
    
    /**
     * @brief 获取指定类型的插件列表
     *        Get list of plugins by type
     *
     * @tparam T 插件接口类型 / Plugin interface type
     * @return 插件 ID 列表 / List of plugin IDs
     */
    template<typename T>
    QStringList getPluginsByType() const;
    
    /**
     * @brief 检查插件是否已加载
     *        Check if plugin is loaded
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return true 如果已加载 / true if loaded
     */
    bool isPluginLoaded(const QString& pluginId) const;
    
    /**
     * @brief 获取插件状态
     *        Get plugin status
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return 插件状态 / Plugin status
     */
    PluginStatus getPluginStatus(const QString& pluginId) const;
    
    /**
     * @brief 获取插件错误信息
     *        Get plugin error message
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return 错误消息 / Error message
     */
    QString getPluginError(const QString& pluginId) const;
    
    /**
     * @brief 刷新插件缓存
     *        Refresh plugin cache
     *
     * @details 重新扫描插件目录并更新插件列表。
     *          Re-scans plugin directory and updates plugin list.
     */
    void refreshPluginCache();
    
    /**
     * @brief 获取插件上下文
     *        Get plugin context
     * @return CTK 插件上下文 / CTK plugin context
     */
    ctkPluginContext* getPluginContext() const { return m_pluginContext; }
    
signals:
    /**
     * @brief 插件加载完成信号
     *        Plugin loaded signal
     * @param pluginId 插件 ID / Plugin ID
     * @param success 是否成功 / Whether successful
     */
    void pluginLoaded(const QString& pluginId, bool success);
    
    /**
     * @brief 插件卸载完成信号
     *        Plugin unloaded signal
     * @param pluginId 插件 ID / Plugin ID
     */
    void pluginUnloaded(const QString& pluginId);
    
    /**
     * @brief 插件状态改变信号
     *        Plugin status changed signal
     * @param pluginId 插件 ID / Plugin ID
     * @param status 新状态 / New status
     */
    void pluginStatusChanged(const QString& pluginId, PluginStatus status);
    
    /**
     * @brief 插件错误信号
     *        Plugin error signal
     * @param pluginId 插件 ID / Plugin ID
     * @param error 错误信息 / Error information
     */
    void pluginError(const QString& pluginId, const QString& error);
    
private:
    /**
     * @brief 私有构造函数（单例模式）
     *        Private constructor (Singleton pattern)
     */
    PluginManager();
    
    /**
     * @brief 私有析构函数
     *        Private destructor
     */
    ~PluginManager() override;
    
    /**
     * @brief 禁止拷贝构造
     *        Disable copy constructor
     */
    PluginManager(const PluginManager&) = delete;
    
    /**
     * @brief 禁止赋值操作
     *        Disable assignment operator
     */
    PluginManager& operator=(const PluginManager&) = delete;
    
    /**
     * @brief 解析插件依赖
     *        Resolve plugin dependencies
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return true 如果依赖满足 / true if dependencies satisfied
     */
    bool resolveDependencies(const QString& pluginId);
    
    /**
     * @brief 实例化插件接口
     *        Instantiate plugin interface
     *
     * @tparam T 接口类型 / Interface type
     * @param pluginId 插件 ID / Plugin ID
     * @return 接口实例指针 / Interface instance pointer
     */
    template<typename T>
    T* instantiateInterface(const QString& pluginId);
    
private:
    mutable QReadWriteLock m_mutex;           ///< 读写锁 / Read-write lock
    QMap<QString, PluginInfo> m_plugins;      ///< 插件映射 / Plugin map
    ctkPluginContext* m_pluginContext;        ///< 插件上下文 / Plugin context
    std::unique_ptr<ctkPluginFrameworkFactory> m_frameworkFactory; ///< 框架工厂 / Framework factory
    bool m_initialized;                       ///< 是否已初始化 / Is initialized
};

// =============================================================================
// Template Implementations / 模板实现
// =============================================================================

template<typename T>
QStringList PluginManager::getPluginsByType() const {
    QReadLocker locker(&m_mutex);
    QStringList result;
    
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it) {
        if (it->status == PluginStatus::Active && it->enabled) {
            // Try to get the interface
            auto serviceRef = m_pluginContext->getServiceReference<T>();
            if (serviceRef) {
                result << it->metadata.id;
            }
        }
    }
    
    return result;
}

template<typename T>
T* PluginManager::instantiateInterface(const QString& pluginId) {
    QReadLocker locker(&m_mutex);
    
    auto it = m_plugins.find(pluginId);
    if (it == m_plugins.end() || it->status != PluginStatus::Active) {
        return nullptr;
    }
    
    auto serviceRef = m_pluginContext->getServiceReference<T>();
    if (!serviceRef) {
        return nullptr;
    }
    
    return m_pluginContext->getService<T>(serviceRef);
}

} // namespace MediaViralAssistant::Base

#endif // PLUGIN_MANAGER_H
