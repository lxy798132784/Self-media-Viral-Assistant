/**
 * @file jizhile_provider.cpp
 * @brief 极致了数据 API 插件实现
 *        Jizhile Data API Plugin Implementation
 */

#include "jizhile_provider.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtConcurrent>
#include <QEventLoop>
#include <QTimer>
#include <QUuid>

namespace MediaViralAssistant::Plugins {

JizhileProvider::JizhileProvider(QObject* parent)
    : QObject(parent)
    , networkManager_(new QNetworkAccessManager(this))
    , token_("")
    , m_baseUrl("https://api.jizhile.com/v1")
{
    connect(networkManager_, &QNetworkAccessManager::finished,
            this, &JizhileProvider::onNetworkReplyFinished);
}

JizhileProvider::~JizhileProvider()
{
    networkManager_->deleteLater();
}

Base::PluginMetadata JizhileProvider::metadata() const
{
    Base::PluginMetadata meta;
    meta.id = "jizhile";
    meta.name = QStringLiteral("极致了");
    meta.version = "1.0.0";
    meta.description = QStringLiteral("极致了数据 API 数据源插件，支持微信公众号、抖音、小红书等平台");
    meta.author = "MVA Team";
    meta.isCore = false;
    return meta;
}

QStringList JizhileProvider::supportedPlatforms() const
{
    return {
        QStringLiteral("wechat"),
        QStringLiteral("weixin"),
        QStringLiteral("douyin"),
        QStringLiteral("xiaohongshu"),
        QStringLiteral("weibo"),
        QStringLiteral("kuaishou"),
        QStringLiteral("bilibili"),
        QStringLiteral("toutiao"),
        QStringLiteral("zhihu")
    };
}

QFuture<std::expected<QList<QVariantMap>, Base::Error>> 
JizhileProvider::discoverViralContent(const QString& platform, 
                                       qint64 startTime, 
                                       qint64 endTime, 
                                       int limit)
{
    return QtConcurrent::run([=]() -> std::expected<QList<QVariantMap>, Base::Error> {
        QUrl url(m_baseUrl + QStringLiteral("/content/viral"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("platform"), platform);
        query.addQueryItem(QStringLiteral("start_time"), QString::number(startTime));
        query.addQueryItem(QStringLiteral("end_time"), QString::number(endTime));
        query.addQueryItem(QStringLiteral("limit"), QString::number(limit));
        url.setQuery(query);

        QNetworkRequest request = buildRequest(url.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.hasError()) {
            return std::unexpected(error);
        }

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            Base::Error err;
            err.type = Base::ErrorType::ParseError;
            err.message = QStringLiteral("Failed to parse JSON response");
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QJsonObject root = doc.object();
        int code = root.value(QStringLiteral("code")).toInt(-1);
        if (code != 200) {
            Base::Error err;
            err.type = Base::ErrorType::InternalError;
            err.message = root.value(QStringLiteral("message")).toString();
            err.errorCode = code;
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QList<QVariantMap> items;
        QJsonArray contentArray = root.value(QStringLiteral("data")).toArray();
        for (const QJsonValue& val : contentArray) {
            QJsonObject obj = val.toObject();
            QVariantMap item;
            item["id"] = obj.value(QStringLiteral("id")).toString();
            item["title"] = obj.value(QStringLiteral("title")).toString();
            item["url"] = obj.value(QStringLiteral("url")).toString();
            item["platform"] = obj.value(QStringLiteral("platform")).toString();
            item["author"] = obj.value(QStringLiteral("author")).toString();
            item["publish_time"] = QDateTime::fromString(
                obj.value(QStringLiteral("publish_time")).toString(),
                Qt::ISODate
            );
            
            QVariantMap metrics;
            QJsonObject metricsObj = obj.value(QStringLiteral("metrics")).toObject();
            metrics["views"] = metricsObj.value(QStringLiteral("views")).toVariant();
            metrics["likes"] = metricsObj.value(QStringLiteral("likes")).toVariant();
            metrics["comments"] = metricsObj.value(QStringLiteral("comments")).toVariant();
            metrics["shares"] = metricsObj.value(QStringLiteral("shares")).toVariant();
            item["metrics"] = metrics;
            
            items.append(item);
        }

        return items;
    });
}

QFuture<std::expected<QVariantMap, Base::Error>> 
JizhileProvider::fetchContentMetrics(const QString& url, const QString& platform)
{
    return QtConcurrent::run([=]() -> std::expected<QVariantMap, Base::Error> {
        QUrl apiUrl(m_baseUrl + QStringLiteral("/content/metrics"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("url"), url);
        query.addQueryItem(QStringLiteral("platform"), platform);
        apiUrl.setQuery(query);

        QNetworkRequest request = buildRequest(apiUrl.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.hasError()) {
            return std::unexpected(error);
        }

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            Base::Error err;
            err.type = Base::ErrorType::ParseError;
            err.message = QStringLiteral("Failed to parse JSON response");
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QJsonObject root = doc.object();
        int code = root.value(QStringLiteral("code")).toInt(-1);
        if (code != 200) {
            Base::Error err;
            err.type = Base::ErrorType::InternalError;
            err.message = root.value(QStringLiteral("message")).toString();
            err.errorCode = code;
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QVariantMap metrics;
        QJsonObject metricsObj = root.value(QStringLiteral("data")).toObject();
        metrics["views"] = metricsObj.value(QStringLiteral("views")).toVariant();
        metrics["likes"] = metricsObj.value(QStringLiteral("likes")).toVariant();
        metrics["comments"] = metricsObj.value(QStringLiteral("comments")).toVariant();
        metrics["shares"] = metricsObj.value(QStringLiteral("shares")).toVariant();
        metrics["favorites"] = metricsObj.value(QStringLiteral("favorites")).toVariant();
        metrics["collects"] = metricsObj.value(QStringLiteral("collects")).toVariant();
        metrics["engagement_rate"] = metricsObj.value(QStringLiteral("engagement_rate")).toVariant();
        metrics["collection_time"] = QDateTime::currentDateTime();

        return metrics;
    });
}

QFuture<std::expected<QString, Base::Error>> 
JizhileProvider::fetchFullContent(const QString& url, const QString& platform)
{
    return QtConcurrent::run([=]() -> std::expected<QString, Base::Error> {
        QUrl apiUrl(m_baseUrl + QStringLiteral("/content/detail"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("url"), url);
        query.addQueryItem(QStringLiteral("platform"), platform);
        apiUrl.setQuery(query);

        QNetworkRequest request = buildRequest(apiUrl.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.hasError()) {
            return std::unexpected(error);
        }

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            Base::Error err;
            err.type = Base::ErrorType::ParseError;
            err.message = QStringLiteral("Failed to parse JSON response");
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QJsonObject root = doc.object();
        int code = root.value(QStringLiteral("code")).toInt(-1);
        if (code != 200) {
            Base::Error err;
            err.type = Base::ErrorType::InternalError;
            err.message = root.value(QStringLiteral("message")).toString();
            err.errorCode = code;
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QJsonObject dataObj = root.value(QStringLiteral("data")).toObject();
        QString content = dataObj.value(QStringLiteral("content")).toString();
        if (content.isEmpty()) {
            content = dataObj.value(QStringLiteral("html_content")).toString();
        }

        return content;
    });
}

QFuture<std::expected<QList<QVariantMap>, Base::Error>> 
JizhileProvider::fetchComments(const QString& url, const QString& platform, int limit)
{
    return QtConcurrent::run([=]() -> std::expected<QList<QVariantMap>, Base::Error> {
        QUrl apiUrl(m_baseUrl + QStringLiteral("/content/comments"));
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("url"), url);
        query.addQueryItem(QStringLiteral("platform"), platform);
        query.addQueryItem(QStringLiteral("limit"), QString::number(limit));
        apiUrl.setQuery(query);

        QNetworkRequest request = buildRequest(apiUrl.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.hasError()) {
            return std::unexpected(error);
        }

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull()) {
            Base::Error err;
            err.type = Base::ErrorType::ParseError;
            err.message = QStringLiteral("Failed to parse JSON response");
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QJsonObject root = doc.object();
        int code = root.value(QStringLiteral("code")).toInt(-1);
        if (code != 200) {
            Base::Error err;
            err.type = Base::ErrorType::InternalError;
            err.message = root.value(QStringLiteral("message")).toString();
            err.errorCode = code;
            err.source = "jizhile";
            return std::unexpected(err);
        }

        QList<QVariantMap> comments;
        QJsonArray commentsArray = root.value(QStringLiteral("data")).toArray();
        for (const QJsonValue& val : commentsArray) {
            QJsonObject obj = val.toObject();
            QVariantMap comment;
            comment["id"] = obj.value(QStringLiteral("id")).toString();
            comment["content"] = obj.value(QStringLiteral("content")).toString();
            comment["author"] = obj.value(QStringLiteral("author")).toString();
            comment["likes"] = obj.value(QStringLiteral("likes")).toInt(0);
            comment["time"] = obj.value(QStringLiteral("time")).toVariant();
            comments.append(comment);
        }

        return comments;
    });
}

QFuture<bool> JizhileProvider::testConnection()
{
    return QtConcurrent::run([=]() -> bool {
        QUrl url(m_baseUrl + QStringLiteral("/health"));
        QNetworkRequest request = buildRequest(url.toString());
        auto [data, error] = sendRequest(request);
        
        if (error.hasError()) {
            qWarning() << "JizhileProvider: Connection test failed:" << error.message;
            return false;
        }

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull()) {
            QJsonObject root = doc.object();
            return root.value(QStringLiteral("status")).toString() == "ok";
        }
        return !data.isEmpty();
    });
}

void JizhileProvider::setToken(const QString& token)
{
    token_ = token;
}

QNetworkRequest JizhileProvider::buildRequest(const QString& endpointUrl)
{
    QUrl url(endpointUrl);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setHeader(QNetworkRequest::UserAgentHeader, 
                      QStringLiteral("MediaViralAssistant/1.0"));
    
    if (!token_.isEmpty()) {
        request.setRawHeader("Authorization", 
                            QStringLiteral("Bearer %1").arg(token_).toUtf8());
    }
    
    request.setRawHeader("X-Device-ID", QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8());
    request.setRawHeader("X-OS-Version", "iOS 17.0");
    
    return request;
}

Base::Error JizhileProvider::parseError(QNetworkReply* reply)
{
    Base::Error error;
    error.source = "jizhile";
    
    if (!reply) {
        error.type = Base::ErrorType::InternalError;
        error.message = QStringLiteral("Null reply pointer");
        return error;
    }
    
    switch (reply->error()) {
        case QNetworkReply::NoError:
            error.type = Base::ErrorType::NoError;
            break;
        case QNetworkReply::AuthenticationRequiredError:
        case QNetworkReply::AccessDenied:
            error.type = Base::ErrorType::AuthenticationError;
            error.message = QStringLiteral("Authentication failed");
            break;
        case QNetworkReply::ContentNotFoundError:
        case QNetworkReply::HostNotFoundError:
            error.type = Base::ErrorType::NotFoundError;
            error.message = reply->errorString();
            break;
        case QNetworkReply::TimeoutError:
        case QNetworkReply::OperationCanceledError:
            error.type = Base::ErrorType::TimeoutError;
            error.message = QStringLiteral("Request timeout");
            break;
        case QNetworkReply::TooManyRedirectsError:
        case QNetworkReply::TemporaryServerFailureError:
            error.type = Base::ErrorType::RateLimitError;
            error.message = QStringLiteral("Rate limit exceeded");
            break;
        default:
            if (reply->error() >= 100 && reply->error() < 200) {
                error.type = Base::ErrorType::NetworkError;
            } else if (reply->error() >= 400 && reply->error() < 500) {
                error.type = Base::ErrorType::ValidationError;
            } else if (reply->error() >= 500) {
                error.type = Base::ErrorType::InternalError;
            } else {
                error.type = Base::ErrorType::UnknownError;
            }
            error.message = reply->errorString();
            break;
    }
    
    error.errorCode = static_cast<int>(reply->error());
    
    QByteArray data = reply->readAll();
    if (!data.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull()) {
            QJsonObject root = doc.object();
            if (root.contains(QStringLiteral("message"))) {
                error.details = root.value(QStringLiteral("message")).toString();
            }
        }
    }
    
    return error;
}

QPair<QByteArray, Base::Error> JizhileProvider::sendRequest(const QNetworkRequest& request)
{
    QNetworkReply* reply = networkManager_->get(request);
    
    QEventLoop loop;
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&timeoutTimer, &QTimer::timeout, [&]() {
        if (reply->isRunning()) {
            reply->abort();
        }
        loop.quit();
    });
    
    timeoutTimer.start(30000);
    loop.exec();
    
    if (reply->error() != QNetworkReply::NoError) {
        Base::Error error = parseError(reply);
        reply->deleteLater();
        return {QByteArray(), error};
    }
    
    QByteArray data = reply->readAll();
    reply->deleteLater();
    
    return {data, Base::Error{}};
}

void JizhileProvider::onNetworkReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "JizhileProvider: Network error:" << reply->errorString();
    }
}

} // namespace MediaViralAssistant::Plugins
