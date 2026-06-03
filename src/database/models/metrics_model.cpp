#include "database/models/metrics_model.h"
namespace mva::database {
MetricsModel::MetricsModel(QObject *parent) : QObject(parent) {}
MetricsModel::~MetricsModel() = default;
QFuture<std::expected<QVariantMap, DatabaseError>> MetricsModel::getMetrics(const QString &contentId) {
    return QtConcurrent::run([](){ return QVariantMap{}; });
}
QFuture<std::expected<bool, DatabaseError>> MetricsModel::updateMetrics(const QString &contentId, const QVariantMap &metrics) {
    return QtConcurrent::run([](){ return true; });
}
} // namespace mva::database

cat > /workspace/include/database/models/metrics_model.h << 'EOF'
#ifndef METRICS_MODEL_H
#define METRICS_MODEL_H
#include "database/database_manager.h"
namespace mva::database {
class MetricsModel : public QObject {
    Q_OBJECT
public:
    explicit MetricsModel(QObject *parent = nullptr);
    ~MetricsModel();
    QFuture<std::expected<QVariantMap, DatabaseError>> getMetrics(const QString &contentId);
    QFuture<std::expected<bool, DatabaseError>> updateMetrics(const QString &contentId, const QVariantMap &metrics);
};
} // namespace mva::database
#endif
