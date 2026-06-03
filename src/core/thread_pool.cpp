/**
 * @file thread_pool.cpp
 * @brief 线程池实现
 *        Thread pool implementation
 * 
 * @details 实现可配置的工作线程池，支持任务优先级、超时控制。
 *          Implements configurable worker thread pool with task priority, timeout control.
 */

#include "core/thread_pool.h"
#include "utils/logger.h"

#include <QThreadPool>
#include <QRunnable>
#include <QFuture>
#include <QtConcurrent>

namespace mva::core {

struct ThreadPool::Impl {
    QThreadPool* pool = nullptr;           ///< Qt 线程池 / Qt thread pool
    int maxThreads = QThread::idealThreadCount();  ///< 最大线程数 / Max threads
};

ThreadPool::ThreadPool(QObject* parent) : QObject(parent), d_(new Impl) {
    d_->pool = QThreadPool::globalInstance();
    d_->pool->setMaxThreadCount(d_->maxThreads);
}

ThreadPool::~ThreadPool() = default;

ThreadPool& ThreadPool::instance() {
    static ThreadPool instance;
    return instance;
}

void ThreadPool::initialize(int maxThreads) {
    d_->maxThreads = maxThreads > 0 ? maxThreads : QThread::idealThreadCount();
    d_->pool->setMaxThreadCount(d_->maxThreads);
    LOG_INFO << "Thread pool initialized with " << d_->maxThreads << " threads";
}

int ThreadPool::execute(std::function<void()> task, int priority) {
    // 创建可运行对象 / Create runnable object
    auto* runnable = new FunctionRunnable(task);
    d_->pool->start(runnable, priority);
    return 0;
}

QFuture<void> ThreadPool::runAsync(std::function<void()> task) {
    return QtConcurrent::run(task);
}

int ThreadPool::activeThreadCount() const {
    return d_->pool->activeThreadCount();
}

int ThreadPool::maxThreadCount() const {
    return d_->maxThreads;
}

void ThreadPool::waitForDone(int timeout) {
    d_->pool->waitForDone(timeout);
}

void ThreadPool::clear() {
    d_->pool->clear();
    LOG_DEBUG << "Thread pool cleared";
}

ThreadPool::FunctionRunnable::FunctionRunnable(std::function<void()> func)
    : func_(std::move(func)) {}

void ThreadPool::FunctionRunnable::run() {
    try {
        func_();
    } catch (const std::exception& e) {
        LOG_ERROR << "Task exception: " << e.what();
    }
}

} // namespace mva::core
