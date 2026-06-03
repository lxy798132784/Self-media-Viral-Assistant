#ifndef XIGUA_DATA_API_H
#define XIGUA_DATA_API_H
#include "api/data_source_base.h"
namespace mva::api { class XiguaDataApi : public DataSourceBase { Q_OBJECT public: explicit XiguaDataApi(QObject *parent = nullptr); ~XiguaDataApi() override; [[nodiscard]] QString providerName() const override { return QStringLiteral("西瓜数据"); } [[nodiscard]] bool initialize(const QVariantMap &config) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> getTrendingContent(const QString &category = "", const QString &timeRange = "24h") override; [[nodiscard]] QFuture<std::expected<QVariantMap, DataSourceError>> getContentMetrics(const QString &contentId) override; }; } // namespace mva::api
#endif
