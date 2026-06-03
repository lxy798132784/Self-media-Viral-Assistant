/**
 * @file openai_provider.h
 * @brief OpenAI LLM 提供商插件头文件
 *        OpenAI LLM Provider Plugin Header
 *
 * @details 支持接入 OpenAI GPT 系列模型，提供内容分析、拆解、建议生成功能。
 *          Supports OpenAI GPT series models for content analysis, breakdown, and suggestion generation.
 */

#ifndef OPENAI_PROVIDER_H
#define OPENAI_PROVIDER_H

#include "core/base/plugin_interface.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QString>
#include <vector>

namespace MediaViralAssistant::Plugins {

/**
 * @brief OpenAI AI 提供商插件类
 *        OpenAI AI Provider Plugin Class
 */
class OpenAiProvider : public QObject, public IAiProviderPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "media-assistant.ai.openai" FILE "plugin.json")
    Q_INTERFACES(MediaViralAssistant::Plugins::IAiProviderPlugin)

public:
    /**
     * @brief 构造函数 / Constructor
     * @param parent 父对象 / Parent object
     */
    explicit OpenAiProvider(QObject* parent = nullptr);

    /**
     * @brief 析构函数 / Destructor
     */
    ~OpenAiProvider() override;

    /**
     * @brief 获取提供商名称 / Get provider name
     * @return 提供商名称 / Provider name
     */
    [[nodiscard]] QString name() const override;

    /**
     * @brief 设置 API Key / Set API Key
     * @param key OpenAI API Key
     */
    void setApiKey(const QString& key);

    /**
     * @brief 设置模型名称 / Set model name
     * @param model 模型名称 / Model name (e.g., "gpt-4", "gpt-3.5-turbo")
     */
    void setModel(const QString& model);

    /**
     * @brief 分析爆款内容 / Analyze viral content
     * @param content 内容文本 / Content text
     * @param metrics 数据指标 / Metrics data
     * @param dimensions 分析维度 / Analysis dimensions
     * @return 分析结果 / Analysis result
     */
    QFuture<std::expected<AiAnalysisResult, ApiError>>
    analyzeContent(const QString& content, 
                   const ContentMetrics& metrics,
                   const QStringList& dimensions) override;

    /**
     * @brief 生成选题建议 / Generate topic suggestions
     * @param analyzedData 已分析的数据 / Analyzed data
     * @param platform 目标平台 / Target platform
     * @param count 建议数量 / Suggestion count
     * @return 选题建议列表 / Topic suggestions list
     */
    QFuture<std::expected<QList<TopicSuggestion>, ApiError>>
    generateTopicSuggestions(const AiAnalysisResult& analyzedData,
                             const QString& platform,
                             int count) override;

    /**
     * @brief 生成写作 SOP / Generate writing SOP
     * @param topic 选题 / Topic
     * @param targetAudience 目标受众 / Target audience
     * @param style 写作风格 / Writing style
     * @return 写作 SOP / Writing SOP
     */
    QFuture<std::expected<WritingSOP, ApiError>>
    generateWritingSOP(const QString& topic,
                       const QString& targetAudience,
                       const QString& style) override;

private slots:
    /**
     * @brief 网络请求完成处理 / Handle network request completion
     * @param reply 网络回复对象 / Network reply object
     */
    void onNetworkReplyFinished(QNetworkReply* reply);

private:
    /**
     * @brief 构建 API 请求头 / Build API request headers
     * @return 网络请求对象 / Network request object
     */
    [[nodiscard]] QNetworkRequest buildRequest() const;

    /**
     * @brief 发送聊天完成请求 / Send chat completion request
     * @param messages 消息列表 / Messages list
     * @return 网络回复对象 / Network reply object
     */
    QNetworkReply* sendChatCompletionRequest(const QJsonArray& messages);

    QNetworkAccessManager* networkManager_;
    QString apiKey_;
    QString model_;
};

} // namespace MediaViralAssistant::Plugins

#endif // OPENAI_PROVIDER_H
