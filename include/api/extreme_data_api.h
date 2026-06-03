/**
 * @file extreme_data_api.h
 * @brief API provider for 极致了 (JiZhiLe) data source - formerly known as 大家拉
 * @brief 极致了数据源 API 提供者 - 前身为"大家拉"
 * 
 * Implements comprehensive WeChat ecosystem data access including:
 * - Video Account (视频号) search and metrics
 * - Official Account (公众号) articles and statistics
 * - Real-time search (搜一搜) capabilities
 * 
 * 实现全面的微信生态系统数据访问，包括：
 * - 视频号搜索和指标
 * - 公众号文章和统计
 * - 实时搜一搜功能
 */

#ifndef EXTREME_DATA_API_H
#define EXTREME_DATA_API_H

#include "api/data_source_base.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

namespace mva::api {

/**
 * @brief ExtremeDataApi - 极致了 API implementation
 * @brief 极致了 API 实现
 * 
 * Supports 40+ endpoints for WeChat ecosystem data retrieval.
 * Key features:
 * - Device fingerprint simulation
 * - Geographic targeting (province_code)
 * - Anti-crawling bypass strategies
 * - Automatic retry with exponential backoff
 * 
 * 支持 40+ 个微信生态系统数据检索端点。
 * 主要特性：
 * - 设备指纹模拟
 * - 地理定位（省份代码）
 * - 反爬虫绕过策略
 * - 指数退避自动重试
 */
class ExtremeDataApi : public DataSourceBase {
    Q_OBJECT
    
public:
    explicit ExtremeDataApi(QObject *parent = nullptr);
    ~ExtremeDataApi() override;
    
    [[nodiscard]] QString providerName() const override { return QStringLiteral("极致了"); }
    [[nodiscard]] bool initialize(const QVariantMap &config) override;
    
    // Search by keyword - supports video accounts, articles, and general search
    [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> 
        searchByKeyword(const QString &keyword, int page = 1, int pageSize = 20) override;
    
    // Get trending content with category and time range filters
    [[nodiscard]] QFuture<std::expected<QVariantList, DataSourceError>> 
        getTrendingContent(const QString &category = "", const QString &timeRange = "24h") override;
    
    // Get detailed metrics for specific content
    [[nodiscard]] QFuture<std::expected<QVariantMap, DataSourceError>> 
        getContentMetrics(const QString &contentId) override;
    
    /**
     * @brief Search video accounts by keyword
     * @brief 按关键词搜索视频号
     */
    Q_INVOKABLE QFuture<std::expected<QVariantList, DataSourceError>> 
        searchVideoAccounts(const QString &keyword, int page = 1);
    
    /**
     * @brief Search video content by keyword
     * @brief 按关键词搜索视频内容
     */
    Q_INVOKABLE QFuture<std::expected<QVariantList, DataSourceError>> 
        searchVideoContent(const QString &keyword, int page = 1, const QString &sortBy = "relevance");
    
    /**
     * @brief Search official account articles
     * @brief 搜索公众号文章
     */
    Q_INVOKABLE QFuture<std::expected<QVariantList, DataSourceError>> 
        searchArticles(const QString &keyword, int page = 1, const QString &timeRange = "");
    
    /**
     * @brief Get video account details by ID
     * @brief 根据 ID 获取视频号详情
     */
    Q_INVOKABLE QFuture<std::expected<QVariantMap, DataSourceError>> 
        getVideoAccountInfo(const QString &accountId);
    
    /**
     * @brief Get video account works list
     * @brief 获取视频号作品列表
     */
    Q_INVOKABLE QFuture<std::expected<QVariantList, DataSourceError>> 
        getVideoAccountWorks(const QString &accountId, int page = 1);
    
    /**
     * @brief Get article real-time metrics (views, likes, etc.)
     * @brief 获取文章实时指标（阅读量、点赞等）
     */
    Q_INVOKABLE QFuture<std::expected<QVariantMap, DataSourceError>> 
        getArticleMetrics(const QString &articleUrl);
    
    /**
     * @brief Get official account info by name or ID
     * @brief 根据名称或 ID 获取公众号信息
     */
    Q_INVOKABLE QFuture<std::expected<QVariantMap, DataSourceError>> 
        getOfficialAccountInfo(const QString &identifier);
    
    /**
     * @brief Set geographic targeting
     * @brief 设置地理定位
     * @param provinceCode Province code (e.g., "110000" for Beijing) / 省份代码
     */
    Q_INVOKABLE void setProvinceCode(const QString &provinceCode);
    
    /**
     * @brief Set device fingerprint
     * @brief 设置设备指纹
     */
    Q_INVOKABLE void setDeviceFingerprint(const QString &deviceId, const QString &osVersion);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    
private:
    struct Private;
    std::unique_ptr<Private> d;
    
    QNetworkAccessManager m_networkManager;
    QString m_apiKey;
    QString m_provinceCode;
    QString m_deviceId;
    QString m_osVersion;
    QTimer m_rateLimitTimer;
    int m_requestCount{0};
    
    QFuture<std::expected<QVariantList, DataSourceError>> executeSearch(
        const QString &endpoint, const QVariantMap &params);
    
    QFuture<std::expected<QVariantMap, DataSourceError>> executeRequest(
        const QString &endpoint, const QVariantMap &params);
    
    QByteArray generateDeviceFingerprint() const;
    QVariantMap buildCommonParams() const;
    bool checkRateLimit();
};

} // namespace mva::api

#endif // EXTREME_DATA_API_H
