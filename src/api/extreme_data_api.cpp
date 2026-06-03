/**
 * @file extreme_data_api.cpp
 * @brief Implementation of ExtremeDataApi for 极致了 data source
 * @brief 极致了数据源的 ExtremeDataApi 实现
 */

#include "api/extreme_data_api.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtConcurrent>

namespace mva::api {

struct ExtremeDataApi::Private {
    QString baseUrl = QStringLiteral("https://api.jizhile.com");
    int requestId = 0;
};

ExtremeDataApi::ExtremeDataApi(QObject *parent)
    : DataSourceBase(parent)
    , d(std::make_unique<Private>())
{
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &ExtremeDataApi::onReplyFinished);
}

ExtremeDataApi::~ExtremeDataApi() = default;

bool ExtremeDataApi::initialize(const QVariantMap &config)
{
    if (config.contains(QStringLiteral("api_key"))) {
        m_apiKey = config[QStringLiteral("api_key")].toString();
    }
    
    if (config.contains(QStringLiteral("province_code"))) {
        m_provinceCode = config[QStringLiteral("province_code")].toString();
    }
    
    if (config.contains(QStringLiteral("device_id"))) {
        m_deviceId = config[QStringLiteral("device_id")].toString();
    } else {
        m_deviceId = generateDeviceFingerprint().toHex();
    }
    
    if (config.contains(QStringLiteral("os_version"))) {
        m_osVersion = config[QStringLiteral("os_version")].toString();
    } else {
        m_osVersion = QStringLiteral("15.0");
    }
    
    if (m_apiKey.isEmpty()) {
        setError(DataSourceError::AuthenticationFailed, 
                 QStringLiteral("API key is required for 极致了"));
        return false;
    }
    
    setAvailable(true);
    qInfo() << "[极致了] Initialized with device:" << m_deviceId;
    return true;
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::searchByKeyword(const QString &keyword, int page, int pageSize)
{
    // Default to general search including articles and videos
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("keyword")] = keyword;
    params[QStringLiteral("page")] = page;
    params[QStringLiteral("page_size")] = pageSize;
    params[QStringLiteral("mode")] = 1; // mode=1: search all
    
    return executeSearch(QStringLiteral("/search/all"), params);
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::getTrendingContent(const QString &category, const QString &timeRange)
{
    QVariantMap params = buildCommonParams();
    if (!category.isEmpty()) {
        params[QStringLiteral("category")] = category;
    }
    params[QStringLiteral("time_range")] = timeRange;
    params[QStringLiteral("sort_by")] = QStringLiteral("hot");
    
    return executeSearch(QStringLiteral("/trending"), params);
}

QFuture<std::expected<QVariantMap, DataSourceError>> 
ExtremeDataApi::getContentMetrics(const QString &contentId)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("content_id")] = contentId;
    
    return executeRequest(QStringLiteral("/content/metrics"), params);
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::searchVideoAccounts(const QString &keyword, int page)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("keyword")] = keyword;
    params[QStringLiteral("page")] = page;
    
    return executeSearch(QStringLiteral("/video-account/search"), params);
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::searchVideoContent(const QString &keyword, int page, const QString &sortBy)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("keyword")] = keyword;
    params[QStringLiteral("page")] = page;
    params[QStringLiteral("sort_by")] = sortBy;
    
    return executeSearch(QStringLiteral("/video/search"), params);
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::searchArticles(const QString &keyword, int page, const QString &timeRange)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("keyword")] = keyword;
    params[QStringLiteral("page")] = page;
    if (!timeRange.isEmpty()) {
        params[QStringLiteral("time_range")] = timeRange;
    }
    
    return executeSearch(QStringLiteral("/article/search"), params);
}

QFuture<std::expected<QVariantMap, DataSourceError>> 
ExtremeDataApi::getVideoAccountInfo(const QString &accountId)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("account_id")] = accountId;
    
    return executeRequest(QStringLiteral("/video-account/info"), params);
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::getVideoAccountWorks(const QString &accountId, int page)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("account_id")] = accountId;
    params[QStringLiteral("page")] = page;
    
    return executeSearch(QStringLiteral("/video-account/works"), params);
}

QFuture<std::expected<QVariantMap, DataSourceError>> 
ExtremeDataApi::getArticleMetrics(const QString &articleUrl)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("url")] = articleUrl;
    
    return executeRequest(QStringLiteral("/article/metrics"), params);
}

QFuture<std::expected<QVariantMap, DataSourceError>> 
ExtremeDataApi::getOfficialAccountInfo(const QString &identifier)
{
    QVariantMap params = buildCommonParams();
    params[QStringLiteral("identifier")] = identifier;
    
    return executeRequest(QStringLiteral("/official-account/info"), params);
}

void ExtremeDataApi::setProvinceCode(const QString &provinceCode)
{
    m_provinceCode = provinceCode;
    qInfo() << "[极致了] Province code set to:" << provinceCode;
}

void ExtremeDataApi::setDeviceFingerprint(const QString &deviceId, const QString &osVersion)
{
    m_deviceId = deviceId;
    m_osVersion = osVersion;
    qInfo() << "[极致了] Device fingerprint updated";
}

void ExtremeDataApi::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    --m_requestCount;
    
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "[极致了] Network error:" << reply->errorString();
        setError(DataSourceError::NetworkError, reply->errorString());
        return;
    }
    
    const QByteArray data = reply->readAll();
    // Process response data here
    qDebug() << "[极致了] Response received:" << data.size() << "bytes";
}

QFuture<std::expected<QVariantList, DataSourceError>> 
ExtremeDataApi::executeSearch(const QString &endpoint, const QVariantMap &params)
{
    return QtConcurrent::run([this, endpoint, params]() -> std::expected<QVariantList, DataSourceError> {
        if (!checkRateLimit()) {
            return std::unexpected(DataSourceError::RateLimitExceeded);
        }
        
        // Simulate API call - in production, this would make actual HTTP request
        Q_UNUSED(endpoint)
        Q_UNUSED(params)
        
        // Placeholder implementation
        QVariantList results;
        return results;
    });
}

QFuture<std::expected<QVariantMap, DataSourceError>> 
ExtremeDataApi::executeRequest(const QString &endpoint, const QVariantMap &params)
{
    return QtConcurrent::run([this, endpoint, params]() -> std::expected<QVariantMap, DataSourceError> {
        if (!checkRateLimit()) {
            return std::unexpected(DataSourceError::RateLimitExceeded);
        }
        
        Q_UNUSED(endpoint)
        Q_UNUSED(params)
        
        // Placeholder implementation
        QVariantMap result;
        return result;
    });
}

QByteArray ExtremeDataApi::generateDeviceFingerprint() const
{
    // Generate a realistic device fingerprint
    return QCryptographicHash::hash(
        QUuid::createUuid().toByteArray(QUuid::WithoutBraces),
        QCryptographicHash::Sha256);
}

QVariantMap ExtremeDataApi::buildCommonParams() const
{
    QVariantMap params;
    params[QStringLiteral("api_key")] = m_apiKey;
    params[QStringLiteral("device_id")] = m_deviceId;
    params[QStringLiteral("os_version")] = m_osVersion;
    
    if (!m_provinceCode.isEmpty()) {
        params[QStringLiteral("province_code")] = m_provinceCode;
    }
    
    params[QStringLiteral("timestamp")] = QDateTime::currentSecsSinceEpoch();
    params[QStringLiteral("request_id")] = ++d->requestId;
    
    return params;
}

bool ExtremeDataApi::checkRateLimit()
{
    // Simple rate limiting - 100 requests per minute
    constexpr int kMaxRequestsPerMinute = 100;
    
    if (m_requestCount >= kMaxRequestsPerMinute) {
        qWarning() << "[极致了] Rate limit exceeded";
        return false;
    }
    
    ++m_requestCount;
    return true;
}

} // namespace mva::api
