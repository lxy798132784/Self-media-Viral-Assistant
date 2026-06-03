#ifndef MEDIA_VIRAL_ASSISTANT_DATABASE_DATABASE_MANAGER_H
#define MEDIA_VIRAL_ASSISTANT_DATABASE_DATABASE_MANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <memory>

namespace mva::database {

/**
 * @brief 数据库管理器，统一管理 PostgreSQL 连接
 *        Database manager that unifies PostgreSQL connection management
 *
 * @details 提供连接池、事务管理、异步查询等功能。
 *          支持自动重连、连接健康检查。
 *          Provides connection pooling, transaction management, async queries.
 *          Supports auto-reconnect, connection health checks.
 */
class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager& instance();

    bool initialize(const QString& connectionString);
    void shutdown();

    QSqlDatabase getConnection();
    void releaseConnection(QSqlDatabase db);

    bool executeTransaction(const std::function<bool(QSqlDatabase&)>& operations);
    
    bool isConnected() const;
    QString getConnectionString() const;

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString& error);

private:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager() override;

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::database

#endif
