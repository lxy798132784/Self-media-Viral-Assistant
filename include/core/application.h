#ifndef MEDIA_VIRAL_ASSISTANT_CORE_APPLICATION_H
#define MEDIA_VIRAL_ASSISTANT_CORE_APPLICATION_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QScopedPointer>
#include <memory>

namespace mva::core {

/**
 * @brief 应用程序核心类，管理整个应用的生命周期
 *        Application core class that manages the entire application lifecycle
 *
 * @details 单例模式实现，负责初始化所有子系统，包括插件管理器、配置管理器、
 *          数据库连接、AI 服务提供者等。提供应用启动、运行和关闭的统一入口。
 *          Singleton pattern implementation responsible for initializing all 
 *          subsystems including plugin manager, config manager, database 
 *          connections, AI service providers, etc. Provides unified entry 
 *          points for application startup, run, and shutdown.
 *
 * @threadsafe 部分线程安全 / Partially thread-safe
 * @see PluginManager, ConfigManager, DatabaseManager
 *
 * @example
 *   auto& app = Application::instance();
 *   app.initialize(argc, argv);
 *   app.run();
 */
class Application : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Application)

public:
    /**
     * @brief 获取单例实例
     *        Get singleton instance
     *
     * @return Application& 全局唯一实例引用 / Reference to global unique instance
     *
     * @note 首次调用时创建实例 / Creates instance on first call
     */
    static Application& instance();

    /**
     * @brief 初始化应用程序
     *        Initialize the application
     *
     * @details 执行以下初始化步骤：
     *          1. 解析命令行参数
     *          2. 加载配置文件
     *          3. 初始化日志系统
     *          4. 建立数据库连接
     *          5. 加载插件系统
     *          6. 注册 QML 类型
     *          7. 初始化 AI 服务
     *          
     *          Performs the following initialization steps:
     *          1. Parse command line arguments
     *          2. Load configuration files
     *          3. Initialize logging system
     *          4. Establish database connections
     *          5. Load plugin system
     *          6. Register QML types
     *          7. Initialize AI services
     *
     * @param argc 命令行参数数量 / Command line argument count
     * @param argv 命令行参数数组 / Command line argument array
     * @return bool 成功返回 true，失败返回 false / true on success, false on failure
     *
     * @throws std::runtime_error 当关键组件初始化失败时 / When critical component initialization fails
     */
    bool initialize(int argc, char* argv[]);

    /**
     * @brief 运行应用程序主循环
     *        Run the application main loop
     *
     * @details 启动事件循环，显示主窗口，处理用户交互。
     *          阻塞直到应用程序退出。
     *          Starts event loop, displays main window, handles user interactions.
     *          Blocks until application exits.
     *
     * @return int 应用程序退出码 / Application exit code
     */
    int run();

    /**
     * @brief 关闭应用程序，释放所有资源
     *        Shutdown application and release all resources
     *
     * @details 按相反顺序清理所有初始化的组件：
     *          1. 保存用户设置
     *          2. 关闭数据库连接
     *          3. 卸载插件
     *          4. 清理 AI 资源
     *          5. 刷新日志
     *          
     *          Cleans up all initialized components in reverse order:
     *          1. Save user settings
     *          2. Close database connections
     *          3. Unload plugins
     *          4. Clean up AI resources
     *          5. Flush logs
     */
    void shutdown();

    /**
     * @brief 获取 QML 引擎实例
     *        Get QML engine instance
     *
     * @return QQmlApplicationEngine* QML 引擎指针 / Pointer to QML engine
     */
    QQmlApplicationEngine* qmlEngine() const;

    /**
     * @brief 检查应用是否已初始化
     *        Check if application is initialized
     *
     * @return bool 已初始化返回 true / true if initialized
     */
    bool isInitialized() const;

signals:
    /**
     * @brief 应用即将关闭时发出
     *        Emitted when application is about to shutdown
     */
    void aboutToShutdown();

    /**
     * @brief 应用初始化完成时发出
     *        Emitted when application initialization completes
     */
    void initializationComplete();

private:
    /**
     * @brief 私有构造函数（单例模式）
     *        Private constructor (singleton pattern)
     */
    Application();

    /**
     * @brief 虚析构函数
     *        Virtual destructor
     */
    ~Application() override;

    /**
     * @brief 设置应用程序属性
     *        Set application attributes
     */
    void setupAttributes();

    /**
     * @brief 注册 QML 类型
     *        Register QML types
     */
    void registerQmlTypes();

    /**
     * @brief 加载主 QML 界面
     *        Load main QML interface
     */
    void loadMainInterface();

private:
    struct Impl;
    QScopedPointer<Impl> d_;  ///< 私有实现指针 / Private implementation pointer
};

} // namespace mva::core

#endif // MEDIA_VIRAL_ASSISTANT_CORE_APPLICATION_H
