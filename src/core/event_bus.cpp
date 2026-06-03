/**
 * @file event_bus.cpp
 * @brief 事件总线实现
 *        Event bus implementation
 * 
 * @details 实现发布/订阅模式的事件总线，支持异步事件分发。
 *          Implements publish/subscribe event bus with async dispatch.
 */

#include "core/event_bus.h"
#include "utils/logger.h"

#include <QMutexLocker>
#include <QCoreApplication>
#include <QMetaObject>

namespace mva::core {

struct EventBus::Impl {
    QMap<QString, QList<QObject*>> subscribers;  ///< 订阅者映射 / Subscriber map
    QMutex mutex;                                 ///< 线程安全锁 / Thread safety lock
};

EventBus::EventBus(QObject* parent) : QObject(parent), d_(new Impl) {
}

EventBus::~EventBus() = default;

EventBus& EventBus::instance() {
    static EventBus instance;
    return instance;
}

void EventBus::subscribe(const QString& event, QObject* receiver, const char* slot) {
    QMutexLocker locker(&d_->mutex);
    
    if (!d_->subscribers.contains(event)) {
        d_->subscribers[event] = QList<QObject*>();
    }
    
    d_->subscribers[event].append(receiver);
    
    // 连接信号槽 / Connect signal to slot
    connect(this, SIGNAL(eventEmitted(QString, QVariant)), 
            receiver, slot, Qt::QueuedConnection);
    
    LOG_DEBUG << "Subscribed to event: " << event;
}

void EventBus::unsubscribe(const QString& event, QObject* receiver) {
    QMutexLocker locker(&d_->mutex);
    
    if (!d_->subscribers.contains(event)) {
        return;
    }
    
    d_->subscribers[event].removeAll(receiver);
    
    if (d_->subscribers[event].isEmpty()) {
        d_->subscribers.remove(event);
    }
    
    LOG_DEBUG << "Unsubscribed from event: " << event;
}

void EventBus::publish(const QString& event, const QVariant& data) {
    QMutexLocker locker(&d_->mutex);
    
    if (!d_->subscribers.contains(event)) {
        LOG_DEBUG << "No subscribers for event: " << event;
        return;
    }
    
    // 异步发送到所有订阅者 / Send asynchronously to all subscribers
    QList<QObject*> receivers = d_->subscribers[event];
    locker.unlock();
    
    for (QObject* receiver : receivers) {
        QMetaObject::invokeMethod(receiver, "onEvent", Qt::QueuedConnection,
                                  Q_ARG(QString, event), Q_ARG(QVariant, data));
    }
    
    emit eventEmitted(event, data);
}

void EventBus::clear() {
    QMutexLocker locker(&d_->mutex);
    d_->subscribers.clear();
    LOG_INFO << "Event bus cleared";
}

} // namespace mva::core
