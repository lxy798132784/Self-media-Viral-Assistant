/**
 * @file JizhileProvider.cpp
 * @brief 极致了数据 API 插件完整实现
 *        Jizhile Data API Plugin Full Implementation
 */

#include "JizhileProvider.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent>
#include <QEventLoop>
#include <QTimer>
#include <QUuid>

namespace mva::api {

// ============================================================================
// JizhileConfig Implementation
// ============================================================================

QJsonObject JizhileConfig::toJson() const
{
    QJsonObject json;
    json["bearer_token"] = bearer_token;
    json["timeout_seconds"] = timeout_seconds;
    json["max_retries"] = max_retries;
    
    QJsonArray platforms;
    for (Platform p : enabled_platforms) {
        platforms.append(static_cast<int>(p));
    }
    json["enabled_platforms"] = platforms;
    
    json["preferred_device"] = static_cast<int>(preferred_device);
    json["os_version_filter"] = os_version_filter;
    json["device_brand_filter"] = device_brand_filter;
    
    QJsonArray provinces, cities;
    for (const QString& p : target_provinces) provinces.append(p);
    for (const QString& c : target_cities) cities.append(c);
    json["target_provinces"] = provinces;
    json["target_cities"] = cities;
    
    json["exclude_abnormal_traffic"] = exclude_abnormal_traffic;
    json["time_granularity"] = static_cast<int>(time_granularity);
    json["history_days"] = history_days;
    
    QJsonObject metricsFilter;
    metricsFilter["min_read_count"] = metrics_filter.min_read_count;
    metricsFilter["min_like_count"] = metrics_filter.min_like_count;
    metricsFilter["min_comment_count"] = metrics_filter.min_comment_count;
    metricsFilter["min_share_count"] = metrics_filter.min_share_count;
    metricsFilter["min_collect_count"] = metrics_filter.min_collect_count;
    metricsFilter["min_engagement_rate"] = metrics_filter.min_engagement_rate;
    json["metrics_filter"] = metricsFilter;
    
    json["enable_real_time_monitoring"] = enable_real_time_monitoring;
    json["real_time_interval_seconds"] = real_time_interval_seconds;
    json["custom_user_agent"] = custom_user_agent;
    json["enable_cache"] = enable_cache;
    json["cache_ttl_hours"] = cache_ttl_hours;
    
    return json;
}

bool JizhileConfig::fromJson(const QJsonObject& json)
{
    if (json.isEmpty()) return false;
    
    bearer_token = json["bearer_token"].toString();
    timeout_seconds = json["timeout_seconds"].toInt(30);
    max_retries = json["max_retries"].toInt(3);
    
    if (json.contains("enabled_platforms")) {
        enabled_platforms.clear();
        for (const QJsonValue& v : json["enabled_platforms"].toArray()) {
            enabled_platforms.append(static_cast<Platform>(v.toInt()));
        }
    }
    
    preferred_device = static_cast<DeviceType>(json["preferred_device"].toInt(0));
    os_version_filter = json["os_version_filter"].toString();
    device_brand_filter = json["device_brand_filter"].toString();
    
    if (json.contains("target_provinces")) {
        target_provinces.clear();
        for (const QJsonValue& v : json["target_provinces"].toArray()) {
            target_provinces.append(v.toString());
        }
    }
    if (json.contains("target_cities")) {
        target_cities.clear();
        for (const QJsonValue& v : json["target_cities"].toArray()) {
            target_cities.append(v.toString());
        }
    }
    
    exclude_abnormal_traffic = json["exclude_abnormal_traffic"].toBool(true);
    time_granularity = static_cast<TimeGranularity>(json["time_granularity"].toInt(1));
    history_days = json["history_days"].toInt(30);
    
    if (json.contains("metrics_filter")) {
        QJsonObject mf = json["metrics_filter"].toObject();
        metrics_filter.min_read_count = mf["min_read_count"].toInt(0);
        metrics_filter.min_like_count = mf["min_like_count"].toInt(0);
        metrics_filter.min_comment_count = mf["min_comment_count"].toInt(0);
        metrics_filter.min_share_count = mf["min_share_count"].toInt(0);
        metrics_filter.min_collect_count = mf["min_collect_count"].toInt(0);
        metrics_filter.min_engagement_rate = mf["min_engagement_rate"].toDouble(0.0);
    }
    
    enable_real_time_monitoring = json["enable_real_time_monitoring"].toBool(false);
    real_time_interval_seconds = json["real_time_interval_seconds"].toInt(300);
    custom_user_agent = json["custom_user_agent"].toString();
    enable_cache = json["enable_cache"].toBool(true);
    cache_ttl_hours = json["cache_ttl_hours"].toInt(24);
    
    return true;
}

// ============================================================================
// JizhileProvider Implementation
// ============================================================================

JizhileProvider::JizhileProvider(QObject* parent)
    : DataSourceBase(parent)
    , m_base_url("https://api.jizhile.com/v1")
    , m_network(new QNetworkAccessManager(this))
    , m_initialized(false)
    , m_remaining_quota(-1)
{
    connect(m_network, &QNetworkAccessManager::finished,
            this, &JizhileProvider::onNetworkReplyFinished);
}

JizhileProvider::~JizhileProvider()
{
    m_network->deleteLater();
}

QString JizhileProvider::id() const { return QStringLiteral("jizhile"); }

QString JizhileProvider::displayName() const { return QStringLiteral("极致了"); }

QStringList JizhileProvider::supportedPlatforms() const
{
    return {
        QStringLiteral("wechat_channels"), QStringLiteral("wechat_official"),
        QStringLiteral("douyin"), QStringLiteral("xiaohongshu"),
        QStringLiteral("weibo"), QStringLiteral("kuaishou"),
        QStringLiteral("bilibili"), QStringLiteral("toutiao"),
        QStringLiteral("zhihu")
    };
}

bool JizhileProvider::initialize(const QVariantMap& config)
{
    QMutexLocker locker(&m_mutex);
    
    if (!config.contains("bearer_token")) {
        qWarning() << "JizhileProvider: Missing bearer_token";
        return false;
    }
    
    m_config.bearer_token = config["bearer_token"].toString();
    if (config.contains("timeout_seconds")) m_config.timeout_seconds = config["timeout_seconds"].toInt();
    if (config.contains("max_retries")) m_config.max_retries = config["max_retries"].toInt();
    if (config.contains("base_url")) m_base_url = config["base_url"].toString();
    
    if (config.contains("enabled_platforms")) {
        m_config.enabled_platforms.clear();
        for (const QString& p : config["enabled_platforms"].toStringList()) {
            m_config.enabled_platforms.append(parsePlatform(p));
        }
    }
    
    if (config.contains("target_provinces")) 
        m_config.target_provinces = config["target_provinces"].toStringList();
    if (config.contains("target_cities")) 
        m_config.target_cities = config["target_cities"].toStringList();
    
    m_initialized = true;
    emit configUpdated(getConfig());
    return true;
}

QFuture<MetricsResult> JizhileProvider::fetchMetrics(const QString& url, const QString& platform)
{
    return QtConcurrent::run([=]() -> MetricsResult {
        QUrl apiUrl(m_base_url + "/content/metrics");
        QUrlQuery query;
        query.addQueryItem("url", url);
        query.addQueryItem("platform", platform);
        apiUrl.setQuery(query);
        
        QNetworkRequest request = buildRequest(apiUrl.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.type != ApiError::NoError) return std::unexpected(error);
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            ApiErrorInfo err{ApiError::ParseError, "Failed to parse JSON", 0, "jizhile"};
            return std::unexpected(err);
        }
        
        QJsonObject root = doc.object();
        int code = root.value("code").toInt(-1);
        if (code != 200) {
            ApiErrorInfo err{ApiError::ServerError, root.value("message").toString(), code, "jizhile"};
            return std::unexpected(err);
        }
        
        Metrics metrics;
        QJsonObject dataObj = root.value("data").toObject();
        metrics.views = dataObj.value("views").toVariant().toLongLong(0);
        metrics.likes = dataObj.value("likes").toVariant().toLongLong(0);
        metrics.comments = dataObj.value("comments").toVariant().toLongLong(0);
        metrics.shares = dataObj.value("shares").toVariant().toLongLong(0);
        metrics.favorites = dataObj.value("favorites").toVariant().toLongLong(0);
        
        qreal total = metrics.likes + metrics.comments + metrics.shares + metrics.favorites;
        if (metrics.views > 0) metrics.engagementRate = (total / static_cast<qreal>(metrics.views)) * 100.0;
        
        metrics.publishTime = QDateTime::fromString(dataObj.value("publish_time").toString(), Qt::ISODate);
        metrics.fetchTime = QDateTime::currentDateTime();
        
        return metrics;
    });
}

QFuture<ContentResult> JizhileProvider::fetchContent(const QString& url, const QString& platform, bool includeComments)
{
    return QtConcurrent::run([=]() -> ContentResult {
        QUrl apiUrl(m_base_url + "/content/detail");
        QUrlQuery query;
        query.addQueryItem("url", url);
        query.addQueryItem("platform", platform);
        if (includeComments) query.addQueryItem("include_comments", "true");
        apiUrl.setQuery(query);
        
        QNetworkRequest request = buildRequest(apiUrl.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.type != ApiError::NoError) return std::unexpected(error);
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            ApiErrorInfo err{ApiError::ParseError, "Failed to parse JSON", 0, "jizhile"};
            return std::unexpected(err);
        }
        
        QJsonObject root = doc.object();
        int code = root.value("code").toInt(-1);
        if (code != 200) {
            ApiErrorInfo err{ApiError::ServerError, root.value("message").toString(), code, "jizhile"};
            return std::unexpected(err);
        }
        
        ArticleContent content;
        QJsonObject dataObj = root.value("data").toObject();
        content.title = dataObj.value("title").toString();
        content.body = dataObj.value("content").toString();
        content.description = dataObj.value("description").toString();
        content.author = dataObj.value("author").toString();
        content.videoUrl = dataObj.value("video_url").toString();
        
        for (const QJsonValue& img : dataObj.value("images").toArray())
            content.images.append(img.toString());
        for (const QJsonValue& tag : dataObj.value("tags").toArray())
            content.tags.append(tag.toString());
        
        content.extra = dataObj.toVariantMap();
        return content;
    });
}

bool JizhileProvider::isAvailable() const { return m_initialized && !m_config.bearer_token.isEmpty(); }

int JizhileProvider::getRemainingQuota() const { return m_remaining_quota; }

bool JizhileProvider::updateConfig(const QVariantMap& config)
{
    QMutexLocker locker(&m_mutex);
    if (config.contains("bearer_token")) m_config.bearer_token = config["bearer_token"].toString();
    if (config.contains("timeout_seconds")) m_config.timeout_seconds = config["timeout_seconds"].toInt();
    if (config.contains("max_retries")) m_config.max_retries = config["max_retries"].toInt();
    emit configUpdated(getConfig());
    return true;
}

QVariantMap JizhileProvider::getConfig() const
{
    QVariantMap config;
    config["bearer_token"] = m_config.bearer_token;
    config["timeout_seconds"] = m_config.timeout_seconds;
    config["max_retries"] = m_config.max_retries;
    config["base_url"] = m_base_url;
    config["initialized"] = m_initialized;
    config["remaining_quota"] = m_remaining_quota;
    return config;
}

void JizhileProvider::setToken(const QString& token)
{
    QMutexLocker locker(&m_mutex);
    m_config.bearer_token = token;
}

QMap<QString, QString> JizhileProvider::buildHeaders() const
{
    QMap<QString, QString> headers;
    headers["Content-Type"] = "application/json";
    headers["User-Agent"] = m_config.custom_user_agent.isEmpty() ? "MediaViralAssistant/1.0" : m_config.custom_user_agent;
    if (!m_config.bearer_token.isEmpty()) headers["Authorization"] = "Bearer " + m_config.bearer_token;
    headers["X-Device-ID"] = QUuid::createUuid().toString(QUuid::WithoutBraces);
    headers["X-OS-Version"] = "iOS 17.0";
    return headers;
}

QUrlQuery JizhileProvider::buildQueryParams(const QVariantMap& params) const
{
    QUrlQuery query;
    for (auto it = params.begin(); it != params.end(); ++it)
        query.addQueryItem(it.key(), it.value().toString());
    if (!m_config.target_provinces.isEmpty()) query.addQueryItem("province_code", m_config.target_provinces.first());
    if (!m_config.target_cities.isEmpty()) query.addQueryItem("city_code", m_config.target_cities.first());
    return query;
}

QNetworkRequest JizhileProvider::buildRequest(const QString& endpointUrl, const QVariantMap& params)
{
    QUrl url(endpointUrl);
    if (!params.isEmpty()) url.setQuery(buildQueryParams(params));
    QNetworkRequest request(url);
    for (auto& h : buildHeaders().toStdMap())
        request.setRawHeader(h.first.toUtf8(), h.second.toUtf8());
    return request;
}

QPair<QByteArray, ApiErrorInfo> JizhileProvider::sendRequest(const QNetworkRequest& request)
{
    QNetworkReply* reply = m_network->get(request);
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&timeoutTimer, &QTimer::timeout, [&]() { if (reply->isRunning()) reply->abort(); loop.quit(); });
    
    timeoutTimer.start(m_config.timeout_seconds * 1000);
    loop.exec();
    
    if (reply->error() != QNetworkReply::NoError) {
        ApiErrorInfo error = parseError(reply);
        reply->deleteLater();
        return {QByteArray(), error};
    }
    
    QByteArray data = reply->readAll();
    reply->deleteLater();
    return {data, ApiErrorInfo{}};
}

ApiErrorInfo JizhileProvider::parseError(QNetworkReply* reply)
{
    ApiErrorInfo error{ApiError::Unknown, {}, 0, "jizhile"};
    if (!reply) { error.message = "Null reply"; return error; }
    
    switch (reply->error()) {
        case QNetworkReply::NoError: error.type = ApiError::NoError; break;
        case QNetworkReply::AuthenticationRequiredError:
        case QNetworkReply::AccessDenied: error.type = ApiError::AuthenticationError; error.message = "Auth failed"; break;
        case QNetworkReply::ContentNotFoundError:
        case QNetworkReply::HostNotFoundError: error.type = ApiError::NotFound; error.message = reply->errorString(); break;
        case QNetworkReply::TimeoutError:
        case QNetworkReply::OperationCanceledError: error.type = ApiError::Timeout; error.message = "Timeout"; break;
        case QNetworkReply::TooManyRedirectsError:
        case QNetworkReply::TemporaryServerFailureError: error.type = ApiError::RateLimitExceeded; error.message = "Rate limit"; break;
        default:
            if (reply->error() < 200) error.type = ApiError::NetworkError;
            else if (reply->error() < 500) error.type = ApiError::ValidationError;
            else error.type = ApiError::ServerError;
            error.message = reply->errorString();
    }
    
    error.httpCode = static_cast<int>(reply->error());
    QByteArray data = reply->readAll();
    if (!data.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull()) {
            QJsonObject root = doc.object();
            if (root.contains("message")) error.message = root.value("message").toString();
        }
    }
    return error;
}

JizhileConfig::Platform JizhileProvider::parsePlatform(const QString& platform)
{
    if (platform.contains("wechat") || platform.contains("shipinhao")) return JizhileConfig::Platform::WeChat;
    if (platform == "douyin") return JizhileConfig::Platform::Douyin;
    if (platform.contains("xiaohongshu") || platform == "red") return JizhileConfig::Platform::Xiaohongshu;
    if (platform == "weibo") return JizhileConfig::Platform::Weibo;
    if (platform == "kuaishou") return JizhileConfig::Platform::Kuaishou;
    if (platform.contains("bilibili") || platform.contains("bstation")) return JizhileConfig::Platform::Bilibili;
    if (platform == "toutiao") return JizhileConfig::Platform::Toutiao;
    if (platform == "zhihu") return JizhileConfig::Platform::Zhihu;
    return JizhileConfig::Platform::Unknown;
}

QString JizhileProvider::platformToString(JizhileConfig::Platform platform)
{
    switch (platform) {
        case JizhileConfig::Platform::WeChat: return "wechat";
        case JizhileConfig::Platform::Douyin: return "douyin";
        case JizhileConfig::Platform::Xiaohongshu: return "xiaohongshu";
        case JizhileConfig::Platform::Weibo: return "weibo";
        case JizhileConfig::Platform::Kuaishou: return "kuaishou";
        case JizhileConfig::Platform::Bilibili: return "bilibili";
        case JizhileConfig::Platform::Toutiao: return "toutiao";
        case JizhileConfig::Platform::Zhihu: return "zhihu";
        default: return "unknown";
    }
}

void JizhileProvider::onNetworkReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        qWarning() << "JizhileProvider: Network error:" << reply->errorString();
    if (reply->hasRawHeader("X-RateLimit-Remaining")) {
        m_remaining_quota = reply->rawHeader("X-RateLimit-Remaining").toInt();
        emit quotaUpdated(m_remaining_quota, -1);
    }
    reply->deleteLater();
}

} // namespace mva::api
