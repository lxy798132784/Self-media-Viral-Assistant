#ifndef WECHAT_OFFICIAL_API_H
#define WECHAT_OFFICIAL_API_H
#include "api/data_source_base.h"
namespace mva::api { class WechatOfficialApi : public DataSourceBase { Q_OBJECT public: explicit WechatOfficialApi(QObject *parent = nullptr); ~WechatOfficialApi() override; [[nodiscard]] QString providerName() const override { return QStringLiteral("微信官方 API"); } [[nodiscard]] bool initialize(const QVariantMap &config) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20) override; [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> getTrendingContent(const QString &category = "", const QString &timeRange = "24h") override; [[nodiscard]] QFuture<std::expected<QVariantMap, DataSourceError>> getContentMetrics(const QString &contentId) override; }; } // namespace mva::api
#endif
