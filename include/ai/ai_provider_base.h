#ifndef MEDIA_VIRAL_ASSISTANT_AI_AI_PROVIDER_BASE_H
#define MEDIA_VIRAL_ASSISTANT_AI_AI_PROVIDER_BASE_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QFuture>
#include <memory>
#include <vector>

namespace mva::ai {

/**
 * @brief AI 提供商类型
 *        AI provider types
 */
enum class ProviderType {
    LLM,            ///< 大语言模型 / Large Language Model
    Agent,          ///< AI Agent / AI Agent
    Skill,          ///< 技能模块 / Skill module
    MCP             ///< Model Context Protocol / Model Context Protocol
};

/**
 * @brief AI 消息结构
 *        AI message structure
 */
struct Message {
    QString role;       ///< 角色 (system/user/assistant) / Role
    QString content;    ///< 消息内容 / Content
    QVariantMap extra;  ///< 额外数据 / Extra data
};

/**
 * @brief AI 响应结构
 *        AI response structure
 */
struct AiResponse {
    QString content;                ///< 响应内容 / Response content
    qreal usagePromptTokens = 0;    ///< 输入 token 数 / Input tokens
    qreal usageCompletionTokens = 0;///< 输出 token 数 / Output tokens
    QVariantMap metadata;           ///< 元数据 / Metadata
};

/**
 * @brief AI 提供商基类
 *        Base class for AI providers
 *
 * @details 统一的 AI 服务接口，支持多种 AI 提供商（OpenAI、Claude、
 *          通义千问等）。提供对话、分析、生成等功能。
 *          Unified AI service interface supporting multiple providers 
 *          (OpenAI, Claude, Qwen, etc.). Provides chat, analysis, generation.
 */
class AiProviderBase : public QObject {
    Q_OBJECT

public:
    explicit AiProviderBase(QObject* parent = nullptr);
    ~AiProviderBase() override;

    virtual QString id() const = 0;
    virtual QString displayName() const = 0;
    virtual ProviderType type() const = 0;

    virtual bool initialize(const QVariantMap& config) = 0;
    virtual bool isAvailable() const = 0;

    virtual QFuture<AiResponse> chat(
        const std::vector<Message>& messages,
        const QVariantMap& options = {}
    ) = 0;

    virtual QFuture<AiResponse> analyzeContent(
        const QString& content,
        const QString& analysisType,
        const QVariantMap& options = {}
    ) = 0;

    virtual QFuture<AiResponse> generateSuggestions(
        const QString& context,
        const QVariantMap& options = {}
    ) = 0;

signals:
    void availabilityChanged(bool available);
    void errorOccurred(const QString& error);

protected:
    struct Impl;
    std::unique_ptr<Impl> d_;
};

} // namespace mva::ai

#endif
