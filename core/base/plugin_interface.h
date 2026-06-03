#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

/**
 * @brief 插件基础接口定义
 *        Base plugin interface definition
 *
 * @details 所有插件必须继承此接口以实现统一的加载和管理。
 *          支持动态加载、热插拔、版本控制等功能。
 *          All plugins must inherit from this interface to enable
 *          unified loading and management. Supports dynamic loading,
 *          hot-swapping, version control, and other features.
 *
 * @note 使用 CTK Plugin Framework 作为底层实现
 *       Uses CTK Plugin Framework as the underlying implementation
 *
 * @example
 *   class MyDataSource : public QObject, public IDataSourcePlugin {
 *       Q_OBJECT
 *       Q_INTERFACES(IDataSourcePlugin)
 *   public:
 *       QString name() const override { return "MyProvider"; }
 *       Task<std::expected<ArticleData, Error>> fetchArticle(const QString& url) override;
 *   };
 */

#include <QString>
#include <QVariantMap>
#include <QFuture>
#include <expected>
#include <memory>

namespace MediaViralAssistant::Base {

/**
 * @brief 错误信息结构体
 *        Error information structure
 */
struct Error {
    int code;                    ///< 错误代码 / Error code
    QString message;             ///< 错误消息 / Error message
    QString source;              ///< 错误来源 / Error source
    QVariantMap details;         ///< 详细信息 / Detailed information
    
    /**
     * @brief 构造函数
     *        Constructor
     * @param c 错误代码 / Error code
     * @param msg 错误消息 / Error message
     * @param src 错误来源 / Error source
     */
    Error(int c = 0, const QString& msg = "", const QString& src = "")
        : code(c), message(msg), source(src) {}
};

/**
 * @brief 插件元数据
 *        Plugin metadata
 */
struct PluginMetadata {
    QString id;                  ///< 插件唯一标识 / Unique plugin identifier
    QString name;                ///< 插件名称 / Plugin name
    QString version;             ///< 版本号 / Version number
    QString description;         ///< 描述信息 / Description
    QString author;              ///< 作者 / Author
    QStringList platforms;       ///< 支持的平台 / Supported platforms
    QStringList dependencies;    ///< 依赖列表 / Dependencies
    bool enabled;                ///< 是否启用 / Is enabled
    
    PluginMetadata() : enabled(true) {}
};

/**
 * @brief 插件状态枚举
 *        Plugin status enumeration
 */
enum class PluginStatus {
    Unloaded,    ///< 未加载 / Not loaded
    Loading,     ///< 加载中 / Loading
    Loaded,      ///< 已加载 / Loaded
    Active,      ///< 激活状态 / Active
    Error,       ///< 错误状态 / Error
    Disabled     ///< 已禁用 / Disabled
};

/**
 * @brief 数据源插件接口
 *        Data source plugin interface
 *
 * @details 用于从各个自媒体平台获取爆款内容和数据指标。
 *          Used to fetch viral content and metrics from various media platforms.
 */
class IDataSourcePlugin {
public:
    virtual ~IDataSourcePlugin() = default;
    
    /**
     * @brief 获取插件元数据
     *        Get plugin metadata
     * @return 插件元数据 / Plugin metadata
     */
    virtual PluginMetadata metadata() const = 0;
    
    /**
     * @brief 获取支持的平台列表
     *        Get list of supported platforms
     * @return 平台标识列表 / List of platform identifiers
     */
    virtual QStringList supportedPlatforms() const = 0;
    
    /**
     * @brief 发现爆款内容
     *        Discover viral content
     *
     * @details 异步获取指定时间段内的爆款内容列表。
     *          Asynchronously fetches list of viral content within specified time period.
     *
     * @param platform 平台标识 / Platform identifier
     * @param startTime 开始时间 / Start time (Unix timestamp)
     * @param endTime 结束时间 / End time (Unix timestamp)
     * @param limit 数量限制 / Limit count
     * @return 包含文章列表或错误 / Contains article list or error
     */
    virtual QFuture<std::expected<QList<QVariantMap>, Error>> 
    discoverViralContent(const QString& platform, 
                         qint64 startTime, 
                         qint64 endTime, 
                         int limit) = 0;
    
    /**
     * @brief 获取内容详细数据
     *        Get detailed content data
     *
     * @details 获取文章的阅读量、点赞数、评论数等指标。
     *          Fetches article metrics including read count, likes, comments, etc.
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return 包含数据指标或错误 / Contains metrics or error
     */
    virtual QFuture<std::expected<QVariantMap, Error>> 
    fetchContentMetrics(const QString& url, const QString& platform) = 0;
    
    /**
     * @brief 获取正文内容
     *        Fetch full text content
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return 包含正文内容或错误 / Contains full text or error
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
     * @return 包含评论列表或错误 / Contains comment list or error
     */
    virtual QFuture<std::expected<QList<QVariantMap>, Error>> 
    fetchComments(const QString& url, const QString& platform, int limit = 50) = 0;
    
    /**
     * @brief 测试连接
     *        Test connection
     *
     * @details 验证 API 凭证和网络连接是否正常。
     *          Verifies API credentials and network connectivity.
     *
     * @return true 如果连接成功 / true if connection successful
     */
    virtual QFuture<bool> testConnection() = 0;
};

/**
 * @brief AI 提供商插件接口
 *        AI provider plugin interface
 *
 * @details 用于接入各种大语言模型服务。
 *          Used to integrate various large language model services.
 */
class IAIProviderPlugin {
public:
    virtual ~IAIProviderPlugin() = default;
    
    /**
     * @brief 获取插件元数据
     *        Get plugin metadata
     */
    virtual PluginMetadata metadata() const = 0;
    
    /**
     * @brief 发送聊天请求
     *        Send chat request
     *
     * @param messages 消息历史 / Message history
     * @param model 模型名称 / Model name
     * @param temperature 温度参数 / Temperature parameter
     * @return 包含响应或错误 / Contains response or error
     */
    virtual QFuture<std::expected<QString, Error>> 
    chat(const QList<QVariantMap>& messages, 
         const QString& model,
         double temperature = 0.7) = 0;
    
    /**
     * @brief 流式聊天
     *        Streaming chat
     *
     * @details 返回流式响应，适合长文本生成。
     *          Returns streaming response, suitable for long text generation.
     *
     * @param messages 消息历史 / Message history
     * @param model 模型名称 / Model name
     * @return QFuture 流式响应 / Streaming response future
     */
    virtual QFuture<QByteArray> 
    chatStream(const QList<QVariantMap>& messages, const QString& model) = 0;
    
    /**
     * @brief 获取可用模型列表
     *        Get available models
     * @return 模型列表 / List of models
     */
    virtual QFuture<QStringList> listModels() = 0;
    
    /**
     * @brief 测试连接
     *        Test connection
     */
    virtual QFuture<bool> testConnection() = 0;
};

/**
 * @brief 分析器插件接口
 *        Analyzer plugin interface
 *
 * @details 用于内容分析、趋势预测、SOP 生成等功能。
 *          Used for content analysis, trend prediction, SOP generation, etc.
 */
class IAnalyzerPlugin {
public:
    virtual ~IAnalyzerPlugin() = default;
    
    /**
     * @brief 获取插件元数据
     *        Get plugin metadata
     */
    virtual PluginMetadata metadata() const = 0;
    
    /**
     * @brief 拆解分析爆款内容
     *        Analyze and deconstruct viral content
     *
     * @param contentData 内容数据 / Content data
     * @param analysisType 分析类型 / Analysis type
     * @return 包含分析报告或错误 / Contains analysis report or error
     */
    virtual QFuture<std::expected<QVariantMap, Error>> 
    analyzeContent(const QVariantMap& contentData, const QString& analysisType) = 0;
    
    /**
     * @brief 生成选题建议
     *        Generate topic recommendations
     *
     * @param historicalData 历史数据 / Historical data
     * @param targetPlatform 目标平台 / Target platform
     * @param count 建议数量 / Number of recommendations
     * @return 包含选题列表或错误 / Contains topic list or error
     */
    virtual QFuture<std::expected<QList<QVariantMap>, Error>> 
    generateTopicSuggestions(const QList<QVariantMap>& historicalData,
                             const QString& targetPlatform,
                             int count = 10) = 0;
    
    /**
     * @brief 生成写作 SOP
     *        Generate writing SOP
     *
     * @param viralContent 爆款内容样本 / Viral content samples
     * @param contentType 内容类型 / Content type
     * @return 包含 SOP 或错误 / Contains SOP or error
     */
    virtual QFuture<std::expected<QString, Error>> 
    generateSOP(const QList<QVariantMap>& viralContent, const QString& contentType) = 0;
};

} // namespace MediaViralAssistant::Base

Q_DECLARE_INTERFACE(MediaViralAssistant::Base::IDataSourcePlugin, 
                    "MediaViralAssistant.IDataSourcePlugin/1.0")
Q_DECLARE_INTERFACE(MediaViralAssistant::Base::IAIProviderPlugin, 
                    "MediaViralAssistant.IAIProviderPlugin/1.0")
Q_DECLARE_INTERFACE(MediaViralAssistant::Base::IAnalyzerPlugin, 
                    "MediaViralAssistant.IAnalyzerPlugin/1.0")

#endif // PLUGIN_INTERFACE_H
