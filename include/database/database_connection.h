#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H
#include <QObject>
#include <QString>
namespace mva::database { class DatabaseConnection : public QObject { Q_OBJECT public: explicit DatabaseConnection(QObject *parent = nullptr); ~DatabaseConnection(); bool connect(const QString &connectionString); void disconnect(); [[nodiscard]] bool isConnected() const; private: bool m_connected{false}; void setConnected(bool connected); }; }
#endif
