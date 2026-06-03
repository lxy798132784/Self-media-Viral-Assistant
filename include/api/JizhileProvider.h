#ifndef JIZHILE_PROVIDER_H
#define JIZHILE_PROVIDER_H

#include "data_source_base.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
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
    std::string bearer_token;              ///< API 访问令牌 / API Access Token
    int timeout_seconds = 30;              ///< 请求超时时间 (秒) / Request timeout (seconds)
    int max_retries = 3;                   ///< 最大重试次数 / Max retry count
    
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
    std::vector<Platform> enabled_platforms;  ///< 启用的平台列表 / List of enabled platforms
    
    // === 设备集群配置 / Device Cluster Configuration ===
    enum class DeviceType {
        iOS,           ///< iOS 设备 / iOS Device
        Android,       ///< Android 设备 / Android Device
        PC_Web,        ///< PC 网页 / PC Web
        Mobile_Web     ///< 移动网页 / Mobile Web
    };
    DeviceType preferred_device = DeviceType::iOS;  ///< 首选设备类型 / Preferred device type
    std::string os_version_filter;                  ///< OS 版本过滤 (e.g., "15.0-17.0") / OS version filter
    std::string device_brand_filter;                ///< 设备品牌过滤 (e.g., "Apple,Huawei") / Device brand filter
    
    // === 地域定向 / Geo-Targeting ===
    std::vector<std::string> target_provinces;      ///< 目标省份 (e.g., ["Beijing", "Shanghai"]) / Target provinces
    std::vector<std::string> target_cities;         ///< 目标城市 / Target cities
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
    std::string custom_user_agent;             ///< 自定义 User-Agent / Custom User-Agent
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
     * @brief 获取提供者名称
     *        Get provider name
     * 
     * @return QString 提供者名称 / Provider name
     */
    [[nodiscard]] QString name() const override;
    
    /**
     * @brief 获取提供者描述
     *        Get provider description
     * 
     * @return QString 提供者描述 / Provider description
     */
    [[nodiscard]] QString description() const override;
    
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
     * @return std::expected<void, ErrorInfo> 初始化结果 / Initialization result
     *
     * @note 异步操作，需配合 co_await 使用
     *       Asynchronous operation, use with co_await
     */
    co_future<std::expected<void, ErrorInfo>> initialize(const QVariantMap& config) override;
    
    /**
     * @brief 发现爆款内容
     *        Discover viral content
     *
     * @param platform 平台标识 / Platform identifier
     * @param category 内容分类 / Content category
     * @param time_range 时间范围 (小时) / Time range (hours)
     * @param limit 返回数量限制 / Limit of results
     * @return std::expected<QList<ContentItem>, ErrorInfo> 内容列表或错误 / Content list or error
     *
     * @example
     *   auto result = co_await discoverViralContent("wechat", "technology", 24, 50);
     *   if (result) {
     *       for (const auto& item : *result) {
     *           qDebug() << item.title;
     *       }
     *   }
     */
    co_future<std::expected<QList<ContentItem>, ErrorInfo>> 
    discoverViralContent(const QString& platform, 
                         const QString& category,
                         int time_range,
                         int limit) override;
    
    /**
     * @brief 获取内容详细数据指标
     *        Fetch detailed metrics for content
     *
     * @param content_id 内容 ID / Content ID
     * @param platform 平台标识 / Platform identifier
     * @return std::expected<ContentMetrics, ErrorInfo> 数据指标或错误 / Metrics or error
     *
     * @note 自动处理限流和重试
     *       Automatically handles rate limiting and retries
     */
    co_future<std::expected<ContentMetrics, ErrorInfo>> 
    fetchContentMetrics(const QString& content_id, 
                        const QString& platform) override;
    
    /**
     * @brief 获取文章正文或视频详情
     *        Fetch article content or video details
     *
     * @param content_id 内容 ID / Content ID
     * @param platform 平台标识 / Platform identifier
     * @return std::expected<ContentDetail, ErrorInfo> 详细内容或错误 / Detail content or error
     */
    co_future<std::expected<ContentDetail, ErrorInfo>> 
    fetchContentDetail(const QString& content_id, 
                       const QString& platform) override;
    
    /**
     * @brief 获取评论内容列表
     *        Fetch comment list
     *
     * @param content_id 内容 ID / Content ID
     * @param platform 平台标识 / Platform identifier
     * @param limit 评论数量限制 / Comment limit
     * @param sort_by 排序方式 ("time", "like") / Sort method
     * @return std::expected<QList<Comment>, ErrorInfo> 评论列表或错误 / Comment list or error
     */
    co_future<std::expected<QList<Comment>, ErrorInfo>> 
    fetchComments(const QString& content_id, 
                  const QString& platform,
                  int limit = 50,
                  const QString& sort_by = "time") override;
    
    /**
     * @brief 更新配置
     *        Update configuration
     *
     * @param config 新配置 / New configuration
     * @return bool 更新是否成功 / Whether update was successful
     */
    bool updateConfig(const QVariantMap& config) override;
    
    /**
     * @brief 获取当前配置
     *        Get current configuration
     *
     * @return QVariantMap 配置参数 / Configuration parameters
     */
    [[nodiscard]] QVariantMap getConfig() const override;
    
    /**
     * @brief 测试连接
     *        Test connection
     *
     * @return std::expected<bool, ErrorInfo> 测试结果 / Test result
     */
    co_future<std::expected<bool, ErrorInfo>> testConnection() override;

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
     * @brief 发送 HTTP 请求
     *        Send HTTP request
     *
     * @param url 请求 URL / Request URL
     * @param method HTTP 方法 / HTTP method
     * @param body 请求体 / Request body
     * @return std::expected<QByteArray, ErrorInfo> 响应数据或错误 / Response data or error
     */
    co_future<std::expected<QByteArray, ErrorInfo>> 
    sendRequest(const QUrl& url, 
                const QString& method = "GET",
                const QByteArray& body = {}) const;
    
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
    JizhileConfig m_config;  ///< 当前配置 / Current configuration
    mutable QMutex m_mutex;  ///< 线程锁 / Thread mutex
    QString m_base_url;      ///< API 基础 URL / Base API URL
};

#endif // JIZHILE_PROVIDER_H
