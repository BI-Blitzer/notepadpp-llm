// src/editor/Editor.h
// Thin Scintilla + N++ wrappers for notepadpp-llm
// Goal: hide raw SendMessage details used by LLM layer and MCP bridge

#pragma once
#include <string>
#include <vector>
#include <windows.h>

namespace Editor {

HWND GetCurrentSci();
std::string GetSelection();
std::string GetFullText();
std::string GetCurrentFilePath();
std::string GetLanguageName();

void ReplaceSelection(const std::string& replacement);
void InsertAtCursor(const std::string& text);
void AppendToEnd(const std::string& text);

// For MCP / advanced
size_t GetLength();
int GetCurrentPos();
void SetSelection(size_t start, size_t end);

void SaveCurrentFile();
int GetNbOpenFiles();
void ActivateDoc(int index);

bool OpenFile(const std::string& path);
void GoToLine(int line);
std::string FindText(const std::string& text, bool matchCase = false, bool wholeWord = false);
bool ReplaceInDoc(const std::string& findWhat, const std::string& replaceWith, bool all = false);
std::vector<std::string> GetOpenFilePaths();

void Undo();
void Redo();
void ReloadCurrentFile();
bool CloseCurrentFile(bool saveIfDirty = true);
std::string GetLineText(int line);
void SetStatusBarText(const std::string& text);
int GetCurrentLangType();

std::string GetSmartContext(int linesAround = 8);
int GetCurrentLine();
std::string GetFileName();
std::string GetStructuredContext(bool preferFullFile = false);
std::string GetLastResponsePayload(const std::string& response);

} // namespace Editor