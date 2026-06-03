#include "api/wechat_official_api.h"
namespace mva::api {
WechatOfficialApi::WechatOfficialApi(QObject *parent) : DataSourceBase(parent) {}
WechatOfficialApi::~WechatOfficialApi() = default;
bool WechatOfficialApi::initialize(const QVariantMap &config) { Q_UNUSED(config); setAvailable(true); return true; }
QFuture<std::expected<QVariantList, DataSourceError>> WechatOfficialApi::searchByKeyword(const QString &, int, int) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantList, DataSourceError>> WechatOfficialApi::getTrendingContent(const QString &, const QString &) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantMap, DataSourceError>> WechatOfficialApi::getContentMetrics(const QString &) { return QtConcurrent::run([](){ return QVariantMap{}; }); }
} // namespace mva::api
