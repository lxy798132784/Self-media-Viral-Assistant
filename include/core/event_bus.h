#ifndef MEDIA_VIRAL_ASSISTANT_CORE_EVENT_BUS_H
#define MEDIA_VIRAL_ASSISTANT_CORE_EVENT_BUS_H

#include <QObject>
#include <QMap>
#include <QString>
#include <functional>
#include <memory>
#include <vector>

namespace mva::core {

/**
 * @brief 全局事件总线，用于组件间解耦通信
 *        Global event bus for decoupled inter-component communication
 *
 * @details 发布/订阅模式实现，支持同步和异步事件分发。
 *          提供事件过滤、优先级处理、线程安全等功能。
 *          Publish/Subscribe pattern implementation supporting sync and async 
 *          event dispatch. Provides event filtering, priority handling, thread safety.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class EventBus : public QObject {
    Q_OBJECT

public:
    using EventHandler = std::function<void(const QVariant&)>;
    using AsyncEventHandler = std::function<QFuture<void>(const QVariant&)>;

    static EventBus& instance();

    void subscribe(const QString& event, EventHandler handler, int priority = 0);
    void subscribeAsync(const QString& event, AsyncEventHandler handler, int priority = 0);
    void unsubscribe(const QString& event, void* subscriber);
    void unsubscribeAll(const QString& event);

    void publish(const QString& event, const QVariant& data = {});
    QFuture<void> publishAsync(const QString& event, const QVariant& data = {});

    bool hasSubscribers(const QString& event) const;
    int subscriberCount(const QString& event) const;

signals:
    void eventPublished(const QString& event, const QVariant& data);

private:
    explicit EventBus(QObject* parent = nullptr);
    ~EventBus() override;

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::core

#endif
