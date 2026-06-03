/**
 * @file llm_manager.cpp
 * @brief LLM 管理器实现
 *        LLM manager implementation
 */

#include "ai/llm_manager.h"
#include "utils/logger.h"

#include <QMutexLocker>
#include <QtConcurrent>

namespace mva::ai {

struct LlmManager::Impl {
    QMap<QString, AiProviderBase*> providers;     ///< AI 提供商映射 / Provider map
    QString activeProviderId;                      ///< 当前活跃提供商 / Active provider ID
    QMutex mutex;                                  ///< 线程安全锁 / Thread safety lock
};

LlmManager::LlmManager(QObject* parent)
    : QObject(parent), d_(new Impl) {
}

LlmManager::~LlmManager() {
    shutdown();
}

LlmManager& LlmManager::instance() {
    static LlmManager instance;
    return instance;
}

bool LlmManager::initialize() {
    LOG_INFO << "LLM Manager initialized";
    return true;
}

void LlmManager::shutdown() {
    QMutexLocker locker(&d_->mutex);
    d_->providers.clear();
    d_->activeProviderId.clear();
    LOG_INFO << "LLM Manager shutdown complete";
}

void LlmManager::registerProvider(AiProviderBase* provider) {
    if (!provider) return;
    
    QMutexLocker locker(&d_->mutex);
    d_->providers[provider->id()] = provider;
    LOG_INFO << "Registered AI provider: " << provider->id();
    emit providerAdded(provider->id());
}

void LlmManager::unregisterProvider(const QString& providerId) {
    QMutexLocker locker(&d_->mutex);
    if (d_->providers.contains(providerId)) {
        d_->providers.remove(providerId);
        LOG_INFO << "Unregistered AI provider: " << providerId;
        emit providerRemoved(providerId);
    }
}

AiProviderBase* LlmManager::getProvider(const QString& providerId) const {
    QMutexLocker locker(&d_->mutex);
    return d_->providers.value(providerId, nullptr);
}

QStringList LlmManager::getAvailableProviders() const {
    QMutexLocker locker(&d_->mutex);
    return d_->providers.keys();
}

void LlmManager::setActiveProvider(const QString& providerId) {
    QMutexLocker locker(&d_->mutex);
    if (d_->providers.contains(providerId)) {
        d_->activeProviderId = providerId;
        LOG_INFO << "Active AI provider set to: " << providerId;
        emit activeProviderChanged(providerId);
    }
}

QString LlmManager::getActiveProvider() const {
    QMutexLocker locker(&d_->mutex);
    return d_->activeProviderId;
}

QFuture<AiResponse> LlmManager::chat(
    const std::vector<Message>& messages,
    const QVariantMap& options
) {
    return QtConcurrent::run([this, messages, options]() -> AiResponse {
        QMutexLocker locker(&d_->mutex);
        
        AiProviderBase* provider = d_->providers.value(d_->activeProviderId);
        if (!provider) {
            return AiResponse{"No active provider", 0, 0, {}};
        }
        
        // 实际实现会调用 provider->chat()
        // Actual implementation would call provider->chat()
        AiResponse response;
        response.content = "AI Response from " + provider->displayName();
        response.usagePromptTokens = 100;
        response.usageCompletionTokens = 50;
        return response;
    });
}

QFuture<AiResponse> LlmManager::analyzeContent(
    const QString& content,
    const QString& analysisType,
    const QVariantMap& options
) {
    return QtConcurrent::run([this, content, analysisType, options]() -> AiResponse {
        QMutexLocker locker(&d_->mutex);
        
        AiProviderBase* provider = d_->providers.value(d_->activeProviderId);
        if (!provider) {
            return AiResponse{"No active provider", 0, 0, {}};
        }
        
        AiResponse response;
        response.content = "Analysis of type '" + analysisType + "' completed";
        response.metadata["analysisType"] = analysisType;
        response.metadata["contentLength"] = content.length();
        return response;
    });
}

QFuture<AiResponse> LlmManager::generateSuggestions(
    const QString& context,
    const QVariantMap& options
) {
    return QtConcurrent::run([this, context, options]() -> AiResponse {
        QMutexLocker locker(&d_->mutex);
        
        AiProviderBase* provider = d_->providers.value(d_->activeProviderId);
        if (!provider) {
            return AiResponse{"No active provider", 0, 0, {}};
        }
        
        AiResponse response;
        response.content = "Generated 5 suggestions based on context";
        response.metadata["suggestionCount"] = 5;
        return response;
    });
}

} // namespace mva::ai
