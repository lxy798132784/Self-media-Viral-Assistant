#include "ai/skill_registry.h"
namespace mva::ai {
SkillRegistry::SkillRegistry(QObject *parent) : QObject(parent) {}
SkillRegistry::~SkillRegistry() = default;
void SkillRegistry::registerSkill(const QString &name, std::function<QVariant(QVariant)> skill) { m_skills[name] = skill; }
QVariant SkillRegistry::executeSkill(const QString &name, const QVariant &input) {
    auto it = m_skills.find(name);
    if (it != m_skills.end()) { return it.value()(input); }
    return QVariant();
}
} // namespace mva::ai

cat > /workspace/include/ai/skill_registry.h << 'EOF'
#ifndef SKILL_REGISTRY_H
#define SKILL_REGISTRY_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <functional>
namespace mva::ai {
class SkillRegistry : public QObject {
    Q_OBJECT
public:
    explicit SkillRegistry(QObject *parent = nullptr);
    ~SkillRegistry();
    void registerSkill(const QString &name, std::function<QVariant(QVariant)> skill);
    QVariant executeSkill(const QString &name, const QVariant &input);
private:
    QMap<QString, std::function<QVariant(QVariant)>> m_skills;
};
} // namespace mva::ai
#endif
