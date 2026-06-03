#ifndef MEDIA_VIRAL_ASSISTANT_CORE_BASE_PLUGIN_INTERFACE_H
#define MEDIA_VIRAL_ASSISTANT_CORE_BASE_PLUGIN_INTERFACE_H

#include <QString>
#include <QStringList>
#include <QVariantMap>
#include <QFuture>
#include <expected>
#include <QtPlugin>

namespace MediaViralAssistant::Base {

/**
 * @brief 插件元数据结构体
 *        Plugin metadata structure
 * 
 * @details 包含插件的基本信息，如名称、版本、描述等
 *          Contains basic plugin information such as name, version, description
 */
struct PluginMetadata {
    QString id;                 ///< 插件唯一标识符 / Unique plugin ID
    QString name;               ///< 插件显示名称 / Display name
    QString version;            ///< 版本号 / Version number
    QString description;        ///< 插件描述 / Description
    QString author;             ///< 作者 / Author
    QStringList dependencies;   ///< 依赖列表 / List of dependencies
    bool isCore = false;        ///< 是否为核心插件 / Whether it's a core plugin
};

/**
 * @brief 错误类型枚举
 *        Error type enumeration
 */
enum class ErrorType {
    NoError,                ///< 无错误 / No error
    NetworkError,           ///< 网络错误 / Network error
    AuthenticationError,    ///< 认证错误 / Authentication error
    ParseError,             ///< 解析错误 / Parse error
    TimeoutError,           ///< 超时错误 / Timeout error
    ValidationError,        ///< 验证错误 / Validation error
    NotFoundError,          ///< 未找到错误 / Not found error
    RateLimitError,         ///< 频率限制错误 / Rate limit error
    InternalError,          ///< 内部错误 / Internal error
    UnknownError            ///< 未知错误 / Unknown error
};

/**
 * @brief 错误信息结构体
 *        Error information structure
 * 
 * @details 包含错误的详细信息，用于错误处理和用户提示
 *          Contains detailed error information for error handling and user prompts
 */
struct Error {
    ErrorType type = ErrorType::NoError;    ///< 错误类型 / Error type
    QString message;                         ///< 错误消息 / Error message
    QString details;                         ///< 详细错误信息 / Detailed error info
    int errorCode = 0;                       ///< 错误代码 / Error code
    QString source;                          ///< 错误来源 / Error source
    
    /**
     * @brief 检查是否有错误
     *        Check if there is an error
     * @return true 如果有错误 / true if there is an error
     */
    [[nodiscard]] bool hasError() const {
        return type != ErrorType::NoError;
    }
    
    /**
     * @brief 转换为字符串表示
     *        Convert to string representation
     * @return QString 错误字符串 / Error string
     */
    [[nodiscard]] QString toString() const {
        if (!hasError()) {
            return QStringLiteral("No error");
        }
        return QStringLiteral("[%1] %2: %3")
            .arg(source.isEmpty() ? QStringLiteral("Unknown") : source)
            .arg(message)
            .arg(details);
    }
};

/**
 * @brief 内容项结构体
 *        Content item structure
 * 
 * @details 表示一个内容项的基本信息
 *          Represents basic information of a content item
 */
struct ContentItem {
    QString id;                 ///< 内容 ID / Content ID
    QString title;              ///< 标题 / Title
    QString url;                ///< URL 链接 / URL link
    QString platform;           ///< 平台标识 / Platform identifier
    QString author;             ///< 作者 / Author
    QString thumbnail;          ///< 缩略图 URL / Thumbnail URL
    QDateTime publishTime;      ///< 发布时间 / Publish time
    QVariantMap metrics;        ///< 数据指标 / Metrics
    QVariantMap extra;          ///< 额外数据 / Extra data
};

/**
 * @brief 内容详情结构体
 *        Content detail structure
 */
struct ContentDetail {
    QString content;            ///< 正文内容 / Main content
    QString htmlContent;        ///< HTML 格式内容 / HTML formatted content
    QString description;        ///< 描述 / Description
    QStringList images;         ///< 图片列表 / Image list
    QString videoUrl;           ///< 视频 URL / Video URL
    QStringList tags;           ///< 标签列表 / Tag list
    QStringList comments;       ///< 评论列表 / Comment list
    QVariantMap extra;          ///< 额外数据 / Extra data
};

/**
 * @brief 内容指标结构体
 *        Content metrics structure
 */
struct ContentMetrics {
    qint64 views = 0;           ///< 阅读量/播放量 / Views or plays
    qint64 likes = 0;           ///< 点赞数 / Likes
    qint64 comments = 0;        ///< 评论数 / Comments
    qint64 shares = 0;          ///< 转发数 / Shares
    qint64 favorites = 0;       ///< 收藏数 / Favorites
    qint64 collects = 0;        ///< 收藏数 (另一种统计) / Collects (alternative metric)
    qreal engagementRate = 0.0; ///< 互动率 / Engagement rate
    QDateTime collectionTime;   ///< 采集时间 / Collection time
    QVariantMap extra;          ///< 额外指标 / Extra metrics
};

/**
 * @brief 数据源插件接口
 *        Data source plugin interface
 * 
 * @details 所有数据源插件必须实现此接口
 *          All data source plugins must implement this interface
 */
class IDataSourcePlugin {
public:
    virtual ~IDataSourcePlugin() = default;
    
    /**
     * @brief 获取插件元数据
     *        Get plugin metadata
     * @return PluginMetadata 插件元数据 / Plugin metadata
     */
    virtual PluginMetadata metadata() const = 0;
    
    /**
     * @brief 获取支持的平台列表
     *        Get list of supported platforms
     * @return QStringList 平台标识列表 / List of platform identifiers
     */
    virtual QStringList supportedPlatforms() const = 0;
    
    /**
     * @brief 发现爆款内容
     *        Discover viral content
     * 
     * @param platform 平台标识 / Platform identifier
     * @param startTime 开始时间戳 / Start timestamp
     * @param endTime 结束时间戳 / End timestamp
     * @param limit 数量限制 / Limit count
     * @return QFuture<std::expected<QList<QVariantMap>, Error>> 异步结果 / Async result
     */
    virtual QFuture<std::expected<QList<QVariantMap>, Error>> 
    discoverViralContent(const QString& platform, 
                         qint64 startTime, 
                         qint64 endTime, 
                         int limit) = 0;
    
    /**
     * @brief 获取内容指标数据
     *        Fetch content metrics
     * 
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return QFuture<std::expected<QVariantMap, Error>> 异步结果 / Async result
     */
    virtual QFuture<std::expected<QVariantMap, Error>> 
    fetchContentMetrics(const QString& url, const QString& platform) = 0;
    
    /**
     * @brief 获取正文内容
     *        Fetch full text content
     * 
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return QFuture<std::expected<QString, Error>> 异步结果 / Async result
     */
    virtual QFuture<std::expected<QString, Error>> 
    fetchFullContent(const QString& url, const QString& platform) = 0;
    
    /**
     * @brief 获取评论内容
     *        Fetch comments
     * 
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @param limit 评论数量限制 / Comment limit
     * @return QFuture<std::expected<QList<QVariantMap>, Error>> 异步结果 / Async result
     */
    virtual QFuture<std::expected<QList<QVariantMap>, Error>> 
    fetchComments(const QString& url, const QString& platform, int limit = 50) = 0;
    
    /**
     * @brief 测试 API 连接
     *        Test API connection
     * @return QFuture<bool> 测试结果 / Test result
     */
    virtual QFuture<bool> testConnection() = 0;
};

} // namespace MediaViralAssistant::Base

// 定义 Qt 插件接口 IID
#define IDataSourcePlugin_iid "MediaViralAssistant.IDataSourcePlugin/1.0"
Q_DECLARE_INTERFACE(MediaViralAssistant::Base::IDataSourcePlugin, IDataSourcePlugin_iid)

#endif // MEDIA_VIRAL_ASSISTANT_CORE_BASE_PLUGIN_INTERFACE_H
