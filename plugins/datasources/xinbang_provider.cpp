/**
 * @file xinbang_provider.cpp
 * @brief 新榜 API 数据源插件实现
 *        XinBang (New Rank) API DataSource Plugin Implementation
 */

#include "xinbang_provider.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent>

namespace MediaViralAssistant::Plugins {

XinBangProvider::XinBangProvider(QObject* parent)
    : QObject(parent)
    , networkManager_(new QNetworkAccessManager(this))
    , apiKey_("")
{
    /**
     * @brief 初始化网络管理器
     *        Initialize network manager
     */
    connect(networkManager_, &QNetworkAccessManager::finished,
            this, &XinBangProvider::onNetworkReplyFinished);
}

XinBangProvider::~XinBangProvider()
{
    /**
     * @brief 清理资源
     *        Cleanup resources
     */
    networkManager_->deleteLater();
}

QString XinBangProvider::name() const
{
    /**
     * @brief 返回插件名称
     *        Return plugin name
     */
    return QStringLiteral("XinBang");
}

void XinBangProvider::setApiKey(const QString& key)
{
    /**
     * @brief 设置 API Key
     *        Set API Key
     * @param key API Key
     */
    apiKey_ = key;
}

QNetworkRequest XinBangProvider::buildRequest() const
{
    /**
     * @brief 构建 API 请求头
     *        Build API request headers
     * @return 网络请求对象 / Network request object
     */
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    if (!apiKey_.isEmpty()) {
        request.setRawHeader("Key", apiKey_.toUtf8());
    }
    return request;
}

QFuture<std::expected<QList<ContentItem>, ApiError>> 
XinBangProvider::fetchViralContent(const QString& platform, int limit, int min_views)
{
    /**
     * @brief 获取爆款内容列表（新榜估算值）
     *        Fetch viral content list (XinBang estimated values)
     * @param platform 平台标识 / Platform identifier
     * @param limit 数量限制 / Limit count
     * @param min_views 最小阅读量 / Minimum view count
     * @return 未来结果包含内容列表或错误 / Future result with content list or error
     */
    return QtConcurrent::run([=]() -> std::expected<QList<ContentItem>, ApiError> {
        QUrl url(QStringLiteral("https://api.newrank.cn/v1/content/viral"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("platform"), platform);
        query.addQueryItem(QStringLiteral("limit"), QString::number(limit));
        query.addQueryItem(QStringLiteral("min_views"), QString::number(min_views));
        url.setQuery(query);

        QNetworkRequest request = buildRequest();
        request.setUrl(url);

        QNetworkReply* reply = networkManager_->get(request);
        
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            return std::unexpected(ApiError{
                ApiError::Type::NetworkError,
                reply->errorString(),
                static_cast<int>(reply->error())
            });
        }

        QByteArray data = reply->readAll();
        reply->deleteLater();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            return std::unexpected(ApiError{
                ApiError::Type::ParseError,
                QStringLiteral("Failed to parse JSON response"),
                0
            });
        }

        QJsonObject root = doc.object();
        if (root.value(QStringLiteral("code")).toInt() != 200) {
            return std::unexpected(ApiError{
                ApiError::Type::ApiError,
                root.value(QStringLiteral("message")).toString(),
                root.value(QStringLiteral("code")).toInt()
            });
        }

        QList<ContentItem> items;
        QJsonArray contentArray = root.value(QStringLiteral("data")).toArray();
        for (const QJsonValue& val : contentArray) {
            QJsonObject obj = val.toObject();
            ContentItem item;
            item.id = obj.value(QStringLiteral("id")).toString();
            item.title = obj.value(QStringLiteral("title")).toString();
            item.url = obj.value(QStringLiteral("url")).toString();
            item.platform = obj.value(QStringLiteral("platform")).toString();
            item.author = obj.value(QStringLiteral("author")).toString();
            item.publishTime = QDateTime::fromString(
                obj.value(QStringLiteral("publish_time")).toString(), 
                Qt::ISODate
            );
            items.append(item);
        }

        return items;
    });
}

QFuture<std::expected<ContentDetail, ApiError>>
XinBangProvider::fetchContentDetail(const QString& url)
{
    /**
     * @brief 获取内容详细信息
     *        Fetch content detailed information
     * @param url 内容链接 / Content URL
     * @return 未来结果包含详情或错误 / Future result with detail or error
     */
    return QtConcurrent::run([=]() -> std::expected<ContentDetail, ApiError> {
        QUrl apiUrl(QStringLiteral("https://api.newrank.cn/v1/content/detail"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("url"), url);
        apiUrl.setQuery(query);

        QNetworkRequest request = buildRequest();
        request.setUrl(apiUrl);

        QNetworkReply* reply = networkManager_->get(request);
        
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            return std::unexpected(ApiError{
                ApiError::Type::NetworkError,
                reply->errorString(),
                static_cast<int>(reply->error())
            });
        }

        QByteArray data = reply->readAll();
        reply->deleteLater();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            return std::unexpected(ApiError{
                ApiError::Type::ParseError,
                QStringLiteral("Failed to parse JSON response"),
                0
            });
        }

        QJsonObject root = doc.object();
        if (root.value(QStringLiteral("code")).toInt() != 200) {
            return std::unexpected(ApiError{
                ApiError::Type::ApiError,
                root.value(QStringLiteral("message")).toString(),
                root.value(QStringLiteral("code")).toInt()
            });
        }

        QJsonObject dataObj = root.value(QStringLiteral("data")).toObject();
        ContentDetail detail;
        detail.content = dataObj.value(QStringLiteral("content")).toString();
        detail.description = dataObj.value(QStringLiteral("description")).toString();
        detail.htmlContent = dataObj.value(QStringLiteral("html_content")).toString();
        
        return detail;
    });
}

QFuture<std::expected<ContentMetrics, ApiError>>
XinBangProvider::fetchContentMetrics(const QString& url)
{
    /**
     * @brief 获取内容数据指标（估算值）
     *        Fetch content metrics (estimated values)
     * @param url 内容链接 / Content URL
     * @return 未来结果包含指标或错误 / Future result with metrics or error
     */
    return QtConcurrent::run([=]() -> std::expected<ContentMetrics, ApiError> {
        QUrl apiUrl(QStringLiteral("https://api.newrank.cn/v1/content/metrics"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("url"), url);
        apiUrl.setQuery(query);

        QNetworkRequest request = buildRequest();
        request.setUrl(apiUrl);

        QNetworkReply* reply = networkManager_->get(request);
        
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            return std::unexpected(ApiError{
                ApiError::Type::NetworkError,
                reply->errorString(),
                static_cast<int>(reply->error())
            });
        }

        QByteArray data = reply->readAll();
        reply->deleteLater();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            return std::unexpected(ApiError{
                ApiError::Type::ParseError,
                QStringLiteral("Failed to parse JSON response"),
                0
            });
        }

        QJsonObject root = doc.object();
        if (root.value(QStringLiteral("code")).toInt() != 200) {
            return std::unexpected(ApiError{
                ApiError::Type::ApiError,
                root.value(QStringLiteral("message")).toString(),
                root.value(QStringLiteral("code")).toInt()
            });
        }

        QJsonObject metricsObj = root.value(QStringLiteral("data")).toObject();
        ContentMetrics metrics;
        metrics.views = metricsObj.value(QStringLiteral("views")).toInt(0);
        metrics.likes = metricsObj.value(QStringLiteral("likes")).toInt(0);
        metrics.comments = metricsObj.value(QStringLiteral("comments")).toInt(0);
        metrics.shares = metricsObj.value(QStringLiteral("shares")).toInt(0);
        metrics.favorites = metricsObj.value(QStringLiteral("favorites")).toInt(0);
        metrics.collectionTime = QDateTime::currentDateTime();

        return metrics;
    });
}

void XinBangProvider::onNetworkReplyFinished(QNetworkReply* reply)
{
    /**
     * @brief 处理网络请求完成信号
     *        Handle network request finished signal
     * @param reply 网络回复对象 / Network reply object
     */
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "XinBangProvider: Network error:" << reply->errorString();
    }
    reply->deleteLater();
}

} // namespace MediaViralAssistant::Plugins
