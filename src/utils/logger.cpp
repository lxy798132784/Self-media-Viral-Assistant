/**
 * @file logger.cpp
 * @brief 日志管理器实现
 *        Logger implementation
 * 
 * @details 实现企业级日志系统，支持多级别、文件轮转、异步写入。
 *          Implements enterprise-grade logging system with multi-level, file rotation, async writing.
 */

#include "utils/logger.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QMutexLocker>
#include <QTextStream>
#include <QThread>
#include <QQueue>
#include <QTimer>

namespace mva::utils {

/**
 * @brief 私有实现结构体
 *        Private implementation structure
 */
struct Logger::Impl {
    QFile logFile;                                    ///< 日志文件 / Log file
    QString logDirectory;                             ///< 日志目录 / Log directory
    LogLevel minLevel = LogLevel::Info;               ///< 最小日志级别 / Minimum log level
    int maxFileSizeBytes = 10 * 1024 * 1024;          ///< 最大文件大小 (10MB) / Max file size
    int maxFiles = 5;                                 ///< 最大文件数 / Max number of files
    bool consoleOutput = true;                        ///< 是否输出到控制台 / Output to console
    bool fileOutput = true;                           ///< 是否输出到文件 / Output to file
    QMutex mutex;                                     ///< 线程安全锁 / Thread safety lock
    QQueue<QString> asyncQueue;                       ///< 异步日志队列 / Async log queue
    QTimer* flushTimer = nullptr;                     ///< 定时刷新器 / Flush timer
    
    static const char* levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }
};

Logger::Logger(QObject* parent) : QObject(parent), d_(new Impl) {
}

Logger::~Logger() {
    shutdown();
}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

bool Logger::initialize(const QString& logPath, LogLevel minLevel, int maxFileSizeMB, int maxFiles) {
    QMutexLocker locker(&d_->mutex);
    
    d_->logDirectory = logPath;
    d_->minLevel = minLevel;
    d_->maxFileSizeBytes = maxFileSizeMB * 1024 * 1024;
    d_->maxFiles = maxFiles;
    
    // 创建日志目录 / Create log directory
    QDir dir(d_->logDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // 设置日志文件路径 / Set log file path
    QString logFileName = d_->logDirectory + "/app.log";
    d_->logFile.setFileName(logFileName);
    
    // 检查是否需要轮转 / Check if rotation is needed
    rotateFiles();
    
    // 打开文件 / Open file
    if (d_->fileOutput && !d_->logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open log file:" << logFileName;
        d_->fileOutput = false;
    }
    
    // 设置定时刷新 / Setup flush timer
    d_->flushTimer = new QTimer(this);
    connect(d_->flushTimer, &QTimer::timeout, this, [this]() {
        QMutexLocker locker(&d_->mutex);
        if (d_->logFile.isOpen()) {
            d_->logFile.flush();
        }
    });
    d_->flushTimer->start(1000);  // 每秒刷新 / Flush every second
    
    log(LogLevel::Info, "Logger initialized", "System");
    return true;
}

void Logger::shutdown() {
    QMutexLocker locker(&d_->mutex);
    
    if (d_->flushTimer) {
        d_->flushTimer->stop();
    }
    
    // 刷新所有待写入的日志 / Flush all pending logs
    while (!d_->asyncQueue.isEmpty()) {
        QString message = d_->asyncQueue.dequeue();
        if (d_->fileOutput && d_->logFile.isOpen()) {
            QTextStream stream(&d_->logFile);
            stream << message << "\n";
        }
    }
    
    if (d_->logFile.isOpen()) {
        d_->logFile.close();
    }
    
    log(LogLevel::Info, "Logger shutdown", "System");
}

void Logger::log(LogLevel level, const QString& message, const QString& category) {
    if (level < d_->minLevel) {
        return;
    }
    
    QString formattedMsg = formatMessage(level, message, category);
    
    QMutexLocker locker(&d_->mutex);
    
    // 输出到控制台 / Output to console
    if (d_->consoleOutput) {
        QTextStream out(stdout);
        out << formattedMsg << "\n";
        out.flush();
    }
    
    // 输出到文件 / Output to file
    if (d_->fileOutput && d_->logFile.isOpen()) {
        QTextStream stream(&d_->logFile);
        stream << formattedMsg << "\n";
        
        // 检查文件大小 / Check file size
        if (d_->logFile.size() >= d_->maxFileSizeBytes) {
            locker.unlock();
            rotateFiles();
        }
    }
}

void Logger::debug(const QString& message, const QString& category) {
    log(LogLevel::Debug, message, category);
}

void Logger::info(const QString& message, const QString& category) {
    log(LogLevel::Info, message, category);
}

void Logger::warning(const QString& message, const QString& category) {
    log(LogLevel::Warning, message, category);
}

void Logger::error(const QString& message, const QString& category) {
    log(LogLevel::Error, message, category);
}

void Logger::critical(const QString& message, const QString& category) {
    log(LogLevel::Critical, message, category);
}

void Logger::setMinLogLevel(LogLevel level) {
    QMutexLocker locker(&d_->mutex);
    d_->minLevel = level;
}

LogLevel Logger::getMinLogLevel() const {
    return d_->minLevel;
}

void Logger::enableConsoleOutput(bool enable) {
    QMutexLocker locker(&d_->mutex);
    d_->consoleOutput = enable;
}

void Logger::enableFileOutput(bool enable) {
    QMutexLocker locker(&d_->mutex);
    d_->fileOutput = enable;
}

QString Logger::formatMessage(LogLevel level, const QString& message, const QString& category) {
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString threadId = QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()), 16);
    QString levelStr = Impl::levelToString(level);
    
    QString result = QString("[%1] [%2] [Thread-%3]")
        .arg(timestamp)
        .arg(levelStr)
        .arg(threadId);
    
    if (!category.isEmpty()) {
        result += QString(" [%1]").arg(category);
    }
    
    result += ": " + message;
    return result;
}

void Logger::rotateFiles() {
    if (!d_->logFile.exists()) {
        return;
    }
    
    // 删除最旧的文件 / Delete oldest file
    QString oldestFile = d_->logDirectory + "/app.log." + QString::number(d_->maxFiles);
    if (QFile::exists(oldestFile)) {
        QFile::remove(oldestFile);
    }
    
    // 重命名现有文件 / Rename existing files
    for (int i = d_->maxFiles - 1; i >= 1; --i) {
        QString oldName = d_->logDirectory + "/app.log." + QString::number(i);
        QString newName = d_->logDirectory + "/app.log." + QString::number(i + 1);
        if (QFile::exists(oldName)) {
            QFile::rename(oldName, newName);
        }
    }
    
    // 重命名当前文件 / Rename current file
    QString currentLog = d_->logDirectory + "/app.log";
    if (QFile::exists(currentLog)) {
        QFile::rename(currentLog, d_->logDirectory + "/app.log.1");
    }
    
    // 重新打开新文件 / Reopen new file
    if (d_->logFile.isOpen()) {
        d_->logFile.close();
    }
    d_->logFile.setFileName(currentLog);
    if (d_->fileOutput && !d_->logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to reopen log file after rotation";
    }
}

void Logger::setLogFile(const QString& logPath) {
    initialize(logPath);
}

void Logger::setLogLevel(LogLevel level) {
    setMinLogLevel(level);
}

} // namespace mva::utils
