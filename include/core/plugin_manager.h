#ifndef MEDIA_VIRAL_ASSISTANT_CORE_PLUGIN_MANAGER_H
#define MEDIA_VIRAL_ASSISTANT_CORE_PLUGIN_MANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <memory>
#include <vector>

namespace mva::core {

/**
 * @brief 插件管理器，负责插件的加载、卸载和生命周期管理
 *        Plugin manager responsible for loading, unloading, and lifecycle management of plugins
 *
 * @details 基于 CTK Plugin Framework 实现，支持动态插件加载、依赖解析、
 *          插件版本兼容性检查。提供插件发现、激活、停用等功能。
 *          Implemented based on CTK Plugin Framework, supports dynamic plugin 
 *          loading, dependency resolution, plugin version compatibility checking.
 *          Provides plugin discovery, activation, deactivation features.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class PluginManager : public QObject {
    Q_OBJECT

public:
    explicit PluginManager(QObject* parent = nullptr);
    ~PluginManager() override;

    /**
     * @brief 初始化插件系统
     *        Initialize plugin system
     *
     * @param pluginPaths 插件搜索路径列表 / List of plugin search paths
     * @return bool 成功返回 true / true on success
     */
    bool initialize(const QStringList& pluginPaths);

    /**
     * @brief 加载指定插件
     *        Load specified plugin
     *
     * @param pluginId 插件唯一标识符 / Unique plugin identifier
     * @return bool 加载成功返回 true / true if loaded successfully
     */
    bool loadPlugin(const QString& pluginId);

    /**
     * @brief 卸载指定插件
     *        Unload specified plugin
     *
     * @param pluginId 插件唯一标识符 / Unique plugin identifier
     * @return bool 卸载成功返回 true / true if unloaded successfully
     */
    bool unloadPlugin(const QString& pluginId);

    /**
     * @brief 获取已加载的插件列表
     *        Get list of loaded plugins
     *
     * @return QStringList 插件 ID 列表 / List of plugin IDs
     */
    QStringList getLoadedPlugins() const;

    /**
     * @brief 获取插件信息
     *        Get plugin information
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return QVariantMap 插件元数据 / Plugin metadata
     */
    QVariantMap getPluginInfo(const QString& pluginId) const;

    /**
     * @brief 检查插件是否已加载
     *        Check if plugin is loaded
     *
     * @param pluginId 插件 ID / Plugin ID
     * @return bool 已加载返回 true / true if loaded
     */
    bool isPluginLoaded(const QString& pluginId) const;

signals:
    void pluginLoaded(const QString& pluginId);
    void pluginUnloaded(const QString& pluginId);
    void pluginError(const QString& pluginId, const QString& error);

private:
    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::core

#endif
