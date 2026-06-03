#ifndef MEDIA_VIRAL_ASSISTANT_API_DATA_SOURCE_BASE_H
#define MEDIA_VIRAL_ASSISTANT_API_DATA_SOURCE_BASE_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QFuture>
#include <expected>
#include <memory>
#include <vector>

namespace mva::api {

/**
 * @brief 数据指标结构体
 *        Data metrics structure
 */
struct Metrics {
    qint64 views = 0;           ///< 阅读量/播放量 / Views or plays
    qint64 likes = 0;           ///< 点赞数 / Likes
    qint64 comments = 0;        ///< 评论数 / Comments
    qint64 shares = 0;          ///< 转发数 / Shares
    qint64 favorites = 0;       ///< 收藏数 / Favorites
    qreal engagementRate = 0.0; ///< 互动率 / Engagement rate
    QDateTime publishTime;      ///< 发布时间 / Publish time
    QDateTime fetchTime;        ///< 抓取时间 / Fetch time
};

/**
 * @brief 文章内容结构体
 *        Article content structure
 */
struct ArticleContent {
    QString title;              ///< 标题 / Title
    QString body;               ///< 正文 / Body text
    QString description;        ///< 视频描述 / Video description
    QStringList images;         ///< 图片 URL 列表 / Image URLs
    QString videoUrl;           ///< 视频 URL / Video URL
    QStringList tags;           ///< 标签 / Tags
    QString author;             ///< 作者 / Author
    QVariantMap extra;          ///< 额外数据 / Extra data
};

/**
 * @brief API 错误类型
 *        API error types
 */
enum class ApiError {
    NetworkError,           ///< 网络错误 / Network error
    AuthenticationError,    ///< 认证失败 / Authentication failed
    RateLimitExceeded,      ///< 频率限制 / Rate limit exceeded
    NotFound,               ///< 资源未找到 / Resource not found
    ServerError,            ///< 服务器错误 / Server error
    ParseError,             ///< 解析错误 / Parse error
    Timeout,                ///< 超时 / Timeout
    Unknown                 ///< 未知错误 / Unknown error
};

/**
 * @brief API 错误信息
 *        API error information
 */
struct ApiErrorInfo {
    ApiError type;          ///< 错误类型 / Error type
    QString message;        ///< 错误消息 / Error message
    int httpCode = 0;       ///< HTTP 状态码 / HTTP status code
    QString provider;       ///< 提供商名称 / Provider name
};

using MetricsResult = std::expected<Metrics, ApiErrorInfo>;
using ContentResult = std::expected<ArticleContent, ApiErrorInfo>;

/**
 * @brief 数据源基类，所有数据提供商的抽象接口
 *        Base class for data sources, abstract interface for all data providers
 *
 * @details 定义了统一的数据获取接口，支持异步操作、错误处理、
 *          Fallback 机制。每个具体的数据源（如极致了、新榜等）都继承此类。
 *          Defines unified data fetching interface supporting async operations, 
 *          error handling, fallback mechanism. Each concrete data source inherits this.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class DataSourceBase : public QObject {
    Q_OBJECT

public:
    explicit DataSourceBase(QObject* parent = nullptr);
    ~DataSourceBase() override;

    /**
     * @brief 获取数据源唯一标识符
     *        Get unique identifier for this data source
     *
     * @return QString 数据源 ID / Data source ID
     */
    virtual QString id() const = 0;

    /**
     * @brief 获取数据源显示名称
     *        Get display name for this data source
     *
     * @return QString 显示名称 / Display name
     */
    virtual QString displayName() const = 0;

    /**
     * @brief 获取支持的平台列表
     *        Get list of supported platforms
     *
     * @return QStringList 平台标识符列表 / List of platform identifiers
     */
    virtual QStringList supportedPlatforms() const = 0;

    /**
     * @brief 初始化数据源连接
     *        Initialize data source connection
     *
     * @param config 配置参数 / Configuration parameters
     * @return bool 成功返回 true / true on success
     */
    virtual bool initialize(const QVariantMap& config) = 0;

    /**
     * @brief 获取文章数据指标（异步）
     *        Fetch article metrics (async)
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return QFuture<MetricsResult> 异步结果 / Async result
     */
    virtual QFuture<MetricsResult> fetchMetrics(
        const QString& url, 
        const QString& platform
    ) = 0;

    /**
     * @brief 获取文章内容详情（异步）
     *        Fetch article content details (async)
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @param includeComments 是否包含评论 / Whether to include comments
     * @return QFuture<ContentResult> 异步结果 / Async result
     */
    virtual QFuture<ContentResult> fetchContent(
        const QString& url,
        const QString& platform,
        bool includeComments = false
    ) = 0;

    /**
     * @brief 检查数据源是否可用
     *        Check if data source is available
     *
     * @return bool 可用返回 true / true if available
     */
    virtual bool isAvailable() const = 0;

    /**
     * @brief 获取当前请求配额使用情况
     *        Get current request quota usage
     *
     * @return int 剩余请求次数 / Remaining request count
     */
    virtual int getRemainingQuota() const = 0;

signals:
    /**
     * @brief 数据源状态变化时发出
     *        Emitted when data source status changes
     */
    void availabilityChanged(bool available);

    /**
     * @brief 配额更新时发出
     *        Emitted when quota is updated
     */
    void quotaUpdated(int remaining, int total);

protected:
    /**
     * @brief 构建错误信息
     *        Build error information
     */
    ApiErrorInfo buildError(
        ApiError type,
        const QString& message,
        int httpCode = 0
    ) const;

private:
    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::api

#endif
