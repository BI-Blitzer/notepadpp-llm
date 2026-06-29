// PluginDefinition.cpp
// notepadpp-llm skeleton - adapted from npp-plugins/plugintemplate + nppopenai patterns

#include "PluginDefinition.h"
#include "menuCmdID.h"

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>

#include "editor/Editor.h"
#include "llm/LlmClient.h"
#include "config/Config.h"
#include "ui/ChatPanel.h"
#include "ui/ConfigDialog.h"
#include "mcp/BridgeServer.h"

// Global plugin data
FuncItem funcItem[nbFunc];
NppData nppData;
TCHAR iniFilePath[MAX_PATH] = { 0 };

static AppConfig g_config;
static LlmClient g_llmClient;
static ChatPanel g_chatPanel;
static BridgeServer g_bridge;
HINSTANCE g_hInst = NULL;
static bool g_nppReady = false;
static bool g_chatPanelCreated = false;
static ShortcutKey* g_hotkeySk = nullptr;

// DLL entry point. Notepad++ plugins rely on this for early init.
BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID /*lpReserved*/)
{
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		pluginInit(hModule);
		break;
	case DLL_PROCESS_DETACH:
		pluginCleanUp();
		break;
	}
	return TRUE;
}

void pluginInit(HANDLE hModule)
{
    g_hInst = (HINSTANCE)hModule;

    // Extremely light — only store hInst. Config and panel creation are lazy.
    commandMenuInit();
}

static bool g_configLoaded = false;

static void EnsureConfigLoaded()
{
    if (!g_configLoaded) {
        g_config = ConfigManager::Load();
        g_configLoaded = true;
    }
}

void pluginCleanUp()
{
    EnsureConfigLoaded();
    g_bridge.Stop();
    ConfigManager::Save(g_config);
}

void commandMenuInit()
{
    // STEP: register menu items + optional shortcuts
    static ShortcutKey* hotkeySk = nullptr;
    if (!hotkeySk) {
        hotkeySk = new ShortcutKey;
        hotkeySk->_isCtrl = true;
        hotkeySk->_isAlt = false;
        hotkeySk->_isShift = true;
        hotkeySk->_key = 'O';
    }

    setCommand(0, TEXT("Process Selection with LLM"), doLlmHotkey, hotkeySk, false);
    setCommand(1, TEXT("Toggle Chat Panel"), toggleChatPanel, NULL, false);
    setCommand(2, TEXT("Edit Config..."), editConfig, NULL, false);
    setCommand(3, TEXT("Start MCP Bridge"), startMcpBridge, NULL, false);
    setCommand(4, TEXT("About / Test"), helloNppLlm, NULL, false);
}

void commandMenuCleanUp()
{
}

bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc) return false;
    if (!pFunc) return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

HWND getCurrentScintilla()
{
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1) return NULL;
    return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

void helloNppLlm()
{
    EnsureConfigLoaded();

    auto backend = g_llmClient.GetBestAvailableBackend();
    std::string status = "notepadpp-llm\n\n";
    if (backend) {
        auto models = g_llmClient.ListDetectedModels();
        status += "Detected backend active. Models: " + std::to_string(models.size()) + "\n";
        for (size_t i = 0; i < std::min<size_t>(4, models.size()); ++i) {
            status += "  - " + models[i] + "\n";
        }
    } else {
        status += "No local LLM detected.\nStart Ollama on :11434 or LM Studio on :1234\n";
    }
    status += "\nCtrl+Shift+O = Process selection with LLM\n";
    MessageBoxA(NULL, status.c_str(), "notepadpp-llm", MB_OK);
}

void doLlmHotkey()
{
    EnsureConfigLoaded();

    std::string selection = Editor::GetSelection();
    std::string context = selection.empty() ? Editor::GetSmartContext(8) : selection;

    std::string userPrompt = context;

    if (context.empty()) {
        userPrompt = g_llmClient.GetLastPrompt();
        if (userPrompt.empty()) {
            MessageBoxA(NULL, "Nothing to process. Select text or type a prompt.", "notepadpp-llm", MB_OK);
            return;
        }
        context = Editor::GetSmartContext(8);
        if (!context.empty()) {
            userPrompt = "Previous request: " + userPrompt + "\n\nCurrent context:\n" + context;
        }
    } else {
        g_llmClient.SetLastPrompt(context);
    }

    std::string system = "You are a helpful coding and text assistant running locally. "
                         "Be concise and accurate. If the user provides code or a request, "
                         "respond directly with the improved / generated result when appropriate.";

    std::string response = g_llmClient.ProcessPrompt(userPrompt, system, g_config.default_model, g_config.temperature);

    if (g_config.replace_mode) {
        if (!selection.empty()) {
            Editor::ReplaceSelection(response);
        } else {
            Editor::ReplaceSelection(response);
        }
    } else {
        Editor::InsertAtCursor("\n\n" + response);
    }
}

void toggleChatPanel()
{
    EnsureConfigLoaded();

    if (!g_nppReady) {
        return;
    }

    if (!g_chatPanelCreated) {
        g_chatPanel.Create(nppData._nppHandle, g_hInst);
        g_chatPanelCreated = true;
    }

    if (g_chatPanel.IsVisible()) {
        g_chatPanel.Hide();
    } else {
        g_chatPanel.Show();
        g_chatPanel.SetModel(g_config.default_model);
    }
}

void editConfig()
{
    EnsureConfigLoaded();

    AppConfig temp = g_config;
    auto models = g_llmClient.ListDetectedModels();
    if (ConfigDialog::Show(nppData._nppHandle, temp, models)) {
        g_config = temp;
        ConfigManager::Save(g_config);
        g_llmClient.UpdateBackends(g_config.ollama_url, g_config.lmstudio_url);
    }
}

void startMcpBridge()
{
    EnsureConfigLoaded();

    if (g_bridge.IsRunning()) {
        g_bridge.Stop();
    } else {
        g_bridge.Start(g_config.bridge_port);
    }
}

// === Required Notepad++ plugin exports ===

extern "C" __declspec(dllexport) const wchar_t * getName()
{
	return L"NppLLM";
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	commandMenuInit();
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	if (funcItem[0]._pFunc == nullptr) {
		commandMenuInit();
	}
	*nbF = nbFunc;
	return funcItem;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification * notifyCode)
{
	if (notifyCode->nmhdr.code == NPPN_READY) {
		g_nppReady = true;
	}
}

extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

extern "C" __declspec(dllexport) BOOL isUnicode()
{
	return TRUE;
}

// Streaming bridge
void SendLlmRequestFromChatStreaming(const std::string& prompt, const std::string& model, ChatPanel* panel) {
    if (!panel) return;

    auto backend = g_llmClient.GetBestAvailableBackend(model);
    if (!backend) {
        panel->AppendMessage("Error", "No LLM backend available.");
        return;
    }

    std::vector<LlmMessage> msgs;
    msgs.push_back({"system", "You are a helpful coding assistant inside Notepad++. The user's message may include editor context. Analyze the provided context and respond with clear explanations plus the updated content so it is easy to apply."});
    msgs.push_back({"user", prompt});

    LlmOptions opts;
    opts.model = model.empty() ? g_config.default_model : model;
    opts.temperature = g_config.temperature;
    opts.stream = true;

    backend->CompleteStream(msgs, opts,
        [panel](const std::string& chunk) {
            if (panel) panel->AppendStreamingChunk(chunk);
        },
        [panel](const std::string& err) {
            if (panel) panel->AppendMessage("Error", err);
        }
    );
}

void RememberLastPrompt(const std::string& p) {
    g_llmClient.SetLastPrompt(p);
}
