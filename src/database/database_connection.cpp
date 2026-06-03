#include "database/database_connection.h"
namespace mva::database {
DatabaseConnection::DatabaseConnection(QObject *parent) : QObject(parent) {}
DatabaseConnection::~DatabaseConnection() = default;
bool DatabaseConnection::connect(const QString &connectionString) { Q_UNUSED(connectionString); return true; }
void DatabaseConnection::disconnect() {}
bool DatabaseConnection::isConnected() const { return m_connected; }
void DatabaseConnection::setConnected(bool connected) { m_connected = connected; }
} // namespace mva::database

cat > /workspace/include/database/database_connection.h << 'EOF'
#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H
#include <QObject>
#include <QString>
namespace mva::database {
class DatabaseConnection : public QObject {
    Q_OBJECT
public:
    explicit DatabaseConnection(QObject *parent = nullptr);
    ~DatabaseConnection();
    bool connect(const QString &connectionString);
    void disconnect();
    [[nodiscard]] bool isConnected() const;
private:
    bool m_connected{false};
    void setConnected(bool connected);
};
} // namespace mva::database
#endif
