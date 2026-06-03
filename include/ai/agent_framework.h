#ifndef AGENT_FRAMEWORK_H
#define AGENT_FRAMEWORK_H
#include "ai/ai_provider_base.h"
#include <QMap>
namespace mva::ai { class AgentFramework : public QObject { Q_OBJECT public: explicit AgentFramework(QObject *parent = nullptr); ~AgentFramework(); void registerAgent(const QString &name, QObject *agent); QFuture<std::expected<QString, AiError>> executeTask(const QString &taskType, const QVariantMap &input); private: QMap<QString, QObject*> m_agents; }; }
#endif
