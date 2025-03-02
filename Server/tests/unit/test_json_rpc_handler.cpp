#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "protocol/json_rpc_handler.hpp"

namespace {

using ::testing::_;
using ::testing::Return;

class JsonRpcHandlerTest : public ::testing::Test {
protected:
    ZeroSyntax::JsonRpcHandler handler;
};

// Basic test for the constructor
TEST_F(JsonRpcHandlerTest, Constructor) {
    // Just verifying that the constructor doesn't throw
    EXPECT_NO_THROW({
        ZeroSyntax::JsonRpcHandler handler;
    });
}

// Test registering a method
TEST_F(JsonRpcHandlerTest, RegisterMethod) {
    bool methodCalled = false;
    
    // Register a simple method
    handler.registerMethod("test.method", [&methodCalled](const nlohmann::json& params) {
        methodCalled = true;
        return nlohmann::json({{"result", "success"}});
    });
    
    // Create a test request
    std::string request = R"({
        "jsonrpc": "2.0",
        "method": "test.method",
        "id": 1
    })";
    
    // Process the request
    auto response = handler.handleRequest(request);
    
    // Verify the method was called
    EXPECT_TRUE(methodCalled);
    
    // Verify we got a response
    EXPECT_TRUE(response.has_value());
    
    // Parse the response
    auto parsedResponse = nlohmann::json::parse(*response);
    
    // Check it has the expected structure
    EXPECT_EQ(parsedResponse["jsonrpc"], "2.0");
    EXPECT_EQ(parsedResponse["id"], 1);
    EXPECT_EQ(parsedResponse["result"]["result"], "success");
}

// Test handling a notification (no id)
TEST_F(JsonRpcHandlerTest, HandleNotification) {
    bool methodCalled = false;
    
    // Register a simple method
    handler.registerMethod("test.notification", [&methodCalled](const nlohmann::json& params) {
        methodCalled = true;
        return nlohmann::json();
    });
    
    // Create a test notification (no id)
    std::string notification = R"({
        "jsonrpc": "2.0",
        "method": "test.notification"
    })";
    
    // Process the notification
    auto response = handler.handleRequest(notification);
    
    // Verify the method was called
    EXPECT_TRUE(methodCalled);
    
    // Verify no response was generated (it's a notification)
    EXPECT_FALSE(response.has_value());
}

// Test method not found
TEST_F(JsonRpcHandlerTest, MethodNotFound) {
    // Create a test request with unknown method
    std::string request = R"({
        "jsonrpc": "2.0",
        "method": "unknown.method",
        "id": 1
    })";
    
    // Process the request
    auto response = handler.handleRequest(request);
    
    // Verify we got a response
    EXPECT_TRUE(response.has_value());
    
    // Parse the response
    auto parsedResponse = nlohmann::json::parse(*response);
    
    // Check it's an error response
    EXPECT_EQ(parsedResponse["jsonrpc"], "2.0");
    EXPECT_EQ(parsedResponse["id"], 1);
    EXPECT_EQ(parsedResponse["error"]["code"], -32601);
    EXPECT_EQ(parsedResponse["error"]["message"], "Method not found");
}

// Test invalid JSON
TEST_F(JsonRpcHandlerTest, InvalidJson) {
    // Create invalid JSON
    std::string invalidJson = "this is not valid json";
    
    // Process the request
    auto response = handler.handleRequest(invalidJson);
    
    // Verify we got a response
    EXPECT_TRUE(response.has_value());
    
    // Parse the response
    auto parsedResponse = nlohmann::json::parse(*response);
    
    // Check it's a parse error
    EXPECT_EQ(parsedResponse["jsonrpc"], "2.0");
    EXPECT_EQ(parsedResponse["error"]["code"], -32700);
    EXPECT_EQ(parsedResponse["error"]["message"], "Parse error");
}

} // namespace