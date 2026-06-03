#ifndef API_FALLBACK_CHAIN_H
#define API_FALLBACK_CHAIN_H
#include "api/data_source_base.h"
#include <QList>
namespace mva::api {
class ApiFallbackChain : public QObject {
    Q_OBJECT
public:
    explicit ApiFallbackChain(QObject *parent = nullptr);
    ~ApiFallbackChain();
    void addProvider(DataSourceBase *provider);
    QFuture<std::expected<QVariantList, DataSourceError>> searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20);
    QFuture<std::expected<QVariantList, DataSourceError>> getTrendingContent(const QString &category = "", const QString &timeRange = "24h");
    QFuture<std::expected<QVariantMap, DataSourceError>> getContentMetrics(const QString &contentId);
private:
    QList<DataSourceBase*> m_providers;
};
} // namespace mva::api
#endif
