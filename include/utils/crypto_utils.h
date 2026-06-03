#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
namespace mva::utils { class CryptoUtils { public: static QByteArray hashSha256(const QByteArray &data); static QByteArray hashMd5(const QByteArray &data); static QString base64Encode(const QByteArray &data); static QByteArray base64Decode(const QString &data); }; }
#endif
