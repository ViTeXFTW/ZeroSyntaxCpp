#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/document_manager.hpp"

namespace {

class DocumentManagerTest : public ::testing::Test {
protected:
    ZeroSyntax::DocumentManager manager;
    
    // Sample document URI and content
    std::string uri = "file:///test/document.ini";
    std::string content = "[Section]\nKey=Value";
    std::string languageId = "ini";
};

TEST_F(DocumentManagerTest, AddDocument) {
    // Add a document
    EXPECT_NO_THROW(manager.addDocument(uri, content, languageId));
    
    // Verify it exists
    EXPECT_TRUE(manager.hasDocument(uri));
    
    // Verify content is correct
    EXPECT_EQ(manager.getDocumentText(uri), content);
}

TEST_F(DocumentManagerTest, UpdateDocument) {
    // Add a document
    manager.addDocument(uri, content, languageId);
    
    // Update the document
    std::string newContent = "[Section]\nKey=NewValue";
    EXPECT_NO_THROW(manager.updateDocument(uri, 2, newContent));
    
    // Verify content is updated
    EXPECT_EQ(manager.getDocumentText(uri), newContent);
}

TEST_F(DocumentManagerTest, RemoveDocument) {
    // Add a document
    manager.addDocument(uri, content, languageId);
    
    // Verify it exists
    EXPECT_TRUE(manager.hasDocument(uri));
    
    // Remove the document
    EXPECT_NO_THROW(manager.removeDocument(uri));
    
    // Verify it no longer exists
    EXPECT_FALSE(manager.hasDocument(uri));
}

TEST_F(DocumentManagerTest, ValidateDocument) {
    // Add a document
    manager.addDocument(uri, content, languageId);
    
    // Validate the document (initially should return no diagnostics for a valid document)
    auto diagnostics = manager.validateDocument(uri);
    EXPECT_TRUE(diagnostics.empty());
    
    // Note: More complex validation tests would be added as you implement
    // the INI parsing logic
}

TEST_F(DocumentManagerTest, NonExistentDocument) {
    // Try operations on a non-existent document
    std::string nonExistentUri = "file:///does/not/exist.ini";
    
    // Verify it doesn't exist
    EXPECT_FALSE(manager.hasDocument(nonExistentUri));
    
    // Get content should return empty string
    EXPECT_TRUE(manager.getDocumentText(nonExistentUri).empty());
    
    // Validation should return empty diagnostics
    auto diagnostics = manager.validateDocument(nonExistentUri);
    EXPECT_TRUE(diagnostics.empty());
    
    // No definition should be found
    auto definition = manager.findDefinition(nonExistentUri, {0, 0});
    EXPECT_FALSE(definition.has_value());
    
    // No completions should be provided
    auto completions = manager.provideCompletions(nonExistentUri, {0, 0});
    EXPECT_TRUE(completions.empty());
}

} // namespace