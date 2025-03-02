// LanguageServer/src/main.cpp
#include "protocol/lsp_server.hpp"
#include "utils/logger.hpp"

int main(int argc, char* argv[]) {
    // Initialize logging
    ZeroSyntax::initLogging(spdlog::level::debug);

    LOG_INFO("Starting ZeroSyntax Language Server");
    
    try {
        // Create and run the LSP server
        ZeroSyntax::LspServer server;
        server.run();
    } catch (const std::exception& e) {
        LOG_CRITICAL("Fatal error: {}", e.what());
        return 1;
    }
    
    return 0;
}