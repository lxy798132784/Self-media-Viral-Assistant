#include "utils/json_utils.h"
namespace mva::utils {
QVariant JsonUtils::parseJson(const QByteArray &json) { return QJsonDocument::fromJson(json).toVariant(); }
QByteArray JsonUtils::toJson(const QVariant &data) { return QJsonDocument::fromVariant(data).toJson(); }
bool JsonUtils::isValid(const QByteArray &json) { QJsonParseError err; QJsonDocument::fromJson(json, &err); return err.error == QJsonParseError::NoError; }
} // namespace mva::utils

cat > /workspace/include/utils/json_utils.h << 'EOF'
#ifndef JSON_UTILS_H
#define JSON_UTILS_H
#include <QJsonDocument>
#include <QByteArray>
#include <QVariant>
namespace mva::utils {
class JsonUtils {
public:
    static QVariant parseJson(const QByteArray &json);
    static QByteArray toJson(const QVariant &data);
    static bool isValid(const QByteArray &json);
};
} // namespace mva::utils
#endif
