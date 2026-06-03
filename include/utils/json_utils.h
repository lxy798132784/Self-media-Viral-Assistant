#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <QJsonDocument>
#include <QByteArray>
#include <QVariant>
namespace mva::utils { class JsonUtils { public: static QVariant parseJson(const QByteArray &json); static QByteArray toJson(const QVariant &data); static bool isValid(const QByteArray &json); }; }
#endif
