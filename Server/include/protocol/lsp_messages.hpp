// LanguageServer/include/protocol/lsp_messages.hpp
#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <optional>

namespace ZeroSyntax {
namespace LSP {

// Basic LSP structures
struct Position {
    int line;
    int character;
};

struct Range {
    Position start;
    Position end;
};

struct Location {
    std::string uri;
    Range range;
};

enum class DiagnosticSeverity {
    Error = 1,
    Warning = 2,
    Information = 3,
    Hint = 4
};

struct Diagnostic {
    Range range;
    DiagnosticSeverity severity;
    std::string message;
    std::optional<std::string> source;
};

struct TextDocumentItem {
    std::string uri;
    std::string languageId;
    int version;
    std::string text;
};

struct VersionedTextDocumentIdentifier {
    std::string uri;
    int version;
};

struct TextDocumentIdentifier {
    std::string uri;
};

struct TextDocumentPositionParams {
    TextDocumentIdentifier textDocument;
    Position position;
};

struct CompletionItem {
    std::string label;
    std::optional<std::string> detail;
    std::optional<std::string> documentation;
    int kind;  // CompletionItemKind enum
};

struct CompletionList {
    bool isIncomplete;
    std::vector<CompletionItem> items;
};

// LSP Capabilities
struct ServerCapabilities {
    bool textDocumentSync = false;
    bool hoverProvider = false;
    bool completionProvider = false;
    bool definitionProvider = false;
    bool referencesProvider = false;
    bool documentSymbolProvider = false;
    bool workspaceSymbolProvider = false;
};

// JSON conversion functions
void to_json(nlohmann::json& j, const Position& p);
void from_json(const nlohmann::json& j, Position& p);

void to_json(nlohmann::json& j, const Range& r);
void from_json(const nlohmann::json& j, Range& r);

void to_json(nlohmann::json& j, const Location& l);
void from_json(const nlohmann::json& j, Location& l);

void to_json(nlohmann::json& j, const Diagnostic& d);
void from_json(const nlohmann::json& j, Diagnostic& d);

void to_json(nlohmann::json& j, const TextDocumentItem& t);
void from_json(const nlohmann::json& j, TextDocumentItem& t);

void to_json(nlohmann::json& j, const VersionedTextDocumentIdentifier& v);
void from_json(const nlohmann::json& j, VersionedTextDocumentIdentifier& v);

void to_json(nlohmann::json& j, const TextDocumentIdentifier& t);
void from_json(const nlohmann::json& j, TextDocumentIdentifier& t);

void to_json(nlohmann::json& j, const TextDocumentPositionParams& t);
void from_json(const nlohmann::json& j, TextDocumentPositionParams& t);

void to_json(nlohmann::json& j, const CompletionItem& c);
void from_json(const nlohmann::json& j, CompletionItem& c);

void to_json(nlohmann::json& j, const CompletionList& c);
void from_json(const nlohmann::json& j, CompletionList& c);

void to_json(nlohmann::json& j, const ServerCapabilities& s);
void from_json(const nlohmann::json& j, ServerCapabilities& s);

} // namespace LSP
} // namespace ZeroSyntax