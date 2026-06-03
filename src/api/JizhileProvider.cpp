#include "JizhileProvider.h"
#include "../utils/logger.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QEventLoop>
#include <QTimer>
#include <stdexcept>

/**
 * @brief 构造函数实现
 *        Constructor implementation
 */
JizhileProvider::JizhileProvider(QObject* parent)
    : DataSourceBase(parent)
    , m_base_url("https://api.jizhile.com/v1") {
    Logger::info("JizhileProvider initialized", "极致了提供者已初始化");
}

/**
 * @brief 析构函数实现
 *        Destructor implementation
 */
JizhileProvider::~JizhileProvider() {
    Logger::info("JizhileProvider destroyed", "极致了提供者已销毁");
}

/**
 * @brief 获取提供者名称
 *        Get provider name
 */
QString JizhileProvider::name() const {
    return "Jizhile";
}

/**
 * @brief 获取提供者描述
 *        Get provider description
 */
QString JizhileProvider::description() const {
    return tr("极致了数据 - 通过设备集群真实采集多平台爆款内容数据\n"
              "Jizhile Data - Real-time viral content data collection via device clusters");
}

/**
 * @brief 获取支持的平台列表
 *        Get list of supported platforms
 */
QStringList JizhileProvider::supportedPlatforms() const {
    return {
        "wechat",      // 微信公众号
        "douyin",      // 抖音
        "xiaohongshu", // 小红书
        "weibo",       // 微博
        "kuaishou",    // 快手
        "bilibili",    // B 站
        "toutiao",     // 今日头条
        "zhihu"        // 知乎
    };
}

/**
 * @brief 转换为 JSON 格式
 *        Convert to JSON format
 */
QJsonObject JizhileConfig::toJson() const {
    QJsonObject json;
    
    // 基础鉴权 / Basic Auth
    json["bearer_token"] = QString::fromStdString(bearer_token);
    json["timeout_seconds"] = timeout_seconds;
    json["max_retries"] = max_retries;
    
    // 支持的平台 / Supported Platforms
    QJsonArray platforms;
    for (const auto& p : enabled_platforms) {
        switch (p) {
            case Platform::WeChat: platforms.append("wechat"); break;
            case Platform::Douyin: platforms.append("douyin"); break;
            case Platform::Xiaohongshu: platforms.append("xiaohongshu"); break;
            case Platform::Weibo: platforms.append("weibo"); break;
            case Platform::Kuaishou: platforms.append("kuaishou"); break;
            case Platform::Bilibili: platforms.append("bilibili"); break;
            case Platform::Toutiao: platforms.append("toutiao"); break;
            case Platform::Zhihu: platforms.append("zhihu"); break;
            default: break;
        }
    }
    json["enabled_platforms"] = platforms;
    
    // 设备配置 / Device Config
    switch (preferred_device) {
        case DeviceType::iOS: json["preferred_device"] = "ios"; break;
        case DeviceType::Android: json["preferred_device"] = "android"; break;
        case DeviceType::PC_Web: json["preferred_device"] = "pc_web"; break;
        case DeviceType::Mobile_Web: json["preferred_device"] = "mobile_web"; break;
    }
    json["os_version_filter"] = QString::fromStdString(os_version_filter);
    json["device_brand_filter"] = QString::fromStdString(device_brand_filter);
    
    // 地域定向 / Geo-Targeting
    QJsonArray provinces, cities;
    for (const auto& p : target_provinces) provinces.append(QString::fromStdString(p));
    for (const auto& c : target_cities) cities.append(QString::fromStdString(c));
    json["target_provinces"] = provinces;
    json["target_cities"] = cities;
    json["exclude_abnormal_traffic"] = exclude_abnormal_traffic;
    
    // 时间粒度 / Time Granularity
    switch (time_granularity) {
        case TimeGranularity::Hourly: json["time_granularity"] = "hourly"; break;
        case TimeGranularity::Daily: json["time_granularity"] = "daily"; break;
        case TimeGranularity::Weekly: json["time_granularity"] = "weekly"; break;
        case TimeGranularity::Monthly: json["time_granularity"] = "monthly"; break;
    }
    json["history_days"] = history_days;
    
    // 指标筛选 / Metrics Filter
    QJsonObject metrics;
    metrics["min_read_count"] = metrics_filter.min_read_count;
    metrics["min_like_count"] = metrics_filter.min_like_count;
    metrics["min_comment_count"] = metrics_filter.min_comment_count;
    metrics["min_share_count"] = metrics_filter.min_share_count;
    metrics["min_collect_count"] = metrics_filter.min_collect_count;
    metrics["min_engagement_rate"] = metrics_filter.min_engagement_rate;
    json["metrics_filter"] = metrics;
    
    // 高级选项 / Advanced Options
    json["enable_real_time_monitoring"] = enable_real_time_monitoring;
    json["real_time_interval_seconds"] = real_time_interval_seconds;
    json["custom_user_agent"] = QString::fromStdString(custom_user_agent);
    json["enable_cache"] = enable_cache;
    json["cache_ttl_hours"] = cache_ttl_hours;
    
    return json;
}

/**
 * @brief 从 JSON 加载配置
 *        Load configuration from JSON
 */
bool JizhileConfig::fromJson(const QJsonObject& json) {
    try {
        // 基础鉴权
        if (json.contains("bearer_token"))
            bearer_token = json["bearer_token"].toString().toStdString();
        if (json.contains("timeout_seconds"))
            timeout_seconds = json["timeout_seconds"].toInt();
        if (json.contains("max_retries"))
            max_retries = json["max_retries"].toInt();
        
        // 平台列表
        if (json.contains("enabled_platforms")) {
            enabled_platforms.clear();
            for (const auto& p : json["enabled_platforms"].toArray()) {
                QString platform = p.toString();
                if (platform == "wechat") enabled_platforms.push_back(Platform::WeChat);
                else if (platform == "douyin") enabled_platforms.push_back(Platform::Douyin);
                else if (platform == "xiaohongshu") enabled_platforms.push_back(Platform::Xiaohongshu);
                else if (platform == "weibo") enabled_platforms.push_back(Platform::Weibo);
                else if (platform == "kuaishou") enabled_platforms.push_back(Platform::Kuaishou);
                else if (platform == "bilibili") enabled_platforms.push_back(Platform::Bilibili);
                else if (platform == "toutiao") enabled_platforms.push_back(Platform::Toutiao);
                else if (platform == "zhihu") enabled_platforms.push_back(Platform::Zhihu);
            }
        }
        
        // 设备配置
        if (json.contains("preferred_device")) {
            QString device = json["preferred_device"].toString();
            if (device == "ios") preferred_device = DeviceType::iOS;
            else if (device == "android") preferred_device = DeviceType::Android;
            else if (device == "pc_web") preferred_device = DeviceType::PC_Web;
            else if (device == "mobile_web") preferred_device = DeviceType::Mobile_Web;
        }
        if (json.contains("os_version_filter"))
            os_version_filter = json["os_version_filter"].toString().toStdString();
        if (json.contains("device_brand_filter"))
            device_brand_filter = json["device_brand_filter"].toString().toStdString();
        
        // 地域定向
        if (json.contains("target_provinces")) {
            target_provinces.clear();
            for (const auto& p : json["target_provinces"].toArray())
                target_provinces.push_back(p.toString().toStdString());
        }
        if (json.contains("target_cities")) {
            target_cities.clear();
            for (const auto& c : json["target_cities"].toArray())
                target_cities.push_back(c.toString().toStdString());
        }
        if (json.contains("exclude_abnormal_traffic"))
            exclude_abnormal_traffic = json["exclude_abnormal_traffic"].toBool();
        
        // 时间粒度
        if (json.contains("time_granularity")) {
            QString granularity = json["time_granularity"].toString();
            if (granularity == "hourly") time_granularity = TimeGranularity::Hourly;
            else if (granularity == "daily") time_granularity = TimeGranularity::Daily;
            else if (granularity == "weekly") time_granularity = TimeGranularity::Weekly;
            else if (granularity == "monthly") time_granularity = TimeGranularity::Monthly;
        }
        if (json.contains("history_days"))
            history_days = json["history_days"].toInt();
        
        // 指标筛选
        if (json.contains("metrics_filter")) {
            QJsonObject metrics = json["metrics_filter"].toObject();
            metrics_filter.min_read_count = metrics["min_read_count"].toInt();
            metrics_filter.min_like_count = metrics["min_like_count"].toInt();
            metrics_filter.min_comment_count = metrics["min_comment_count"].toInt();
            metrics_filter.min_share_count = metrics["min_share_count"].toInt();
            metrics_filter.min_collect_count = metrics["min_collect_count"].toInt();
            metrics_filter.min_engagement_rate = metrics["min_engagement_rate"].toDouble();
        }
        
        // 高级选项
        if (json.contains("enable_real_time_monitoring"))
            enable_real_time_monitoring = json["enable_real_time_monitoring"].toBool();
        if (json.contains("real_time_interval_seconds"))
            real_time_interval_seconds = json["real_time_interval_seconds"].toInt();
        if (json.contains("custom_user_agent"))
            custom_user_agent = json["custom_user_agent"].toString().toStdString();
        if (json.contains("enable_cache"))
            enable_cache = json["enable_cache"].toBool();
        if (json.contains("cache_ttl_hours"))
            cache_ttl_hours = json["cache_ttl_hours"].toInt();
        
        return true;
    } catch (const std::exception& e) {
        Logger::error(std::string("Failed to parse Jizhile config: ") + e.what(),
                      "解析极致了配置失败");
        return false;
    }
}

/**
 * @brief 初始化数据源
 *        Initialize the data source
 */
co_future<std::expected<void, JizhileProvider::ErrorInfo>> 
JizhileProvider::initialize(const QVariantMap& config) {
    QMutexLocker locker(&m_mutex);
    
    try {
        // 转换为 JSON 再解析
        QJsonObject json = QJsonObject::fromVariantMap(config);
        if (!m_config.fromJson(json)) {
            co_return std::unexpected(ErrorInfo{
                .code = ErrorCode::CONFIG_ERROR,
                .message = "Failed to parse configuration",
                .details = "配置解析失败"
            });
        }
        
        if (m_config.bearer_token.empty()) {
            co_return std::unexpected(ErrorInfo{
                .code = ErrorCode::AUTH_ERROR,
                .message = "Bearer token is required",
                .details = "缺少 Bearer Token"
            });
        }
        
        Logger::info("Jizhile provider initialized with " + 
                     std::to_string(m_config.enabled_platforms.size()) + 
                     " platforms",
                     "极致了提供者已初始化，启用平台数：" + 
                     std::to_string(m_config.enabled_platforms.size()));
        
        co_return {};
    } catch (const std::exception& e) {
        co_return std::unexpected(ErrorInfo{
            .code = ErrorCode::INIT_ERROR,
            .message = e.what(),
            .details = "初始化失败"
        });
    }
}

/**
 * @brief 解析平台字符串
 *        Parse platform string
 */
JizhileConfig::Platform JizhileProvider::parsePlatform(const QString& platform) {
    if (platform == "wechat") return JizhileConfig::Platform::WeChat;
    if (platform == "douyin") return JizhileConfig::Platform::Douyin;
    if (platform == "xiaohongshu") return JizhileConfig::Platform::Xiaohongshu;
    if (platform == "weibo") return JizhileConfig::Platform::Weibo;
    if (platform == "kuaishou") return JizhileConfig::Platform::Kuaishou;
    if (platform == "bilibili") return JizhileConfig::Platform::Bilibili;
    if (platform == "toutiao") return JizhileConfig::Platform::Toutiao;
    if (platform == "zhihu") return JizhileConfig::Platform::Zhihu;
    return JizhileConfig::Platform::Unknown;
}

/**
 * @brief 转换平台枚举为字符串
 *        Convert platform enum to string
 */
QString JizhileProvider::platformToString(JizhileConfig::Platform platform) {
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

/**
 * @brief 构建请求头
 *        Build request headers
 */
QMap<QString, QString> JizhileProvider::buildHeaders() const {
    QMap<QString, QString> headers;
    headers["Authorization"] = QString("Bearer %1").arg(QString::fromStdString(m_config.bearer_token));
    headers["Content-Type"] = "application/json";
    headers["Accept"] = "application/json";
    
    if (!m_config.custom_user_agent.empty()) {
        headers["User-Agent"] = QString::fromStdString(m_config.custom_user_agent);
    } else {
        headers["User-Agent"] = "SelfMediaViralAssistant/1.0";
    }
    
    return headers;
}

/**
 * @brief 构建查询参数
 *        Build query parameters
 */
QUrlQuery JizhileProvider::buildQueryParams(const QVariantMap& params) const {
    QUrlQuery query;
    
    for (auto it = params.begin(); it != params.end(); ++it) {
        query.addQueryItem(it.key(), it.value().toString());
    }
    
    // 添加设备指纹参数
    switch (m_config.preferred_device) {
        case JizhileConfig::DeviceType::iOS:
            query.addQueryItem("device_type", "ios");
            break;
        case JizhileConfig::DeviceType::Android:
            query.addQueryItem("device_type", "android");
            break;
        case JizhileConfig::DeviceType::PC_Web:
            query.addQueryItem("device_type", "pc");
            break;
        case JizhileConfig::DeviceType::Mobile_Web:
            query.addQueryItem("device_type", "mobile");
            break;
    }
    
    if (!m_config.os_version_filter.empty()) {
        query.addQueryItem("os_version", QString::fromStdString(m_config.os_version_filter));
    }
    
    if (!m_config.device_brand_filter.empty()) {
        query.addQueryItem("device_brand", QString::fromStdString(m_config.device_brand_filter));
    }
    
    // 地域参数
    if (!m_config.target_provinces.empty()) {
        QStringList provinces;
        for (const auto& p : m_config.target_provinces) {
            provinces << QString::fromStdString(p);
        }
        query.addQueryItem("provinces", provinces.join(","));
    }
    
    if (!m_config.target_cities.empty()) {
        QStringList cities;
        for (const auto& c : m_config.target_cities) {
            cities << QString::fromStdString(c);
        }
        query.addQueryItem("cities", cities.join(","));
    }
    
    query.addQueryItem("exclude_abnormal", m_config.exclude_abnormal_traffic ? "1" : "0");
    
    return query;
}

/**
 * @brief 发送 HTTP 请求
 *        Send HTTP request
 */
co_future<std::expected<QByteArray, JizhileProvider::ErrorInfo>>
JizhileProvider::sendRequest(const QUrl& url, 
                              const QString& method,
                              const QByteArray& body) const {
    // 注意：实际实现需要使用异步网络请求
    // 这里使用简化的同步模拟
    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    
    auto headers = buildHeaders();
    for (auto it = headers.begin(); it != headers.end(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    
    QNetworkReply* reply;
    if (method == "POST") {
        reply = manager.post(request, body.isEmpty() ? QByteArray("{}") : body);
    } else {
        reply = manager.get(request);
    }
    
    // 等待完成（实际应使用异步）
    QEventLoop loop;
    QTimer::singleShot(m_config.timeout_seconds * 1000, &loop, &loop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    
    if (reply->error() != QNetworkReply::NoError) {
        co_return std::unexpected(ErrorInfo{
            .code = ErrorCode::NETWORK_ERROR,
            .message = reply->errorString().toStdString(),
            .details = "网络请求失败"
        });
    }
    
    QByteArray response = reply->readAll();
    reply->deleteLater();
    
    co_return response;
}

// 其他方法实现略（discoverViralContent, fetchContentMetrics 等）
// 在实际项目中需要完整实现所有接口

co_future<std::expected<QList<JizhileProvider::ContentItem>, JizhileProvider::ErrorInfo>>
JizhileProvider::discoverViralContent(const QString& platform,
                                       const QString& category,
                                       int time_range,
                                       int limit) {
    // TODO: 实现完整的爆款发现逻辑
    Q_UNUSED(platform)
    Q_UNUSED(category)
    Q_UNUSED(time_range)
    Q_UNUSED(limit)
    
    QList<ContentItem> items;
    // 模拟返回空列表
    co_return items;
}

co_future<std::expected<JizhileProvider::ContentMetrics, JizhileProvider::ErrorInfo>>
JizhileProvider::fetchContentMetrics(const QString& content_id,
                                      const QString& platform) {
    Q_UNUSED(content_id)
    Q_UNUSED(platform)
    co_return std::unexpected(ErrorInfo{
        .code = ErrorCode::NOT_IMPLEMENTED,
        .message = "Not implemented yet",
        .details = "功能尚未实现"
    });
}

co_future<std::expected<JizhileProvider::ContentDetail, JizhileProvider::ErrorInfo>>
JizhileProvider::fetchContentDetail(const QString& content_id,
                                     const QString& platform) {
    Q_UNUSED(content_id)
    Q_UNUSED(platform)
    co_return std::unexpected(ErrorInfo{
        .code = ErrorCode::NOT_IMPLEMENTED,
        .message = "Not implemented yet",
        .details = "功能尚未实现"
    });
}

co_future<std::expected<QList<JizhileProvider::Comment>, JizhileProvider::ErrorInfo>>
JizhileProvider::fetchComments(const QString& content_id,
                                const QString& platform,
                                int limit,
                                const QString& sort_by) {
    Q_UNUSED(content_id)
    Q_UNUSED(platform)
    Q_UNUSED(limit)
    Q_UNUSED(sort_by)
    co_return std::unexpected(ErrorInfo{
        .code = ErrorCode::NOT_IMPLEMENTED,
        .message = "Not implemented yet",
        .details = "功能尚未实现"
    });
}

bool JizhileProvider::updateConfig(const QVariantMap& config) {
    QMutexLocker locker(&m_mutex);
    QJsonObject json = QJsonObject::fromVariantMap(config);
    if (m_config.fromJson(json)) {
        emit configUpdated(config);
        return true;
    }
    return false;
}

QVariantMap JizhileProvider::getConfig() const {
    QMutexLocker locker(&m_mutex);
    return m_config.toJson().toVariantMap();
}

co_future<std::expected<bool, JizhileProvider::ErrorInfo>>
JizhileProvider::testConnection() {
    // TODO: 实现连接测试
    co_return true;
}
