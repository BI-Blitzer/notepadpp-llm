**Local-first LLM assistant plugin for Notepad++** with optional MCP server support for agentic workflows (Claude Desktop, Cursor, VS Code Copilot, etc.).

## Current Status (June 2026)

The plugin is in much better shape and is usable for daily work:

- **Ctrl+Shift+O** (or menu) processes selection (or smart context) with local LLMs and writes results back.
- **Dockable LLM Chat panel** is stable:
  - Good docking, resize, and redraw behavior
  - Context label shows active file + selection status
  - Buttons (Clear/Copy/Prev/Apply/Send) properly placed
  - Defaults to full file context + auto-detects selection
  - Smarter Apply (extracts last code block cleanly)
- Expanded **MCP Bridge** with many endpoints for external agents (read state, navigate, edit, control UI).
- Real streaming, WinHTTP client, Ollama + OpenAI-compatible backends.
- Config + INI + Edit Config dialog.
- Works well with Ollama / LM Studio.

We're pausing active development and will revisit in July 2026.

See `STATUS.md` for the latest detailed status. See `docs/MCP_INVENTORY.md` for the growing list of N++ capabilities exposed via MCP.

## Key Features

- **Hotkey + menu driven**: Ctrl+Shift+O (Process Selection), full Plugins → NppLLM menu
- **Local first**: Ollama primary (LM Studio / llama.cpp / OpenAI-compatible supported)
- **Dockable chat panel** with smart context options, streaming, Ctrl+Enter for full file
- **Edit Config** dialog + per-user INI
- **MCP exposure**: Let external agents read buffers and apply edits live
- Minimal dependencies (native Windows APIs)

## Quick Start

### 1. Build
See `docs/BUILD_CMAKE.md`, `docs/BUILD.md`, `docs/SETUP.md`.

You will need Notepad++ headers + Visual Studio (x64, Unicode, link `winhttp.lib`).

### 2. Install
Place `NppLlm.dll` in `plugins\NppLlm\` next to your Notepad++ executable and restart N++.

### 3. Use
Have Ollama or LM Studio running.

- Use **Ctrl+Shift+O** for quick process selection.
- Use **Toggle Chat Panel** for interactive chat (with Apply to editor).
- Start **MCP Bridge** for external agents.

See README and docs/ for details.

## Planned Improvements

- Complete remaining MCP endpoints (advanced search/replace, notifications, richer buffer metadata)
- Panel conversation memory and history persistence
- Polish and expand cloud backends + error handling
- Full Config dialog resources and visuals
- Packaging, icons, and Plugin Admin submission

Revisit planned for July 2026.

## License

MIT License (see LICENSE)

## Credits
- Inspired by Krazal/nppopenai
- N++ plugin template
- MCP: modelcontextprotocol.io
