#ifndef METRICS_MODEL_H
#define METRICS_MODEL_H
#include "database/database_manager.h"
namespace mva::database { class MetricsModel : public QObject { Q_OBJECT public: explicit MetricsModel(QObject *parent = nullptr); ~MetricsModel(); QFuture<std::expected<QVariantMap, DatabaseError>> getMetrics(const QString &contentId); QFuture<std::expected<bool, DatabaseError>> updateMetrics(const QString &contentId, const QVariantMap &metrics); }; }
#endif
