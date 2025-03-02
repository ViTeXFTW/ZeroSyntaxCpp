#pragma once

#include <nlohmann/json.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <optional>

namespace ZeroSyntax {

class JsonRpcHandler {
public:
    using MessageCallback = std::function<nlohmann::json(const nlohmann::json&)>;

    JsonRpcHandler();

    // Register a method handler
    void registerMethod(const std::string& method, MessageCallback callback);

    // Process a JSON-RPC message
    std::optional<std::string> handleRequest(const std::string& message);
    std::string createResponse(const nlohmann::json& result, const nlohmann::json& id);
    std::string createErrorResponse(int code, const std::string& message, const nlohmann::json& id, const nlohmann::json& data = nullptr);

private:
    std::unordered_map<std::string, MessageCallback> methodHandlers_;
};

} // namespace ZeroSyntax