#ifndef AI_PROVIDER_BASE_H
#define AI_PROVIDER_BASE_H
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QFuture>
#include <expected>
namespace mva::ai {
enum class AiError { Success = 0, NetworkError, AuthenticationFailed, RateLimitExceeded, ModelNotFound, UnknownError };
class AiProviderBase : public QObject {
    Q_OBJECT
public:
    explicit AiProviderBase(QObject *parent = nullptr);
    ~AiProviderBase() override;
    [[nodiscard]] virtual QString providerName() const = 0;
    [[nodiscard]] virtual bool initialize(const QVariantMap &config);
    [[nodiscard]] virtual QFuture<std::expected<QString, AiError>> generateText(const QString &prompt, const QString &systemPrompt = "");
    [[nodiscard]] virtual QFuture<std::expected<QVariantList, AiError>> analyzeContent(const QString &content);
    [[nodiscard]] bool isAvailable() const { return m_available; }
signals:
    void availabilityChanged(bool available);
protected:
    bool m_available{false};
    void setAvailable(bool available);
};
} // namespace mva::ai
#endif
