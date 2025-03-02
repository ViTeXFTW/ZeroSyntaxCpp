#include "protocol/json_rpc_handler.hpp"
#include "utils/logger.hpp"
#include <sstream>

namespace ZeroSyntax {
    
JsonRpcHandler::JsonRpcHandler() {}

void JsonRpcHandler::registerMethod(const std::string& method, MessageCallback callback) {
    methodHandlers_[method] = std::move(callback);
    LOG_DEBUG("Registered method handler: {}", method);
}

std::optional<std::string> JsonRpcHandler::handleRequest(const std::string& message) {
    try {
        nlohmann::json jsonRequest = nlohmann::json::parse(message);
        
        // Check if this is a valid JSON-RPC request
        if (!jsonRequest.contains("jsonrpc") || jsonRequest["jsonrpc"] != "2.0") {
            return createErrorResponse(-32600, "Invalid Request", nlohmann::json(nullptr), {});
        }
        
        std::string method;
        if (jsonRequest.contains("method")) {
            method = jsonRequest["method"].get<std::string>();
        } else {
            return createErrorResponse(-32600, "Method not specified", nlohmann::json(nullptr), {});
        }
        
        // Get params if they exist
        nlohmann::json params = nlohmann::json::object();
        if (jsonRequest.contains("params")) {
            params = jsonRequest["params"];
        }
        
        // Check if we have an id (request vs notification)
        bool hasId = jsonRequest.contains("id");
        nlohmann::json id = hasId ? jsonRequest["id"] : nullptr;
        
        LOG_DEBUG("Received {} for method: {}", hasId ? "request" : "notification", method);
        
        // Find and execute the handler for the method
        auto it = methodHandlers_.find(method);
        if (it != methodHandlers_.end()) {
            try {
                nlohmann::json result = it->second(params);
            
                // Only send a response if there was an id (request, not notification)
                if (hasId) {
                    return createResponse(result, id);
                }
            } catch (const std::exception& e) {
                LOG_ERROR("Error handling method {}: {}", method, e.what());
                if (hasId) {
                    return createErrorResponse(-32603, "Internal error", id, e.what());
                }
            }
            return std::nullopt;
        } else {
            LOG_WARN("Method not found: {}", method);
            if (hasId) {
                return createErrorResponse(-32601, "Method not found", id, {});
            }
            return std::nullopt;
        }
    } catch (const nlohmann::json::parse_error& e) {
        LOG_ERROR("JSON parse error: {}", e.what());
        return createErrorResponse(-32700, "Parse error", nlohmann::json(nullptr), e.what());
    } catch (const std::exception& e) {
        LOG_ERROR("Error processing request: {}", e.what());
        return createErrorResponse(-32603, "Internal error", nlohmann::json(nullptr), e.what());
    }
}

std::string JsonRpcHandler::createResponse(const nlohmann::json& result, const nlohmann::json& id) {
    nlohmann::json response = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"result", result}
    };
    return response.dump();
}

std::string JsonRpcHandler::createErrorResponse(int code, const std::string& message, 
                                                const nlohmann::json& id, 
                                                const nlohmann::json& data) {
    nlohmann::json error = {
        {"code", code},
        {"message", message}
    };
    
    if (!data.is_null()) {
        error["data"] = data;
    }
    
    nlohmann::json response = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"error", error}
    };
    
    return response.dump();
}

} // namespace ZeroSyntax