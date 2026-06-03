#ifndef QINGBO_API_H
#define QINGBO_API_H
#include "api/data_source_base.h"
namespace mva::api { class QingBoApi : public DataSourceBase { Q_OBJECT public: explicit QingBoApi(QObject *parent = nullptr); ~QingBoApi() override; [[nodiscard]] QString providerName() const override { return QStringLiteral("清博智能"); } [[nodiscard]] bool initialize(const QVariantMap &config) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> getTrendingContent(const QString &category = "", const QString &timeRange = "24h") override; [[nodiscard]] QFuture<std::expected<QVariantMap, DataSourceError>> getContentMetrics(const QString &contentId) override; }; } // namespace mva::api
#endif
