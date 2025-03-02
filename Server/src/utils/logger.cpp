// LanguageServer/src/utils/logger.cpp
#include "utils/logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>

namespace ZeroSyntax {

void initLogging(spdlog::level::level_enum level) {
    try {
        // Create logs directory if it doesn't exist
        std::filesystem::create_directories("logs");
        
        // Create console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(level);
        
        // Create file sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            "logs/zero_syntax_lsp.log", true);
        file_sink->set_level(level);
        
        // Create combined logger
        auto logger = std::make_shared<spdlog::logger>(
            "zero_syntax", spdlog::sinks_init_list{console_sink, file_sink});
        logger->set_level(level);
        
        // Set as default logger
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
        
        spdlog::info("Logging initialized");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

} // namespace ZeroSyntax