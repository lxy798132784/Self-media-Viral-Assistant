#ifndef MEDIA_VIRAL_ASSISTANT_AI_LLM_MANAGER_H
#define MEDIA_VIRAL_ASSISTANT_AI_LLM_MANAGER_H

#include "ai_provider_base.h"
#include <QObject>
#include <QMap>
#include <QString>
#include <memory>

namespace mva::ai {

/**
 * @brief LLM 管理器，统一管理多个大语言模型提供商
 *        LLM manager that unifies management of multiple LLM providers
 *
 * @details 支持动态添加/删除 AI 提供商，自动故障转移，负载均衡。
 *          提供统一的对话、分析、生成接口。
 *          Supports dynamic add/remove AI providers, automatic failover, load balancing.
 *          Provides unified chat, analysis, generation interfaces.
 */
class LlmManager : public QObject {
    Q_OBJECT

public:
    static LlmManager& instance();

    bool initialize();
    void shutdown();

    void registerProvider(AiProviderBase* provider);
    void unregisterProvider(const QString& providerId);
    
    AiProviderBase* getProvider(const QString& providerId) const;
    QStringList getAvailableProviders() const;
    
    void setActiveProvider(const QString& providerId);
    QString getActiveProvider() const;

    QFuture<AiResponse> chat(
        const std::vector<Message>& messages,
        const QVariantMap& options = {}
    );

    QFuture<AiResponse> analyzeContent(
        const QString& content,
        const QString& analysisType,
        const QVariantMap& options = {}
    );

    QFuture<AiResponse> generateSuggestions(
        const QString& context,
        const QVariantMap& options = {}
    );

signals:
    void providerAdded(const QString& providerId);
    void providerRemoved(const QString& providerId);
    void activeProviderChanged(const QString& providerId);

private:
    explicit LlmManager(QObject* parent = nullptr);
    ~LlmManager() override;

    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::ai

#endif
