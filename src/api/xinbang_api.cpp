#include "api/xinbang_api.h"
namespace mva::api {
XinBangApi::XinBangApi(QObject *parent) : DataSourceBase(parent) {}
XinBangApi::~XinBangApi() = default;
bool XinBangApi::initialize(const QVariantMap &config) {
    Q_UNUSED(config);
    setAvailable(true);
    return true;
}
QFuture<std::expected<QVariantList, DataSourceError>> XinBangApi::searchByKeyword(const QString &, int, int) {
    return QtConcurrent::run([](){ return std::unexpected(DataSourceError::UnknownError); });
}
QFuture<std::expected<QVariantList, DataSourceError>> XinBangApi::getTrendingContent(const QString &, const QString &) {
    return QtConcurrent::run([](){ return QVariantList{}; });
}
QFuture<std::expected<QVariantMap, DataSourceError>> XinBangApi::getContentMetrics(const QString &) {
    return QtConcurrent::run([](){ return QVariantMap{}; });
}
} // namespace mva::api
