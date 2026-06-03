#ifndef MEDIA_VIRAL_ASSISTANT_CORE_CONFIG_MANAGER_H
#define MEDIA_VIRAL_ASSISTANT_CORE_CONFIG_MANAGER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <memory>

namespace mva::core {

/**
 * @brief 配置管理器，统一管理应用程序的所有配置
 *        Configuration manager that unifies all application configurations
 *
 * @details 支持多种配置源：本地配置文件、环境变量、命令行参数、
 *          远程配置服务器。提供配置的读取、写入、监听变化等功能。
 *          Supports multiple configuration sources: local config files,
 *          environment variables, command line arguments, remote config server.
 *          Provides config read, write, change listening features.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(QObject* parent = nullptr);
    ~ConfigManager() override;

    bool load(const QString& filePath = {});
    bool save(const QString& filePath = {});

    QVariant getValue(const QString& key, const QVariant& defaultValue = {}) const;
    void setValue(const QString& key, const QVariant& value);
    void remove(const QString& key);
    bool contains(const QString& key) const;

    QVariantMap getAll() const;
    void setAll(const QVariantMap& config);

    void setAutoSave(bool enable);

signals:
    void configChanged(const QString& key, const QVariant& newValue);
    void configGroupChanged(const QString& group);
    void configReloaded();

private:
    void setDefaults();
    void setupWatcher();

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::core

#endif
