#include "core/document_manager.hpp"
#include "utils/logger.hpp"

namespace ZeroSyntax {

DocumentManager::DocumentManager() {
    LOG_INFO("Document manager initialized");
}

void DocumentManager::addDocument(const std::string& uri, const std::string& text, const std::string& languageId) {
    documents_[uri] = Document{uri, text, languageId, 0};
    parseIniDocument(documents_[uri]);
    LOG_INFO("Added document: {}", uri);
}

void DocumentManager::updateDocument(const std::string& uri, int version, const std::string& text) {
    if (documents_.find(uri) != documents_.end()) {
        documents_[uri].text = text;
        documents_[uri].version = version;
        parseIniDocument(documents_[uri]);
        LOG_INFO("Updated document: {} to version {}", uri, version);
    } else {
        LOG_WARN("Tried to update non-existent document: {}", uri);
    }
}

void DocumentManager::removeDocument(const std::string& uri) {
    auto it = documents_.find(uri);
    if (it != documents_.end()) {
        documents_.erase(it);
        LOG_INFO("Removed document: {}", uri);
    }
}

std::string DocumentManager::getDocumentText(const std::string& uri) const {
    auto it = documents_.find(uri);
    if (it != documents_.end()) {
        return it->second.text;
    }
    return "";
}

bool DocumentManager::hasDocument(const std::string& uri) const {
    return documents_.find(uri) != documents_.end();
}

std::vector<LSP::Diagnostic> DocumentManager::validateDocument(const std::string& uri) {
    std::vector<LSP::Diagnostic> diagnostics;
    
    if (!hasDocument(uri)) {
        LOG_WARN("Tried to validate non-existent document: {}", uri);
        return diagnostics;
    }
    
    // TODO: Implement actual validation logic based on INI file parsing
    // For now, returning empty diagnostics (no errors)
    
    LOG_INFO("Validated document: {} with {} diagnostics", uri, diagnostics.size());
    return diagnostics;
}

std::optional<LSP::Location> DocumentManager::findDefinition(const std::string& uri, const LSP::Position& position) {
    if (!hasDocument(uri)) {
        LOG_WARN("Tried to find definition in non-existent document: {}", uri);
        return std::nullopt;
    }
    
    // TODO: Implement actual definition finding logic
    // For now, returning nullptr (no definition found)
    
    LOG_INFO("No definition found at {}:{} in {}", position.line, position.character, uri);
    return std::nullopt;
}

std::vector<LSP::CompletionItem> DocumentManager::provideCompletions(const std::string& uri, const LSP::Position& position) {
    std::vector<LSP::CompletionItem> completions;
    
    if (!hasDocument(uri)) {
        LOG_WARN("Tried to get completions in non-existent document: {}", uri);
        return completions;
    }
    
    // TODO: Implement actual completion logic
    // For now, return a simple dummy completion
    
    LSP::CompletionItem item;
    item.label = "Example";
    item.detail = "Example completion";
    item.documentation = "This is an example completion item";
    item.kind = 1;  // Text
    
    completions.push_back(item);
    
    LOG_INFO("Provided {} completion items for {}:{}",
             completions.size(), position.line, position.character);
    
    return completions;
}

void DocumentManager::parseIniDocument(Document& document) {
    // TODO: Implement actual INI file parsing logic based on the game code
    LOG_INFO("Parsed document: {}", document.uri);
}

} // namespace ZeroSyntax