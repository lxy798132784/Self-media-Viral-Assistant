#include "utils/http_client.h"
namespace mva::utils {
HttpClient::HttpClient(QObject *parent) : QObject(parent) {}
HttpClient::~HttpClient() = default;
QFuture<std::expected<QByteArray, HttpError>> HttpClient::get(const QString &url, const QVariantMap &headers) {
    Q_UNUSED(url); Q_UNUSED(headers);
    return QtConcurrent::run([](){ return QByteArray{}; });
}
QFuture<std::expected<QByteArray, HttpError>> HttpClient::post(const QString &url, const QByteArray &data, const QVariantMap &headers) {
    Q_UNUSED(url); Q_UNUSED(data); Q_UNUSED(headers);
    return QtConcurrent::run([](){ return QByteArray{}; });
}
} // namespace mva::utils

cat > /workspace/include/utils/http_client.h << 'EOF'
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QFuture>
#include <expected>
namespace mva::utils {
enum class HttpError { Success = 0, NetworkError, Timeout, InvalidUrl, SslError };
class HttpClient : public QObject {
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient();
    QFuture<std::expected<QByteArray, HttpError>> get(const QString &url, const QVariantMap &headers = {});
    QFuture<std::expected<QByteArray, HttpError>> post(const QString &url, const QByteArray &data, const QVariantMap &headers = {});
};
} // namespace mva::utils
#endif
