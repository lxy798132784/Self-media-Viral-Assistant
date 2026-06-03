#ifndef MEDIA_VIRAL_ASSISTANT_UTILS_LOGGER_H
#define MEDIA_VIRAL_ASSISTANT_UTILS_LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <memory>

namespace mva::utils {

/**
 * @brief 日志级别
 *        Log levels
 */
enum class LogLevel {
    Debug,      ///< 调试信息 / Debug information
    Info,       ///< 普通信息 / Information
    Warning,    ///< 警告 / Warning
    Error,      ///< 错误 / Error
    Critical    ///< 严重错误 / Critical error
};

/**
 * @brief 全局日志管理器
 *        Global log manager
 *
 * @details 支持多级别日志、文件轮转、异步写入、结构化日志。
 *          符合 Google C++ Style 的日志规范。
 *          Supports multi-level logging, file rotation, async writing, structured logging.
 *          Complies with Google C++ Style logging standards.
 */
class Logger : public QObject {
    Q_OBJECT

public:
    static Logger& instance();

    bool initialize(
        const QString& logPath,
        LogLevel minLevel = LogLevel::Info,
        int maxFileSizeMB = 10,
        int maxFiles = 5
    );

    void shutdown();

    void log(LogLevel level, const QString& message, const QString& category = {});
    
    void debug(const QString& message, const QString& category = {});
    void info(const QString& message, const QString& category = {});
    void warning(const QString& message, const QString& category = {});
    void error(const QString& message, const QString& category = {});
    void critical(const QString& message, const QString& category = {});

    void setMinLogLevel(LogLevel level);
    LogLevel getMinLogLevel() const;

    void enableConsoleOutput(bool enable);
    void enableFileOutput(bool enable);

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger() override;

    QString formatMessage(LogLevel level, const QString& message, const QString& category);
    void rotateFiles();

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::utils

// Convenience macros
#define LOG_DEBUG(msg) mva::utils::Logger::instance().debug(msg, __func__)
#define LOG_INFO(msg) mva::utils::Logger::instance().info(msg, __func__)
#define LOG_WARNING(msg) mva::utils::Logger::instance().warning(msg, __func__)
#define LOG_ERROR(msg) mva::utils::Logger::instance().error(msg, __func__)
#define LOG_CRITICAL(msg) mva::utils::Logger::instance().critical(msg, __func__)

#endif
