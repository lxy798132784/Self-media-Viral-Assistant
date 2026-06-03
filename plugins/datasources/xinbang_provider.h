/**
 * @file xinbang_provider.h
 * @brief 新榜 API 数据源插件头文件
 *        XinBang (New Rank) API DataSource Plugin Header
 *
 * @details 新榜 API 主要提供估算值数据，使用 API Key 鉴权（Header: Key）。
 *          XinBang API mainly provides estimated data with API Key authentication (Header: Key).
 */

#ifndef XINBANG_PROVIDER_H
#define XINBANG_PROVIDER_H

#include "core/base/plugin_interface.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QString>

namespace MediaViralAssistant::Plugins {

class XinBangProvider : public QObject, public IDataSourcePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "media-assistant.datasource.xinbang" FILE "plugin.json")
    Q_INTERFACES(MediaViralAssistant::Plugins::IDataSourcePlugin)

public:
    /**
     * @brief 构造函数 / Constructor
     * @param parent 父对象 / Parent object
     */
    explicit XinBangProvider(QObject* parent = nullptr);

    /**
     * @brief 析构函数 / Destructor
     */
    ~XinBangProvider() override;

    /**
     * @brief 获取插件名称 / Get plugin name
     * @return 插件名称 / Plugin name
     */
    [[nodiscard]] QString name() const override;

    /**
     * @brief 设置 API Key / Set API Key
     * @param key API Key
     */
    void setApiKey(const QString& key);

    /**
     * @brief 获取爆款内容列表 / Fetch viral content list
     * @param platform 平台标识 / Platform identifier
     * @param limit 数量限制 / Limit count
     * @param min_views 最小阅读量 / Minimum view count
     * @return 内容列表结果 / Content list result
     */
    QFuture<std::expected<QList<ContentItem>, ApiError>> 
    fetchViralContent(const QString& platform, int limit, int min_views) override;

    /**
     * @brief 获取内容详细数据 / Fetch content detailed data
     * @param url 内容链接 / Content URL
     * @return 内容详情结果 / Content detail result
     */
    QFuture<std::expected<ContentDetail, ApiError>>
    fetchContentDetail(const QString& url) override;

    /**
     * @brief 获取内容数据指标 / Fetch content metrics
     * @param url 内容链接 / Content URL
     * @return 数据指标结果 / Metrics result
     */
    QFuture<std::expected<ContentMetrics, ApiError>>
    fetchContentMetrics(const QString& url) override;

private slots:
    /**
     * @brief 网络请求完成处理 / Handle network request completion
     * @param reply 网络回复对象 / Network reply object
     */
    void onNetworkReplyFinished(QNetworkReply* reply);

private:
    /**
     * @brief 构建 API 请求头 / Build API request headers
     * @return 网络请求对象 / Network request object
     */
    [[nodiscard]] QNetworkRequest buildRequest() const;

    QNetworkAccessManager* networkManager_;
    QString apiKey_;
};

} // namespace MediaViralAssistant::Plugins

#endif // XINBANG_PROVIDER_H
