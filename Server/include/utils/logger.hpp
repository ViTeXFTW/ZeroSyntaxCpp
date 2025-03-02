// LanguageServer/include/utils/logger.hpp
#pragma once

#include <spdlog/spdlog.h>

#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

namespace ZeroSyntax {

// Initialize logging
void initLogging(spdlog::level::level_enum level = spdlog::level::info);

} // namespace ZeroSyntax