// LanguageServer/include/core/document_manager.hpp
#pragma once

#include "../protocol/lsp_messages.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace ZeroSyntax {

class DocumentManager {
public:
    DocumentManager();
    
    // Add a document when it's opened
    void addDocument(const std::string& uri, const std::string& text, const std::string& languageId);
    
    // Update document content
    void updateDocument(const std::string& uri, int version, const std::string& text);
    
    // Remove a document when it's closed
    void removeDocument(const std::string& uri);
    
    // Get document text
    std::string getDocumentText(const std::string& uri) const;
    
    // Check if a document exists
    bool hasDocument(const std::string& uri) const;
    
    // Parse and validate the current document
    std::vector<LSP::Diagnostic> validateDocument(const std::string& uri);
    
    // Find definition at the given position
    std::optional<LSP::Location> findDefinition(const std::string& uri, const LSP::Position& position);
    
    // Provide completions at the given position
    std::vector<LSP::CompletionItem> provideCompletions(const std::string& uri, const LSP::Position& position);
    
private:
    // Document storage
    struct Document {
        std::string uri;
        std::string text;
        std::string languageId;
        int version;
        
        // Parsed representation could be added here
    };
    
    std::unordered_map<std::string, Document> documents_;
    
    // INI file parsing helpers
    void parseIniDocument(Document& document);
};

} // namespace ZeroSyntax