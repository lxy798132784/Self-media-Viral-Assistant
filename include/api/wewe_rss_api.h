#ifndef WEWE_RSS_API_H
#define WEWE_RSS_API_H
#include "api/data_source_base.h"
namespace mva::api { class WeWeRssApi : public DataSourceBase { Q_OBJECT public: explicit WeWeRssApi(QObject *parent = nullptr); ~WeWeRssApi() override; [[nodiscard]] QString providerName() const override { return QStringLiteral("WeWe RSS"); } [[nodiscard]] bool initialize(const QVariantMap &config) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> getTrendingContent(const QString &category = "", const QString &timeRange = "24h") override; [[nodiscard]] QFuture<std::expected<QVariantMap, DataSourceError>> getContentMetrics(const QString &contentId) override; }; } // namespace mva::api
#endif
