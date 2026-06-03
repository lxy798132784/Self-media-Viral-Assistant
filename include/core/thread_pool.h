#ifndef MEDIA_VIRAL_ASSISTANT_CORE_THREAD_POOL_H
#define MEDIA_VIRAL_ASSISTANT_CORE_THREAD_POOL_H

#include <QObject>
#include <QThreadPool>
#include <QFuture>
#include <functional>
#include <memory>

namespace mva::core {

/**
 * @brief 全局线程池管理器，提供统一的并发任务执行环境
 *        Global thread pool manager providing unified concurrent task execution environment
 *
 * @details 基于 Qt 线程池实现，支持任务优先级、超时控制、任务取消等功能。
 *          自动调整线程数量以适应系统负载。
 *          Implemented based on Qt thread pool, supports task priority, timeout control, 
 *          task cancellation. Automatically adjusts thread count based on system load.
 *
 * @threadsafe 线程安全 / Thread-safe
 */
class ThreadPool : public QObject {
    Q_OBJECT

public:
    using Task = std::function<void()>;
    using TaskResult = std::function<QVariant()>;

    static ThreadPool& instance();

    void initialize(int maxThreads = -1);
    void shutdown();

    void execute(Task task, int priority = 0);
    QFuture<QVariant> run(TaskResult task, int priority = 0);
    
    template<typename T>
    QFuture<T> runTyped(std::function<T()> task, int priority = 0) {
        return QtConcurrent::run([task = std::move(task)]() -> T {
            return task();
        });
    }

    int getMaxThreadCount() const;
    void setMaxThreadCount(int count);
    
    int getActiveThreadCount() const;
    int getPendingTaskCount() const;

    bool waitForAllTasksFinished(int timeout = -1);
    void clear();

signals:
    void taskStarted(const QString& taskId);
    void taskCompleted(const QString& taskId);
    void taskFailed(const QString& taskId, const QString& error);

private:
    explicit ThreadPool(QObject* parent = nullptr);
    ~ThreadPool() override;

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::core

#endif
