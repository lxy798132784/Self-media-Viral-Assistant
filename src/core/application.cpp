/**
 * @file application.cpp
 * @brief 应用程序核心类实现
 *        Application core class implementation
 * 
 * @details 实现单例模式的应用程序核心，管理整个应用的生命周期。
 *          Implements singleton pattern application core managing entire lifecycle.
 */

#include "core/application.h"
#include "core/plugin_manager.h"
#include "core/config_manager.h"
#include "core/event_bus.h"
#include "core/thread_pool.h"
#include "database/database_manager.h"
#include "ai/llm_manager.h"
#include "ui/qml_bridge.h"
#include "utils/logger.h"

#include <QApplication>
#include <QQmlContext>
#include <QDir>
#include <QStandardPaths>
#include <stdexcept>

namespace mva::core {

/**
 * @brief 私有实现结构体
 *        Private implementation structure
 */
struct Application::Impl {
    std::unique_ptr<QApplication> app;              ///< Qt 应用实例 / Qt application instance
    std::unique_ptr<QQmlApplicationEngine> engine;  ///< QML 引擎 / QML engine
    std::unique_ptr<PluginManager> pluginManager;   ///< 插件管理器 / Plugin manager
    std::unique_ptr<ConfigManager> configManager;   ///< 配置管理器 / Config manager
    std::unique_ptr<DatabaseManager> databaseManager; ///< 数据库管理器 / Database manager
    std::unique_ptr<ai::LlmManager> llmManager;     ///< AI 管理器 / AI manager
    std::unique_ptr<ui::QmlBridge> qmlBridge;       ///< QML 桥接 / QML bridge
    bool initialized = false;                        ///< 初始化标志 / Initialization flag
};

Application::Application() : d_(new Impl) {
    utils::Logger::instance().setLogLevel(utils::Logger::Level::Info);
}

Application::~Application() {
    if (d_->initialized) {
        shutdown();
    }
}

Application& Application::instance() {
    static Application instance;
    return instance;
}

bool Application::initialize(int argc, char* argv[]) {
    try {
        // 1. 创建 QApplication / Create QApplication
        d_->app = std::make_unique<QApplication>(argc, argv);
        d_->app->setApplicationName("MediaViralAssistant");
        d_->app->setApplicationVersion("1.0.0");
        d_->app->setOrganizationName("SelfMediaStudio");
        
        setupAttributes();
        
        // 2. 初始化日志系统 / Initialize logging system
        utils::Logger::instance().setLogFile(
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/app.log"
        );
        LOG_INFO << "Application starting...";
        
        // 3. 加载配置文件 / Load configuration files
        d_->configManager = std::make_unique<ConfigManager>();
        if (!d_->configManager->load()) {
            LOG_WARNING << "Failed to load config file, using defaults";
        }
        
        // 4. 初始化数据库连接 / Initialize database connection
        d_->databaseManager = std::make_unique<DatabaseManager>();
        if (!d_->databaseManager->connect()) {
            LOG_ERROR << "Database connection failed";
            return false;
        }
        
        // 5. 加载插件系统 / Load plugin system
        d_->pluginManager = std::make_unique<PluginManager>();
        d_->pluginManager->discoverPlugins();
        
        // 6. 初始化 AI 服务 / Initialize AI services
        d_->llmManager = std::make_unique<ai::LlmManager>();
        d_->llmManager->initialize();
        
        // 7. 注册 QML 类型 / Register QML types
        registerQmlTypes();
        
        // 8. 创建 QML 引擎 / Create QML engine
        d_->engine = std::make_unique<QQmlApplicationEngine>();
        d_->qmlBridge = std::make_unique<ui::QmlBridge>();
        d_->engine->rootContext()->setContextProperty("qmlBridge", d_->qmlBridge.get());
        
        // 9. 加载主界面 / Load main interface
        loadMainInterface();
        
        d_->initialized = true;
        LOG_INFO << "Application initialized successfully";
        
        emit initializationComplete();
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR << "Initialization failed: " << e.what();
        return false;
    }
}

int Application::run() {
    if (!d_->initialized) {
        throw std::runtime_error("Application not initialized");
    }
    
    LOG_INFO << "Starting application main loop";
    return d_->app->exec();
}

void Application::shutdown() {
    LOG_INFO << "Shutting down application...";
    emit aboutToShutdown();
    
    // 1. 保存设置 / Save settings
    if (d_->configManager) {
        d_->configManager->save();
    }
    
    // 2. 关闭数据库 / Close database
    if (d_->databaseManager) {
        d_->databaseManager->disconnect();
    }
    
    // 3. 卸载插件 / Unload plugins
    if (d_->pluginManager) {
        d_->pluginManager->unloadAll();
    }
    
    // 4. 清理 AI 资源 / Clean up AI resources
    if (d_->llmManager) {
        d_->llmManager->shutdown();
    }
    
    // 5. 销毁 QML 引擎 / Destroy QML engine
    d_->engine.reset();
    
    // 6. 销毁应用 / Destroy application
    d_->app.reset();
    
    d_->initialized = false;
    LOG_INFO << "Application shutdown complete";
}

QQmlApplicationEngine* Application::qmlEngine() const {
    return d_->engine.get();
}

bool Application::isInitialized() const {
    return d_->initialized;
}

void Application::setupAttributes() {
    d_->app->setAttribute(Qt::AA_EnableHighDpiScaling);
    d_->app->setAttribute(Qt::AA_UseHighDpiPixmaps);
}

void Application::registerQmlTypes() {
    qmlRegisterType<mva::ui::QmlBridge>("MVA", 1, 0, "QmlBridge");
}

void Application::loadMainInterface() {
    const QString mainQml = ":/qml/main.qml";
    d_->engine->load(QUrl(mainQml));
    
    if (d_->engine->rootObjects().isEmpty()) {
        throw std::runtime_error("Failed to load QML interface");
    }
}

} // namespace mva::core
