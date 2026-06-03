/**
 * @file config_manager.cpp
 * @brief 配置管理器实现
 *        Configuration manager implementation
 * 
 * @details 管理应用程序配置文件，支持热重载、默认值、验证等功能。
 *          Manages application configuration files with hot-reload, defaults, validation.
 */

#include "core/config_manager.h"
#include "utils/logger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

namespace mva::core {

struct ConfigManager::Impl {
    QVariantMap config;                        ///< 当前配置 / Current configuration
    QString configFilePath;                    ///< 配置文件路径 / Config file path
    bool autoSave = true;                      ///< 自动保存 / Auto-save
    QFileSystemWatcher* watcher = nullptr;     ///< 文件监视器 / File watcher
};

ConfigManager::ConfigManager(QObject* parent)
    : QObject(parent), d_(new Impl) {
    
    // 设置默认配置路径 / Set default config path
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    d_->configFilePath = configDir + "/config.json";
}

ConfigManager::~ConfigManager() {
    if (d_->autoSave) {
        save();
    }
}

bool ConfigManager::load(const QString& filePath) {
    if (!filePath.isEmpty()) {
        d_->configFilePath = filePath;
    }
    
    QFile file(d_->configFilePath);
    if (!file.exists()) {
        LOG_INFO << "Config file not found, using defaults: " << d_->configFilePath;
        setDefaults();
        return true;
    }
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG_ERROR << "Failed to open config file: " << d_->configFilePath;
        setDefaults();
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        LOG_ERROR << "Failed to parse config JSON: " << error.errorString();
        setDefaults();
        return false;
    }
    
    d_->config = doc.object().toVariantMap();
    LOG_INFO << "Configuration loaded from: " << d_->configFilePath;
    
    setupWatcher();
    return true;
}

bool ConfigManager::save(const QString& filePath) {
    QString targetPath = filePath.isEmpty() ? d_->configFilePath : filePath;
    
    QJsonObject jsonObj = QJsonObject::fromVariantMap(d_->config);
    QJsonDocument doc(jsonObj);
    
    QFile file(targetPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        LOG_ERROR << "Failed to save config file: " << targetPath;
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    LOG_INFO << "Configuration saved to: " << targetPath;
    return true;
}

QVariant ConfigManager::getValue(const QString& key, const QVariant& defaultValue) const {
    return d_->config.value(key, defaultValue);
}

void ConfigManager::setValue(const QString& key, const QVariant& value) {
    d_->config[key] = value;
    
    if (d_->autoSave) {
        save();
    }
    
    emit configChanged(key, value);
}

QVariantMap ConfigManager::getAll() const {
    return d_->config;
}

void ConfigManager::setAll(const QVariantMap& config) {
    d_->config = config;
    
    if (d_->autoSave) {
        save();
    }
    
    emit configChanged("", QVariant());
}

void ConfigManager::setDefaults() {
    d_->config = {
        {"database/host", "localhost"},
        {"database/port", 5432},
        {"database/name", "media_viral_db"},
        {"database/user", "postgres"},
        {"database/password", ""},
        
        {"api/jizhile_token", ""},
        {"api/xinbang_key", ""},
        {"api/xigua_key", ""},
        {"api/qingbo_appid", ""},
        {"api/qingbo_secret", ""},
        
        {"ai/active_provider", "openai"},
        {"ai/openai_key", ""},
        {"ai/claude_key", ""},
        {"ai/qwen_key", ""},
        {"ai/ernie_key", ""},
        
        {"app/language", "zh_CN"},
        {"app/theme", "light"},
        {"app/auto_update", true},
        {"app/check_interval_hours", 6}
    };
    
    LOG_INFO << "Default configuration set";
}

void ConfigManager::setupWatcher() {
    d_->watcher = new QFileSystemWatcher(this);
    d_->watcher->addPath(d_->configFilePath);
    
    connect(d_->watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString& path) {
        LOG_INFO << "Config file changed, reloading: " << path;
        load(path);
        emit configReloaded();
    });
}

void ConfigManager::setAutoSave(bool enable) {
    d_->autoSave = enable;
}

} // namespace mva::core
