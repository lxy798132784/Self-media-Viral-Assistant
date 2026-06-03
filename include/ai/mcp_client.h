#ifndef MCP_CLIENT_H
#define MCP_CLIENT_H
#include "ai/ai_provider_base.h"
namespace mva::ai { class McpClient : public QObject { Q_OBJECT public: explicit McpClient(QObject *parent = nullptr); ~McpClient(); bool connect(const QString &endpoint); QFuture<std::expected<QVariant, AiError>> callTool(const QString &toolName, const QVariantMap &params); }; }
#endif
