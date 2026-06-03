#include "ai/ai_provider_base.h"
namespace mva::ai {
AiProviderBase::AiProviderBase(QObject *parent) : QObject(parent) {}
AiProviderBase::~AiProviderBase() = default;
bool AiProviderBase::initialize(const QVariantMap &config) { Q_UNUSED(config); setAvailable(true); return true; }
QFuture<std::expected<QString, AiError>> AiProviderBase::generateText(const QString &, const QString &) {
    return QtConcurrent::run([](){ return QString{}; });
}
QFuture<std::expected<QVariantList, AiError>> AiProviderBase::analyzeContent(const QString &) {
    return QtConcurrent::run([](){ return QVariantList{}; });
}
void AiProviderBase::setAvailable(bool available) { m_available = available; emit availabilityChanged(available); }
} // namespace mva::ai
