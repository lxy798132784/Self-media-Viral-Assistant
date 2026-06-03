#include "ai/agent_framework.h"
namespace mva::ai {
AgentFramework::AgentFramework(QObject *parent) : QObject(parent) {}
AgentFramework::~AgentFramework() = default;
void AgentFramework::registerAgent(const QString &name, QObject *agent) { m_agents[name] = agent; }
QFuture<std::expected<QString, AiError>> AgentFramework::executeTask(const QString &taskType, const QVariantMap &input) {
    return QtConcurrent::run([this, taskType, input](){ 
        auto it = m_agents.find(taskType);
        if (it != m_agents.end()) { return QString("Executed: %1").arg(taskType); }
        return QString{};
    });
}
} // namespace mva::ai

cat > /workspace/include/ai/agent_framework.h << 'EOF'
#ifndef AGENT_FRAMEWORK_H
#define AGENT_FRAMEWORK_H
#include "ai/ai_provider_base.h"
#include <QMap>
namespace mva::ai {
class AgentFramework : public QObject {
    Q_OBJECT
public:
    explicit AgentFramework(QObject *parent = nullptr);
    ~AgentFramework();
    void registerAgent(const QString &name, QObject *agent);
    QFuture<std::expected<QString, AiError>> executeTask(const QString &taskType, const QVariantMap &input);
private:
    QMap<QString, QObject*> m_agents;
};
} // namespace mva::ai
#endif
