#include "protocol/lsp_messages.hpp"

namespace ZeroSyntax {
namespace LSP {

// Position conversion
void to_json(nlohmann::json& j, const Position& p) {
    j = nlohmann::json{
        {"line", p.line},
        {"character", p.character}
    };
}

void from_json(const nlohmann::json& j, Position& p) {
    j.at("line").get_to(p.line);
    j.at("character").get_to(p.character);
}

// Range conversion
void to_json(nlohmann::json& j, const Range& r) {
    j = nlohmann::json{
        {"start", r.start},
        {"end", r.end}
    };
}

void from_json(const nlohmann::json& j, Range& r) {
    j.at("start").get_to(r.start);
    j.at("end").get_to(r.end);
}

// Location conversion
void to_json(nlohmann::json& j, const Location& l) {
    j = nlohmann::json{
        {"uri", l.uri},
        {"range", l.range}
    };
}

void from_json(const nlohmann::json& j, Location& l) {
    j.at("uri").get_to(l.uri);
    j.at("range").get_to(l.range);
}

// Diagnostic conversion
void to_json(nlohmann::json& j, const Diagnostic& d) {
    j = nlohmann::json{
        {"range", d.range},
        {"severity", static_cast<int>(d.severity)},
        {"message", d.message}
    };
    
    if (d.source) {
        j["source"] = *d.source;
    }
}

void from_json(const nlohmann::json& j, Diagnostic& d) {
    j.at("range").get_to(d.range);
    d.severity = static_cast<DiagnosticSeverity>(j.at("severity").get<int>());
    j.at("message").get_to(d.message);
    
    if (j.contains("source")) {
        d.source = j.at("source").get<std::string>();
    } else {
        d.source = std::nullopt;
    }
}

// TextDocumentItem conversion
void to_json(nlohmann::json& j, const TextDocumentItem& t) {
    j = nlohmann::json{
        {"uri", t.uri},
        {"languageId", t.languageId},
        {"version", t.version},
        {"text", t.text}
    };
}

void from_json(const nlohmann::json& j, TextDocumentItem& t) {
    j.at("uri").get_to(t.uri);
    j.at("languageId").get_to(t.languageId);
    j.at("version").get_to(t.version);
    j.at("text").get_to(t.text);
}

// VersionedTextDocumentIdentifier conversion
void to_json(nlohmann::json& j, const VersionedTextDocumentIdentifier& v) {
    j = nlohmann::json{
        {"uri", v.uri},
        {"version", v.version}
    };
}

void from_json(const nlohmann::json& j, VersionedTextDocumentIdentifier& v) {
    j.at("uri").get_to(v.uri);
    j.at("version").get_to(v.version);
}

// TextDocumentIdentifier conversion
void to_json(nlohmann::json& j, const TextDocumentIdentifier& t) {
    j = nlohmann::json{
        {"uri", t.uri}
    };
}

void from_json(const nlohmann::json& j, TextDocumentIdentifier& t) {
    j.at("uri").get_to(t.uri);
}

// TextDocumentPositionParams conversion
void to_json(nlohmann::json& j, const TextDocumentPositionParams& t) {
    j = nlohmann::json{
        {"textDocument", t.textDocument},
        {"position", t.position}
    };
}

void from_json(const nlohmann::json& j, TextDocumentPositionParams& t) {
    j.at("textDocument").get_to(t.textDocument);
    j.at("position").get_to(t.position);
}

// CompletionItem conversion
void to_json(nlohmann::json& j, const CompletionItem& c) {
    j = nlohmann::json{
        {"label", c.label},
        {"kind", c.kind}
    };
    
    if (c.detail) {
        j["detail"] = *c.detail;
    }
    
    if (c.documentation) {
        j["documentation"] = *c.documentation;
    }
}

void from_json(const nlohmann::json& j, CompletionItem& c) {
    j.at("label").get_to(c.label);
    j.at("kind").get_to(c.kind);
    
    if (j.contains("detail")) {
        c.detail = j.at("detail").get<std::string>();
    } else {
        c.detail = std::nullopt;
    }
    
    if (j.contains("documentation")) {
        c.documentation = j.at("documentation").get<std::string>();
    } else {
        c.documentation = std::nullopt;
    }
}

// CompletionList conversion
void to_json(nlohmann::json& j, const CompletionList& c) {
    j = nlohmann::json{
        {"isIncomplete", c.isIncomplete},
        {"items", c.items}
    };
}

void from_json(const nlohmann::json& j, CompletionList& c) {
    j.at("isIncomplete").get_to(c.isIncomplete);
    j.at("items").get_to(c.items);
}

// ServerCapabilities conversion
void to_json(nlohmann::json& j, const ServerCapabilities& s) {
    j = nlohmann::json{};
    
    if (s.textDocumentSync) {
        // Use full sync mode (1)
        j["textDocumentSync"] = 1;
    }
    
    if (s.completionProvider) {
        j["completionProvider"] = nlohmann::json::object();
    }
    
    if (s.definitionProvider) {
        j["definitionProvider"] = true;
    }
    
    if (s.referencesProvider) {
        j["referencesProvider"] = true;
    }
    
    if (s.documentSymbolProvider) {
        j["documentSymbolProvider"] = true;
    }
    
    if (s.workspaceSymbolProvider) {
        j["workspaceSymbolProvider"] = true;
    }
    
    if (s.hoverProvider) {
        j["hoverProvider"] = true;
    }
}

void from_json(const nlohmann::json& j, ServerCapabilities& s) {
    // This might not be needed as we typically don't parse capabilities from JSON
    if (j.contains("textDocumentSync")) {
        s.textDocumentSync = true;
    }
    
    if (j.contains("completionProvider")) {
        s.completionProvider = true;
    }
    
    if (j.contains("definitionProvider")) {
        s.definitionProvider = j["definitionProvider"].get<bool>();
    }
    
    if (j.contains("referencesProvider")) {
        s.referencesProvider = j["referencesProvider"].get<bool>();
    }
    
    if (j.contains("documentSymbolProvider")) {
        s.documentSymbolProvider = j["documentSymbolProvider"].get<bool>();
    }
    
    if (j.contains("workspaceSymbolProvider")) {
        s.workspaceSymbolProvider = j["workspaceSymbolProvider"].get<bool>();
    }
    
    if (j.contains("hoverProvider")) {
        s.hoverProvider = j["hoverProvider"].get<bool>();
    }
}

} // namespace LSP
} // namespace ZeroSyntax