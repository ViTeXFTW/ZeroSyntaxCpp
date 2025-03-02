#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "protocol/lsp_messages.hpp"
#include <nlohmann/json.hpp>

namespace {

TEST(LspMessagesTest, PositionSerialization) {
    ZeroSyntax::LSP::Position pos{10, 20};
    nlohmann::json j = pos;
    
    EXPECT_EQ(j["line"], 10);
    EXPECT_EQ(j["character"], 20);
}

TEST(LspMessagesTest, PositionDeserialization) {
    nlohmann::json j = {{"line", 15}, {"character", 25}};
    ZeroSyntax::LSP::Position pos = j;
    
    EXPECT_EQ(pos.line, 15);
    EXPECT_EQ(pos.character, 25);
}

TEST(LspMessagesTest, RangeSerialization) {
    ZeroSyntax::LSP::Range range{
        {1, 2}, // start position
        {3, 4}  // end position
    };
    
    nlohmann::json j = range;
    
    EXPECT_EQ(j["start"]["line"], 1);
    EXPECT_EQ(j["start"]["character"], 2);
    EXPECT_EQ(j["end"]["line"], 3);
    EXPECT_EQ(j["end"]["character"], 4);
}

TEST(LspMessagesTest, RangeDeserialization) {
    nlohmann::json j = {
        {"start", {{"line", 5}, {"character", 6}}},
        {"end", {{"line", 7}, {"character", 8}}}
    };
    
    ZeroSyntax::LSP::Range range = j;
    
    EXPECT_EQ(range.start.line, 5);
    EXPECT_EQ(range.start.character, 6);
    EXPECT_EQ(range.end.line, 7);
    EXPECT_EQ(range.end.character, 8);
}

TEST(LspMessagesTest, LocationSerialization) {
    ZeroSyntax::LSP::Location loc{
        "file:///path/to/file.txt",
        {{1, 2}, {3, 4}}
    };
    
    nlohmann::json j = loc;
    
    EXPECT_EQ(j["uri"], "file:///path/to/file.txt");
    EXPECT_EQ(j["range"]["start"]["line"], 1);
    EXPECT_EQ(j["range"]["start"]["character"], 2);
    EXPECT_EQ(j["range"]["end"]["line"], 3);
    EXPECT_EQ(j["range"]["end"]["character"], 4);
}

TEST(LspMessagesTest, DiagnosticSerialization) {
    ZeroSyntax::LSP::Diagnostic diag{
        {{1, 2}, {3, 4}},
        ZeroSyntax::LSP::DiagnosticSeverity::Error,
        "This is an error message",
        "test-source"
    };
    
    nlohmann::json j = diag;
    
    EXPECT_EQ(j["range"]["start"]["line"], 1);
    EXPECT_EQ(j["severity"], 1); // Error = 1
    EXPECT_EQ(j["message"], "This is an error message");
    EXPECT_EQ(j["source"], "test-source");
}

TEST(LspMessagesTest, TextDocumentItemSerialization) {
    ZeroSyntax::LSP::TextDocumentItem item{
        "file:///path/to/file.txt",
        "ini",
        1,
        "[Section]\nKey=Value"
    };
    
    nlohmann::json j = item;
    
    EXPECT_EQ(j["uri"], "file:///path/to/file.txt");
    EXPECT_EQ(j["languageId"], "ini");
    EXPECT_EQ(j["version"], 1);
    EXPECT_EQ(j["text"], "[Section]\nKey=Value");
}

} // namespace