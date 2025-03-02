// LanguageServer/src/protocol/lsp_server.cpp
#include "protocol/lsp_server.hpp"
#include "utils/logger.hpp"
#include <iostream>

namespace ZeroSyntax
{

    LspServer::LspServer()
        : rpcHandler_(std::make_unique<JsonRpcHandler>()),
          documentManager_(std::make_unique<DocumentManager>())
    {

        // Register LSP methods
        rpcHandler_->registerMethod("initialize", [this](const nlohmann::json &params)
                                    { return this->handleInitialize(params); });

        rpcHandler_->registerMethod("textDocument/didOpen", [this](const nlohmann::json &params)
                                    { return this->handleTextDocumentDidOpen(params); });

        rpcHandler_->registerMethod("textDocument/didChange", [this](const nlohmann::json &params)
                                    { return this->handleTextDocumentDidChange(params); });

        rpcHandler_->registerMethod("textDocument/didClose", [this](const nlohmann::json &params)
                                    { return this->handleTextDocumentDidClose(params); });

        rpcHandler_->registerMethod("textDocument/completion", [this](const nlohmann::json &params)
                                    { return this->handleTextDocumentCompletion(params); });

        rpcHandler_->registerMethod("textDocument/definition", [this](const nlohmann::json &params)
                                    { return this->handleTextDocumentDefinition(params); });

        LOG_INFO("LSP server initialized");
    }

    std::optional<std::string> LspServer::processMessage(const std::string &message)
    {
        return rpcHandler_->handleRequest(message);
    }

    void LspServer::run()
    {
        LOG_INFO("Starting LSP server");

        // Message processing loop
        std::string line;
        std::string content;
        int contentLength = -1;

        while (std::getline(std::cin, line))
        {
            // Remove carriage return if present
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }

            if (line.empty())
            {
                // End of headers, start reading content if length is known
                if (contentLength > 0)
                {
                    content.resize(contentLength);
                    std::cin.read(&content[0], contentLength);

                    LOG_DEBUG("Received message: {}", content);

                    // Process the message
                    auto response = processMessage(content);
                    if (response)
                    {
                        // Send the response
                        std::string responseMsg = *response;
                        LOG_DEBUG("Sending response: {}", responseMsg);
                        std::cout << "Content-Length: " << responseMsg.size() << "\r\n\r\n";
                        std::cout << responseMsg;
                        std::cout.flush();
                    }

                    // Reset for next message
                    contentLength = -1;
                    content.clear();
                }
            }
            else if (line.rfind("Content-Length: ", 0) == 0)
            {
                // Parse content length
                contentLength = std::stoi(line.substr(16));
                LOG_DEBUG("Content length: {}", contentLength);
            }
            // Ignore other headers for now
        }
    }

    nlohmann::json LspServer::handleInitialize(const nlohmann::json &params)
    {
        LOG_INFO("Handling initialize request");

        // Set up server capabilities
        nlohmann::json capabilities = {
            {"textDocumentSync", 1}, // 1 = full sync mode
            {"completionProvider", nlohmann::json::object()},
            {"definitionProvider", true}};

        nlohmann::json result = {
            {"capabilities", capabilities}};

        return result;
    }

    nlohmann::json LspServer::handleTextDocumentDidOpen(const nlohmann::json &params)
    {
        try
        {
            auto textDocument = params["textDocument"];
            std::string uri = textDocument["uri"];
            std::string text = textDocument["text"];
            std::string languageId = textDocument["languageId"];
            int version = textDocument["version"];

            LOG_INFO("Document opened: {}", uri);

            documentManager_->addDocument(uri, text, languageId);

            // Validate and publish diagnostics
            auto diagnostics = documentManager_->validateDocument(uri);
            publishDiagnostics(uri, diagnostics);

            return nlohmann::json({});
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Error in didOpen: {}", e.what());
            return nlohmann::json({});
        }
    }

    nlohmann::json LspServer::handleTextDocumentDidChange(const nlohmann::json &params)
    {
        try
        {
            auto textDocument = params["textDocument"];
            std::string uri = textDocument["uri"];
            int version = textDocument["version"];
            auto changes = params["contentChanges"];

            LOG_INFO("Document changed: {}", uri);

            // For simplicity, we assume the first change contains the entire document
            if (!changes.empty())
            {
                std::string newText = changes[0]["text"];
                documentManager_->updateDocument(uri, version, newText);

                // Validate and publish diagnostics
                auto diagnostics = documentManager_->validateDocument(uri);
                publishDiagnostics(uri, diagnostics);
            }

            return nlohmann::json({});
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Error in didChange: {}", e.what());
            return nlohmann::json({});
        }
    }

    nlohmann::json LspServer::handleTextDocumentDidClose(const nlohmann::json &params)
    {
        try
        {
            auto textDocument = params["textDocument"];
            std::string uri = textDocument["uri"];

            LOG_INFO("Document closed: {}", uri);

            documentManager_->removeDocument(uri);

            // Clear diagnostics when document is closed
            publishDiagnostics(uri, {});

            return nlohmann::json({});
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Error in didClose: {}", e.what());
            return nlohmann::json({});
        }
    }

    nlohmann::json LspServer::handleTextDocumentCompletion(const nlohmann::json &params)
    {
        try
        {
            auto textDocument = params["textDocument"];
            std::string uri = textDocument["uri"];
            auto position = params["position"];
            int line = position["line"];
            int character = position["character"];

            LOG_INFO("Completion requested at {}:{} in {}", line, character, uri);

            auto completions = documentManager_->provideCompletions(uri, {line, character});

            return nlohmann::json(completions);
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Error in completion: {}", e.what());
            return nlohmann::json::array();
        }
    }

    nlohmann::json LspServer::handleTextDocumentDefinition(const nlohmann::json &params)
    {
        try
        {
            auto textDocument = params["textDocument"];
            std::string uri = textDocument["uri"];
            auto position = params["position"];
            int line = position["line"];
            int character = position["character"];

            LOG_INFO("Definition requested at {}:{} in {}", line, character, uri);

            auto definitionLocation = documentManager_->findDefinition(uri, {line, character});

            if (definitionLocation)
            {
                return nlohmann::json(*definitionLocation);
            }

            return nlohmann::json(nullptr);
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("Error in definition: {}", e.what());
            return nlohmann::json(nullptr);
        }
    }

    void LspServer::publishDiagnostics(const std::string &uri, const std::vector<LSP::Diagnostic> &diagnostics)
    {
        nlohmann::json params = {
            {"uri", uri},
            {"diagnostics", nlohmann::json::array()}};

        for (const auto &diag : diagnostics)
        {
            params["diagnostics"].push_back({{"range", {{"start", {{"line", diag.range.start.line}, {"character", diag.range.start.character}}}, {"end", {{"line", diag.range.end.line}, {"character", diag.range.end.character}}}}},
                                             {"severity", static_cast<int>(diag.severity)},
                                             {"message", diag.message}});
        }

        sendNotification("textDocument/publishDiagnostics", params);
    }

    void LspServer::sendNotification(const std::string &method, const nlohmann::json &params)
    {
        nlohmann::json notification = {
            {"jsonrpc", "2.0"},
            {"method", method},
            {"params", params}};

        std::string message = notification.dump();
        std::cout << "Content-Length: " << message.size() << "\r\n\r\n";
        std::cout << message;
        std::cout.flush();
    }

} // namespace ZeroSyntax