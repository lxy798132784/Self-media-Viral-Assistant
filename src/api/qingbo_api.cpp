#include "api/qingbo_api.h"
namespace mva::api {
QingBoApi::QingBoApi(QObject *parent) : DataSourceBase(parent) {}
QingBoApi::~QingBoApi() = default;
bool QingBoApi::initialize(const QVariantMap &config) { Q_UNUSED(config); setAvailable(true); return true; }
QFuture<std::expected<QVariantList, DataSourceError>> QingBoApi::searchByKeyword(const QString &, int, int) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantList, DataSourceError>> QingBoApi::getTrendingContent(const QString &, const QString &) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantMap, DataSourceError>> QingBoApi::getContentMetrics(const QString &) { return QtConcurrent::run([](){ return QVariantMap{}; }); }
} // namespace mva::api
