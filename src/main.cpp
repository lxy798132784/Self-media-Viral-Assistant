/**
 * @file main.cpp
 * @brief 应用程序入口点 / Application entry point
 * 
 * @details 初始化 Qt 应用，设置全局属性，启动主应用程序。
 *          Initializes Qt application, sets global attributes, starts main application.
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QDir>
#include <QLocale>
#include <QTranslator>

#include "core/application.h"
#include "utils/logger.h"

/**
 * @brief 主函数
 *        Main function
 * 
 * @param argc 命令行参数数量 / Command line argument count
 * @param argv 命令行参数数组 / Command line argument array
 * @return int 程序退出码 / Program exit code
 */
int main(int argc, char* argv[]) {
    // Set environment for high DPI displays
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough
    );
#endif

    QApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("MediaViralAssistant");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("MediaViralTeam");
    app.setApplicationDisplayName(QObject::tr("自媒体爆款助手 / Media Viral Assistant"));

    // Set default style
    QQuickStyle::setStyle("Material");

    // Initialize logger
    QString logPath = QDir::homePath() + "/.media-viral-assistant/logs";
    QDir().mkpath(logPath);
    
    if (!mva::utils::Logger::instance().initialize(logPath)) {
        qCritical() << "Failed to initialize logger";
        return -1;
    }

    LOG_INFO("Application starting...");

    try {
        // Get singleton application instance
        auto& mvaApp = mva::core::Application::instance();
        
        // Initialize application
        if (!mvaApp.initialize(argc, argv)) {
            LOG_ERROR("Application initialization failed");
            return -1;
        }

        LOG_INFO("Application initialized successfully");

        // Run application
        int result = mvaApp.run();

        // Cleanup
        mvaApp.shutdown();
        mva::utils::Logger::instance().shutdown();

        LOG_INFO("Application shutdown complete");
        
        return result;

    } catch (const std::exception& e) {
        LOG_CRITICAL(QString("Unhandled exception: %1").arg(e.what()));
        return -1;
    } catch (...) {
        LOG_CRITICAL("Unknown unhandled exception");
        return -1;
    }
}
