#ifndef CONTENT_ANALYZER_H
#define CONTENT_ANALYZER_H
#include "ai/ai_provider_base.h"
namespace mva::ai { class ContentAnalyzer : public QObject { Q_OBJECT public: explicit ContentAnalyzer(QObject *parent = nullptr); ~ContentAnalyzer(); QFuture<std::expected<QVariantMap, AiError>> analyzeViralPotential(const QString &content); QFuture<std::expected<QVariantList, AiError>> extractKeywords(const QString &content); }; }
#endif
