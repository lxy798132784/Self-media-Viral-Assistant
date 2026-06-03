#ifndef USER_MODEL_H
#define USER_MODEL_H
#include "database/database_manager.h"
namespace mva::database { class UserModel : public QObject { Q_OBJECT public: explicit UserModel(QObject *parent = nullptr); ~UserModel(); QFuture<std::expected<QVariantMap, DatabaseError>> findByUsername(const QString &username); QFuture<std::expected<bool, DatabaseError>> create(const QVariantMap &data); }; }
#endif
