#include "api/wewe_rss_api.h"
namespace mva::api {
WeWeRssApi::WeWeRssApi(QObject *parent) : DataSourceBase(parent) {}
WeWeRssApi::~WeWeRssApi() = default;
bool WeWeRssApi::initialize(const QVariantMap &config) { Q_UNUSED(config); setAvailable(true); return true; }
QFuture<std::expected<QVariantList, DataSourceError>> WeWeRssApi::searchByKeyword(const QString &, int, int) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantList, DataSourceError>> WeWeRssApi::getTrendingContent(const QString &, const QString &) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantMap, DataSourceError>> WeWeRssApi::getContentMetrics(const QString &) { return QtConcurrent::run([](){ return QVariantMap{}; }); }
} // namespace mva::api
