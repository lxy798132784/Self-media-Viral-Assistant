#ifndef JIZHILE_PROVIDER_H
#define JIZHILE_PROVIDER_H

#include "data_source_base.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <vector>
#include <string>

/**
 * @brief 极致了数据源配置参数结构体
 *        Jizhile Data Source Configuration Parameters
 *
 * @details 包含极致了平台支持的所有可配置参数。
 *          极致了通过设备集群真实模拟用户访问，因此支持详细的设备指纹和地域定向。
 *          Contains all configurable parameters supported by Jizhile platform.
 *          Jizhile simulates real user access via device clusters, 
 *          thus supporting detailed device fingerprints and geo-targeting.
 *
 * @note 所有参数均支持热更新，无需重启应用
 *       All parameters support hot-updates without restarting the application
 */
struct JizhileConfig {
    // === 基础鉴权 / Basic Authentication ===
    QString bearer_token;              ///< API 访问令牌 / API Access Token
    int timeout_seconds = 30;          ///< 请求超时时间 (秒) / Request timeout (seconds)
    int max_retries = 3;               ///< 最大重试次数 / Max retry count
    
    // === 支持的平台 / Supported Platforms ===
    enum class Platform {
        WeChat,        ///< 微信公众号 / WeChat Official Accounts
        Douyin,        ///< 抖音 / Douyin (TikTok CN)
        Xiaohongshu,   ///< 小红书 / Xiaohongshu (RED)
        Weibo,         ///< 微博 / Weibo
        Kuaishou,      ///< 快手 / Kuaishou
        Bilibili,      ///< B 站 / Bilibili
        Toutiao,       ///< 今日头条 / Toutiao
        Zhihu,         ///< 知乎 / Zhihu
        Unknown        ///< 未知平台 / Unknown platform
    };
    QList<Platform> enabled_platforms;  ///< 启用的平台列表 / List of enabled platforms
    
    // === 设备集群配置 / Device Cluster Configuration ===
    enum class DeviceType {
        iOS,           ///< iOS 设备 / iOS Device
        Android,       ///< Android 设备 / Android Device
        PC_Web,        ///< PC 网页 / PC Web
        Mobile_Web     ///< 移动网页 / Mobile Web
    };
    DeviceType preferred_device = DeviceType::iOS;  ///< 首选设备类型 / Preferred device type
    QString os_version_filter;                      ///< OS 版本过滤 (e.g., "15.0-17.0") / OS version filter
    QString device_brand_filter;                    ///< 设备品牌过滤 (e.g., "Apple,Huawei") / Device brand filter
    
    // === 地域定向 / Geo-Targeting ===
    QStringList target_provinces;      ///< 目标省份 (e.g., ["Beijing", "Shanghai"]) / Target provinces
    QStringList target_cities;         ///< 目标城市 / Target cities
    bool exclude_abnormal_traffic = true;           ///< 排除异常流量 / Exclude abnormal traffic
    
    // === 数据采集粒度 / Data Collection Granularity ===
    enum class TimeGranularity {
        Hourly,      ///< 小时级 / Hourly
        Daily,       ///< 天级 / Daily
        Weekly,      ///< 周级 / Weekly
        Monthly      ///< 月级 / Monthly
    };
    TimeGranularity time_granularity = TimeGranularity::Daily;  ///< 时间粒度 / Time granularity
    int history_days = 30;                                      ///< 历史数据天数 / History days
    
    // === 指标筛选 / Metrics Filtering ===
    struct MetricsThreshold {
        int min_read_count = 0;        ///< 最小阅读量 / Min read count
        int min_like_count = 0;        ///< 最小点赞数 / Min like count
        int min_comment_count = 0;     ///< 最小评论数 / Min comment count
        int min_share_count = 0;       ///< 最小转发数 / Min share count
        int min_collect_count = 0;     ///< 最小收藏数 / Min collect count
        double min_engagement_rate = 0.0;  ///< 最小互动率 (%) / Min engagement rate (%)
    };
    MetricsThreshold metrics_filter;   ///< 指标过滤条件 / Metrics filter conditions
    
    // === 高级选项 / Advanced Options ===
    bool enable_real_time_monitoring = false;  ///< 启用实时监控 / Enable real-time monitoring
    int real_time_interval_seconds = 300;      ///< 实时监控间隔 (秒) / Real-time interval (seconds)
    QString custom_user_agent;                 ///< 自定义 User-Agent / Custom User-Agent
    bool enable_cache = true;                  ///< 启用本地缓存 / Enable local cache
    int cache_ttl_hours = 24;                  ///< 缓存有效期 (小时) / Cache TTL (hours)
    
    /**
     * @brief 转换为 JSON 格式，用于网络请求
     *        Convert to JSON format for network requests
     * @return QJsonObject JSON 对象 / JSON object
     */
    QJsonObject toJson() const;
    
    /**
     * @brief 从 JSON 加载配置
     *        Load configuration from JSON
     * @param json JSON 对象 / JSON object
     * @return bool 加载是否成功 / Whether loading was successful
     */
    bool fromJson(const QJsonObject& json);
};

/**
 * @brief 极致了数据源提供者
 *        Jizhile Data Source Provider
 *
 * @details 实现极致了平台的 API 接口，支持多平台爆款内容发现和数据采集。
 *          通过设备集群真实模拟用户行为，获取精确的阅读量、互动量等指标。
 *          Implements Jizhile platform API interface, supporting multi-platform 
 *          viral content discovery and data collection.
 *          Accurately fetches metrics like read counts and engagement 
 *          by simulating real user behavior via device clusters.
 *
 * @extends DataSourceBase
 *
 * @threadsafe 是 / Yes
 */
class JizhileProvider : public DataSourceBase {
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     *        Constructor
     *
     * @param parent 父对象 / Parent object
     */
    explicit JizhileProvider(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     *        Destructor
     */
    ~JizhileProvider() override;
    
    /**
     * @brief 获取提供者唯一标识符
     *        Get provider unique identifier
     * 
     * @return QString 提供者 ID / Provider ID
     */
    [[nodiscard]] QString id() const override;
    
    /**
     * @brief 获取提供者显示名称
     *        Get provider display name
     * 
     * @return QString 提供者名称 / Provider name
     */
    [[nodiscard]] QString displayName() const override;
    
    /**
     * @brief 获取支持的平台列表
     *        Get list of supported platforms
     * 
     * @return QStringList 平台列表 / List of platforms
     */
    [[nodiscard]] QStringList supportedPlatforms() const override;
    
    /**
     * @brief 初始化数据源
     *        Initialize the data source
     *
     * @param config 配置参数 / Configuration parameters
     * @return bool 初始化是否成功 / Whether initialization was successful
     */
    bool initialize(const QVariantMap& config) override;
    
    /**
     * @brief 获取文章数据指标（异步）
     *        Fetch article metrics (async)
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return QFuture<MetricsResult> 异步结果 / Async result
     */
    QFuture<MetricsResult> fetchMetrics(
        const QString& url, 
        const QString& platform
    ) override;
    
    /**
     * @brief 获取文章内容详情（异步）
     *        Fetch article content details (async)
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @param includeComments 是否包含评论 / Whether to include comments
     * @return QFuture<ContentResult> 异步结果 / Async result
     */
    QFuture<ContentResult> fetchContent(
        const QString& url,
        const QString& platform,
        bool includeComments = false
    ) override;
    
    /**
     * @brief 检查数据源是否可用
     *        Check if data source is available
     *
     * @return bool 可用返回 true / true if available
     */
    bool isAvailable() const override;
    
    /**
     * @brief 获取当前请求配额使用情况
     *        Get current request quota usage
     *
     * @return int 剩余请求次数 / Remaining request count
     */
    int getRemainingQuota() const override;
    
    /**
     * @brief 更新配置
     *        Update configuration
     *
     * @param config 新配置 / New configuration
     * @return bool 更新是否成功 / Whether update was successful
     */
    bool updateConfig(const QVariantMap& config);
    
    /**
     * @brief 获取当前配置
     *        Get current configuration
     *
     * @return QVariantMap 配置参数 / Configuration parameters
     */
    [[nodiscard]] QVariantMap getConfig() const;
    
    /**
     * @brief 设置认证 Token
     *        Set authentication token
     * @param token Bearer Token
     */
    void setToken(const QString& token);

signals:
    /**
     * @brief 配置更新信号
     *        Configuration updated signal
     *
     * @param config 新配置 / New configuration
     */
    void configUpdated(const QVariantMap& config);
    
    /**
     * @brief 数据采集进度信号
     *        Data collection progress signal
     *
     * @param current 当前进度 / Current progress
     * @param total 总任务数 / Total tasks
     * @param status 状态描述 / Status description
     */
    void progressChanged(int current, int total, const QString& status);

private slots:
    /**
     * @brief 处理网络请求完成信号
     *        Handle network request finished signal
     * @param reply 网络回复对象 / Network reply object
     */
    void onNetworkReplyFinished(QNetworkReply* reply);

private:
    /**
     * @brief 构建 API 请求头
     *        Build API request headers
     *
     * @return QMap<QString, QString> 请求头 / Request headers
     */
    [[nodiscard]] QMap<QString, QString> buildHeaders() const;
    
    /**
     * @brief 构建查询参数
     *        Build query parameters
     *
     * @param params 基础参数 / Base parameters
     * @return QUrlQuery 查询参数 / Query parameters
     */
    [[nodiscard]] QUrlQuery buildQueryParams(const QVariantMap& params) const;
    
    /**
     * @brief 构建 API 请求
     *        Build API request
     * @param endpoint API 端点 / API endpoint
     * @param params 查询参数 / Query parameters
     * @return QNetworkRequest 网络请求对象 / Network request object
     */
    QNetworkRequest buildRequest(const QString& endpoint, 
                                 const QVariantMap& params = {});
    
    /**
     * @brief 发送 HTTP 请求并等待响应
     *        Send HTTP request and wait for response
     * @param request 网络请求 / Network request
     * @return QPair<QByteArray, ApiErrorInfo> 响应数据或错误 / Response data or error
     */
    QPair<QByteArray, ApiErrorInfo> sendRequest(const QNetworkRequest& request);
    
    /**
     * @brief 解析错误响应
     *        Parse error response
     * @param reply 网络响应 / Network reply
     * @return ApiErrorInfo 错误对象 / Error object
     */
    ApiErrorInfo parseError(QNetworkReply* reply);
    
    /**
     * @brief 解析平台字符串为枚举
     *        Parse platform string to enum
     *
     * @param platform 平台字符串 / Platform string
     * @return JizhileConfig::Platform 平台枚举 / Platform enum
     */
    [[nodiscard]] static JizhileConfig::Platform parsePlatform(const QString& platform);
    
    /**
     * @brief 转换平台枚举为字符串
     *        Convert platform enum to string
     *
     * @param platform 平台枚举 / Platform enum
     * @return QString 平台字符串 / Platform string
     */
    [[nodiscard]] static QString platformToString(JizhileConfig::Platform platform);

private:
    JizhileConfig m_config;           ///< 当前配置 / Current configuration
    mutable QMutex m_mutex;           ///< 线程锁 / Thread mutex
    QString m_base_url;               ///< API 基础 URL / Base API URL
    QNetworkAccessManager* m_network; ///< 网络管理器 / Network manager
    bool m_initialized = false;       ///< 是否已初始化 / Whether initialized
    int m_remaining_quota = -1;       ///< 剩余配额 / Remaining quota
};

#endif // JIZHILE_PROVIDER_H
