#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QFuture>
#include <expected>
namespace mva::utils { enum class HttpError { Success = 0, NetworkError, Timeout, InvalidUrl, SslError }; class HttpClient : public QObject { Q_OBJECT public: explicit HttpClient(QObject *parent = nullptr); ~HttpClient(); QFuture<std::expected<QByteArray, HttpError>> get(const QString &url, const QVariantMap &headers = {}); QFuture<std::expected<QByteArray, HttpError>> post(const QString &url, const QByteArray &data, const QVariantMap &headers = {}); }; }
#endif
