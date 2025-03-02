set(TEST_SOURCES
    test_main.cpp
    unit/test_json_rpc_handler.cpp
    unit/test_lsp_messages.cpp
    unit/test_document_manager.cpp
)

add_executable(ZS_Tests ${TEST_SOURCES})

target_link_libraries(ZS_Tests PRIVATE
    gtest
    gmock
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

target_include_directories(ZS_Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/Server/include
    ${CMAKE_SOURCE_DIR}/Server/src
)

# Add source files from main project, excluding main.cpp
set(TEST_IMPLEMENTATION_SOURCES
    ${CMAKE_SOURCE_DIR}/Server/src/protocol/lsp_server.cpp
    ${CMAKE_SOURCE_DIR}/Server/src/protocol/json_rpc_handler.cpp
    ${CMAKE_SOURCE_DIR}/Server/src/protocol/lsp_messages.cpp
    ${CMAKE_SOURCE_DIR}/Server/src/utils/logger.cpp
    ${CMAKE_SOURCE_DIR}/Server/src/core/document_manager.cpp
)
target_sources(ZS_Tests PRIVATE ${TEST_IMPLEMENTATION_SOURCES})

include(GoogleTest)
gtest_discover_tests(ZS_Tests)