#ifndef ARTICLE_MODEL_H
#define ARTICLE_MODEL_H
#include "database/database_manager.h"
namespace mva::database { class ArticleModel : public QObject { Q_OBJECT public: explicit ArticleModel(QObject *parent = nullptr); ~ArticleModel(); QFuture<std::expected<QVariantList, DatabaseError>> findAll(int limit = 100, int offset = 0); QFuture<std::expected<QVariantMap, DatabaseError>> findById(const QString &id); QFuture<std::expected<bool, DatabaseError>> save(const QVariantMap &data); }; }
#endif
