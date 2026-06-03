#include "database/models/user_model.h"
namespace mva::database {
UserModel::UserModel(QObject *parent) : QObject(parent) {}
UserModel::~UserModel() = default;
QFuture<std::expected<QVariantMap, DatabaseError>> UserModel::findByUsername(const QString &username) {
    return QtConcurrent::run([](){ return QVariantMap{}; });
}
QFuture<std::expected<bool, DatabaseError>> UserModel::create(const QVariantMap &data) {
    return QtConcurrent::run([](){ return true; });
}
} // namespace mva::database

cat > /workspace/include/database/models/user_model.h << 'EOF'
#ifndef USER_MODEL_H
#define USER_MODEL_H
#include "database/database_manager.h"
namespace mva::database {
class UserModel : public QObject {
    Q_OBJECT
public:
    explicit UserModel(QObject *parent = nullptr);
    ~UserModel();
    QFuture<std::expected<QVariantMap, DatabaseError>> findByUsername(const QString &username);
    QFuture<std::expected<bool, DatabaseError>> create(const QVariantMap &data);
};
} // namespace mva::database
#endif
