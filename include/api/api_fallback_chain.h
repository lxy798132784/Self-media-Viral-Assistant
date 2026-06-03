#ifndef MEDIA_VIRAL_ASSISTANT_API_API_FALLBACK_CHAIN_H
#define MEDIA_VIRAL_ASSISTANT_API_API_FALLBACK_CHAIN_H

#include "data_source_base.h"
#include <QObject>
#include <QVector>
#include <memory>

namespace mva::api {

/**
 * @brief API Fallback 链，实现多级容错机制
 *        API Fallback chain implementing multi-level fault tolerance
 *
 * @details 当主数据源失败时，自动尝试备用数据源。支持优先级配置、
 *          健康检查、失败计数等功能。确保数据获取的高可用性。
 *          Automatically tries backup data sources when primary fails. 
 *          Supports priority configuration, health checks, failure counting.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class ApiFallbackChain : public QObject {
    Q_OBJECT

public:
    explicit ApiFallbackChain(QObject* parent = nullptr);
    ~ApiFallbackChain() override;

    void addProvider(DataSourceBase* provider, int priority = 0);
    void removeProvider(const QString& providerId);
    
    QFuture<MetricsResult> fetchMetricsWithFallback(
        const QString& url,
        const QString& platform
    );
    
    QFuture<ContentResult> fetchContentWithFallback(
        const QString& url,
        const QString& platform,
        bool includeComments = false
    );

    void setMaxRetries(int maxRetries);
    void setTimeoutMs(int timeoutMs);
    
    QStringList getProviderChain() const;
    QString getActiveProvider() const;

signals:
    void providerFailed(const QString& providerId, const QString& error);
    void providerSwitched(const QString& fromProvider, const QString& toProvider);
    void allProvidersFailed(const QString& url);

private:
    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::api

#endif
