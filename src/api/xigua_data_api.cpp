#include "api/xigua_data_api.h"
namespace mva::api {
XiguaDataApi::XiguaDataApi(QObject *parent) : DataSourceBase(parent) {}
XiguaDataApi::~XiguaDataApi() = default;
bool XiguaDataApi::initialize(const QVariantMap &config) { Q_UNUSED(config); setAvailable(true); return true; }
QFuture<std::expected<QVariantList, DataSourceError>> XiguaDataApi::searchByKeyword(const QString &, int, int) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantList, DataSourceError>> XiguaDataApi::getTrendingContent(const QString &, const QString &) { return QtConcurrent::run([](){ return QVariantList{}; }); }
QFuture<std::expected<QVariantMap, DataSourceError>> XiguaDataApi::getContentMetrics(const QString &) { return QtConcurrent::run([](){ return QVariantMap{}; }); }
} // namespace mva::api
