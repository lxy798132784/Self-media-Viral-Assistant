#include "ai/mcp_client.h"
namespace mva::ai {
McpClient::McpClient(QObject *parent) : QObject(parent) {}
McpClient::~McpClient() = default;
bool McpClient::connect(const QString &endpoint) { Q_UNUSED(endpoint); return true; }
QFuture<std::expected<QVariant, AiError>> McpClient::callTool(const QString &toolName, const QVariantMap &params) {
    return QtConcurrent::run([](){ return QVariant(); });
}
} // namespace mva::ai

cat > /workspace/include/ai/mcp_client.h << 'EOF'
#ifndef MCP_CLIENT_H
#define MCP_CLIENT_H
#include "ai/ai_provider_base.h"
namespace mva::ai {
class McpClient : public QObject {
    Q_OBJECT
public:
    explicit McpClient(QObject *parent = nullptr);
    ~McpClient();
    bool connect(const QString &endpoint);
    QFuture<std::expected<QVariant, AiError>> callTool(const QString &toolName, const QVariantMap &params);
};
} // namespace mva::ai
#endif
