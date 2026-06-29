// PluginDefinition.h - notepadpp-llm

#pragma once

#include "PluginInterface.h"
#include "Notepad_plus_msgs.h"
#include "Scintilla.h"
#include <tchar.h>

const int nbFunc = 5;

extern FuncItem funcItem[nbFunc];
extern NppData nppData;
extern TCHAR iniFilePath[MAX_PATH];
extern HINSTANCE g_hInst;

void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);

void helloNppLlm();
void doLlmHotkey();
void toggleChatPanel();
void editConfig();
void startMcpBridge();

HWND getCurrentScintilla();
