#include "api/api_fallback_chain.h"
namespace mva::api {
ApiFallbackChain::ApiFallbackChain(QObject *parent) : QObject(parent) {}
ApiFallbackChain::~ApiFallbackChain() = default;
void ApiFallbackChain::addProvider(DataSourceBase *provider) { m_providers.append(provider); }
QFuture<std::expected<QVariantList, DataSourceError>> ApiFallbackChain::searchByKeyword(const QString &keyword, int page, int pageSize) {
    return QtConcurrent::run([this, keyword, page, pageSize](){ 
        for (auto *provider : m_providers) {
            if (provider && provider->isAvailable()) {
                auto future = provider->searchByKeyword(keyword, page, pageSize);
                // In real implementation, wait for result and check
                return QVariantList{};
            }
        }
        return QVariantList{};
    });
}
QFuture<std::expected<QVariantList, DataSourceError>> ApiFallbackChain::getTrendingContent(const QString &category, const QString &timeRange) {
    return QtConcurrent::run([](){ return QVariantList{}; });
}
QFuture<std::expected<QVariantMap, DataSourceError>> ApiFallbackChain::getContentMetrics(const QString &contentId) {
    return QtConcurrent::run([](){ return QVariantMap{}; });
}
} // namespace mva::api
