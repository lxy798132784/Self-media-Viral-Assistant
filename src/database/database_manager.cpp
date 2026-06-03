/**
 * @file database_manager.cpp
 * @brief 数据库管理器实现
 *        Database manager implementation
 */

#include "database/database_manager.h"
#include "utils/logger.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutexLocker>

namespace mva::database {

struct DatabaseManager::Impl {
    QString connectionString;                    ///< 连接字符串 / Connection string
    QSqlDatabase db;                             ///< 数据库连接 / Database connection
    QMutex mutex;                                ///< 线程安全锁 / Thread safety lock
    bool connected = false;                      ///< 连接状态 / Connection status
};

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent), d_(new Impl) {
}

DatabaseManager::~DatabaseManager() {
    shutdown();
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initialize(const QString& connectionString) {
    QMutexLocker locker(&d_->mutex);
    
    d_->connectionString = connectionString;
    
    // 创建数据库连接 / Create database connection
    d_->db = QSqlDatabase::addDatabase("QPSQL");
    d_->db.setHostName("localhost");
    d_->db.setPort(5432);
    d_->db.setDatabaseName("media_viral_db");
    d_->db.setUserName("postgres");
    d_->db.setPassword("");
    
    return connect();
}

bool DatabaseManager::connect() {
    if (d_->connected) {
        return true;
    }
    
    if (!d_->db.open()) {
        LOG_ERROR << "Database connection failed: " << d_->db.lastError().text();
        emit errorOccurred(d_->db.lastError().text());
        return false;
    }
    
    d_->connected = true;
    LOG_INFO << "Database connected successfully";
    emit connected();
    
    // 初始化数据库表 / Initialize database tables
    initializeTables();
    
    return true;
}

void DatabaseManager::disconnect() {
    if (d_->connected) {
        d_->db.close();
        d_->connected = false;
        LOG_INFO << "Database disconnected";
        emit disconnected();
    }
}

void DatabaseManager::shutdown() {
    disconnect();
}

QSqlDatabase DatabaseManager::getConnection() {
    return d_->db;
}

void DatabaseManager::releaseConnection(QSqlDatabase db) {
    // 连接池管理逻辑 / Connection pool management logic
    Q_UNUSED(db)
}

bool DatabaseManager::executeTransaction(const std::function<bool(QSqlDatabase&)>& operations) {
    QMutexLocker locker(&d_->mutex);
    
    if (!d_->connected) {
        return false;
    }
    
    d_->db.transaction();
    
    try {
        if (operations(d_->db)) {
            d_->db.commit();
            return true;
        } else {
            d_->db.rollback();
            return false;
        }
    } catch (...) {
        d_->db.rollback();
        return false;
    }
}

bool DatabaseManager::isConnected() const {
    return d_->connected;
}

QString DatabaseManager::getConnectionString() const {
    return d_->connectionString;
}

void DatabaseManager::initializeTables() {
    QSqlQuery query(d_->db);
    
    // 创建文章表 / Create articles table
    QString createArticlesTable = R"(
        CREATE TABLE IF NOT EXISTS articles (
            id SERIAL PRIMARY KEY,
            url VARCHAR(512) UNIQUE NOT NULL,
            title VARCHAR(256) NOT NULL,
            platform VARCHAR(64) NOT NULL,
            author VARCHAR(128),
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    if (!query.exec(createArticlesTable)) {
        LOG_ERROR << "Failed to create articles table: " << query.lastError().text();
    }
    
    // 创建指标表 / Create metrics table
    QString createMetricsTable = R"(
        CREATE TABLE IF NOT EXISTS metrics (
            id SERIAL PRIMARY KEY,
            article_id INTEGER REFERENCES articles(id),
            views BIGINT DEFAULT 0,
            likes BIGINT DEFAULT 0,
            comments BIGINT DEFAULT 0,
            shares BIGINT DEFAULT 0,
            favorites BIGINT DEFAULT 0,
            engagement_rate REAL DEFAULT 0.0,
            fetched_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";
    
    if (!query.exec(createMetricsTable)) {
        LOG_ERROR << "Failed to create metrics table: " << query.lastError().text();
    }
    
    LOG_INFO << "Database tables initialized";
}

} // namespace mva::database
