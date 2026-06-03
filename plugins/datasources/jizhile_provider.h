/**
 * @file jizhile_provider.h
 * @brief 极致了数据 API 插件头文件
 *        Jizhile Data API Plugin Header
 */

#ifndef JIZHILE_PROVIDER_H
#define JIZHILE_PROVIDER_H

#include "core/base/plugin_interface.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>

namespace MediaViralAssistant::Plugins {

/**
 * @brief 极致了数据 API 数据源插件
 *        Jizhile Data API DataSource Plugin
 *
 * @details 通过极致了数据 API 获取各平台的爆款内容数据。
 *          支持微信公众号、抖音、小红书等平台。
 *          Fetches viral content data from multiple platforms via
 *          Jizhile Data API. Supports WeChat, Douyin, Xiaohongshu, etc.
 *
 * @note 需要配置有效的 Bearer Token 才能使用
 *       Requires valid Bearer Token for authentication
 */
class JizhileProvider : public QObject, 
                        public Base::IDataSourcePlugin {
    Q_OBJECT
    Q_INTERFACES(MediaViralAssistant::Base::IDataSourcePlugin)
    Q_PLUGIN_METADATA(IID "MediaViralAssistant.IDataSourcePlugin/1.0" 
                      FILE "jizhile_provider.json")

public:
    /**
     * @brief 构造函数
     *        Constructor
     * @param parent 父对象 / Parent object
     */
    explicit JizhileProvider(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     *        Destructor
     */
    ~JizhileProvider() override;

    // IDataSourcePlugin interface implementation
    // IDataSourcePlugin 接口实现
    
    /**
     * @brief 获取插件元数据
     *        Get plugin metadata
     * @return 插件元数据 / Plugin metadata
     */
    Base::PluginMetadata metadata() const override;
    
    /**
     * @brief 获取支持的平台列表
     *        Get list of supported platforms
     * @return 平台标识列表 / List of platform identifiers
     */
    QStringList supportedPlatforms() const override;
    
    /**
     * @brief 发现爆款内容
     *        Discover viral content
     *
     * @param platform 平台标识 / Platform identifier
     * @param startTime 开始时间戳 / Start timestamp
     * @param endTime 结束时间戳 / End timestamp
     * @param limit 数量限制 / Limit count
     * @return 包含文章列表或错误 / Contains article list or error
     */
    QFuture<std::expected<QList<QVariantMap>, Base::Error>> 
    discoverViralContent(const QString& platform, 
                         qint64 startTime, 
                         qint64 endTime, 
                         int limit) override;
    
    /**
     * @brief 获取内容指标数据
     *        Fetch content metrics
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return 包含指标数据或错误 / Contains metrics or error
     */
    QFuture<std::expected<QVariantMap, Base::Error>> 
    fetchContentMetrics(const QString& url, const QString& platform) override;
    
    /**
     * @brief 获取正文内容
     *        Fetch full text content
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @return 包含正文或错误 / Contains full text or error
     */
    QFuture<std::expected<QString, Base::Error>> 
    fetchFullContent(const QString& url, const QString& platform) override;
    
    /**
     * @brief 获取评论内容
     *        Fetch comments
     *
     * @param url 文章链接 / Article URL
     * @param platform 平台标识 / Platform identifier
     * @param limit 评论数量限制 / Comment limit
     * @return 包含评论列表或错误 / Contains comment list or error
     */
    QFuture<std::expected<QList<QVariantMap>, Base::Error>> 
    fetchComments(const QString& url, const QString& platform, int limit = 50) override;
    
    /**
     * @brief 测试 API 连接
     *        Test API connection
     * @return true 如果连接成功 / true if connection successful
     */
    QFuture<bool> testConnection() override;
    
    /**
     * @brief 设置认证 Token
     *        Set authentication token
     * @param token Bearer Token
     */
    void setToken(const QString& token);

private slots:
    /**
     * @brief 处理网络请求完成信号
     *        Handle network request finished signal
     * @param reply 网络回复对象 / Network reply object
     */
    void onNetworkReplyFinished(QNetworkReply* reply);

private:
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
     * @brief 解析错误响应
     *        Parse error response
     * @param reply 网络响应 / Network reply
     * @return Error 错误对象 / Error object
     */
    Base::Error parseError(QNetworkReply* reply);
    
    /**
     * @brief 发送 HTTP 请求并等待响应
     *        Send HTTP request and wait for response
     * @param request 网络请求 / Network request
     * @return QPair<QByteArray, Base::Error> 响应数据或错误 / Response data or error
     */
    QPair<QByteArray, Base::Error> sendRequest(const QNetworkRequest& request);

private:
    QNetworkAccessManager* networkManager_;  ///< 网络管理器 / Network manager
    QString token_;                          ///< API 令牌 / API token
    QString m_baseUrl;                       ///< API 基础 URL / Base URL
};

} // namespace MediaViralAssistant::Plugins

#endif // JIZHILE_PROVIDER_H
