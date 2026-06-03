#ifndef SKILL_REGISTRY_H
#define SKILL_REGISTRY_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>
#include <functional>
namespace mva::ai { class SkillRegistry : public QObject { Q_OBJECT public: explicit SkillRegistry(QObject *parent = nullptr); ~SkillRegistry(); void registerSkill(const QString &name, std::function<QVariant(QVariant)> skill); QVariant executeSkill(const QString &name, const QVariant &input); private: QMap<QString, std::function<QVariant(QVariant)>> m_skills; }; }
#endif
