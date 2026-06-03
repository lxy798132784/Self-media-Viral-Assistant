#include "database/models/article_model.h"
namespace mva::database {
ArticleModel::ArticleModel(QObject *parent) : QObject(parent) {}
ArticleModel::~ArticleModel() = default;
QFuture<std::expected<QVariantList, DatabaseError>> ArticleModel::findAll(int limit, int offset) {
    return QtConcurrent::run([](){ return QVariantList{}; });
}
QFuture<std::expected<QVariantMap, DatabaseError>> ArticleModel::findById(const QString &id) {
    return QtConcurrent::run([](){ return QVariantMap{}; });
}
QFuture<std::expected<bool, DatabaseError>> ArticleModel::save(const QVariantMap &data) {
    return QtConcurrent::run([](){ return true; });
}
} // namespace mva::database

cat > /workspace/include/database/models/article_model.h << 'EOF'
#ifndef ARTICLE_MODEL_H
#define ARTICLE_MODEL_H
#include "database/database_manager.h"
namespace mva::database {
class ArticleModel : public QObject {
    Q_OBJECT
public:
    explicit ArticleModel(QObject *parent = nullptr);
    ~ArticleModel();
    QFuture<std::expected<QVariantList, DatabaseError>> findAll(int limit = 100, int offset = 0);
    QFuture<std::expected<QVariantMap, DatabaseError>> findById(const QString &id);
    QFuture<std::expected<bool, DatabaseError>> save(const QVariantMap &data);
};
} // namespace mva::database
#endif
