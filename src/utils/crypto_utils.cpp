#include "utils/crypto_utils.h"
namespace mva::utils {
QByteArray CryptoUtils::hashSha256(const QByteArray &data) { return QCryptographicHash::hash(data, QCryptographicHash::Sha256); }
QByteArray CryptoUtils::hashMd5(const QByteArray &data) { return QCryptographicHash::hash(data, QCryptographicHash::Md5); }
QString CryptoUtils::base64Encode(const QByteArray &data) { return data.toBase64(); }
QByteArray CryptoUtils::base64Decode(const QString &data) { return QByteArray::fromBase64(data.toUtf8()); }
} // namespace mva::utils

cat > /workspace/include/utils/crypto_utils.h << 'EOF'
#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
namespace mva::utils {
class CryptoUtils {
public:
    static QByteArray hashSha256(const QByteArray &data);
    static QByteArray hashMd5(const QByteArray &data);
    static QString base64Encode(const QByteArray &data);
    static QByteArray base64Decode(const QString &data);
};
} // namespace mva::utils
#endif
