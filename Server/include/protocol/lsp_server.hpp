#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

#include "core/document_manager.hpp"

namespace ZeroSyntax {

class JsonRpcHandler {
public:
    using MessageCallback = std::function<nlohmann::json(const nlohmann::json&)>;
    
    JsonRpcHandler();
    
    // Register a method handler
    void registerMethod(const std::string& method, MessageCallback callback);
    
    // Process a JSON-RPC message and return a response if needed
    std::optional<std::string> handleRequest(const std::string& message);
    
    // Create a JSON-RPC response
    std::string createResponse(const nlohmann::json& result, const nlohmann::json& id);
    
    // Create a JSON-RPC error response
    std::string createErrorResponse(int code, const std::string& message,
                                    const nlohmann::json& id,
                                    const nlohmann::json& data = nullptr);
private:
    std::unordered_map<std::string, MessageCallback> methodHandlers_;
};

class LspServer {
public:
    LspServer();

    // Process a single JSON-RPC message
    std::optional<std::string> processMessage(const std::string& message);

    // Run the server's message processing loop
    void run();

private:
    // Handler methods for LSP notifications and requests
    nlohmann::json handleInitialize(const nlohmann::json& params);
    nlohmann::json handleTextDocumentDidOpen(const nlohmann::json& params);
    nlohmann::json handleTextDocumentDidChange(const nlohmann::json& params);
    nlohmann::json handleTextDocumentDidClose(const nlohmann::json& params);
    nlohmann::json handleTextDocumentCompletion(const nlohmann::json& params);
    nlohmann::json handleTextDocumentDefinition(const nlohmann::json& params);
    
    // Helper method to publish diagnostics
    void publishDiagnostics(const std::string& uri, const std::vector<LSP::Diagnostic>& diagnostics);
    
    // Send a notification to the client
    void sendNotification(const std::string& method, const nlohmann::json& params);
    
    // Member variables
    std::unique_ptr<JsonRpcHandler> rpcHandler_;
    std::unique_ptr<DocumentManager> documentManager_;
};
    


} // namespace ZeroSyntax