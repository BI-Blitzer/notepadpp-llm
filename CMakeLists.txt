cmake_minimum_required(VERSION 3.20)
project(NppLlm LANGUAGES CXX)

set(CMAKE_SYSTEM_VERSION "10.0.22621" CACHE STRING "Windows SDK version to use" FORCE)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Notepad++ plugins must be built as 64-bit (x64).")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "This plugin is Windows-only.")
endif()

set(NPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include" CACHE PATH "Path containing Notepad++ plugin dev headers")

if(NOT EXISTS "${NPP_INCLUDE_DIR}/PluginInterface.h")
    message(FATAL_ERROR "Notepad++ headers not found. Run get-headers.ps1 or set NPP_INCLUDE_DIR.")
endif()

add_library(NppLlm SHARED
    src/PluginDefinition.cpp
    src/config/Config.cpp
    src/editor/Editor.cpp
    src/llm/LlmClient.cpp
    src/llm/OllamaBackend.cpp
    src/llm/OpenAICompatBackend.cpp
    src/mcp/BridgeServer.cpp
    src/ui/ChatPanel.cpp
    src/ui/ConfigDialog.cpp
    src/utils/HttpClient.cpp
    src/utils/Json.cpp
    src/NppLlm.rc
)

target_include_directories(NppLlm PRIVATE include src ${NPP_INCLUDE_DIR})

target_link_libraries(NppLlm PRIVATE winhttp)

set_target_properties(NppLlm PROPERTIES
    OUTPUT_NAME "NppLlm"
    SUFFIX ".dll"
)

# Copy to plugins dir on build if desired (optional)
# install(TARGETS NppLlm RUNTIME DESTINATION "${NPP_ROOT}/plugins/NppLlm")
