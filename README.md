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

See `docs/MCP_INVENTORY.md` for N++ capabilities exposed via MCP.

## Key Features

- Hotkey + menu driven
- Local first (Ollama primary)
- Dockable chat panel with smart context
- MCP exposure for agents

## Planned Improvements

- Complete remaining MCP endpoints (advanced search/replace, notifications)
- Panel conversation memory
- Polish cloud backends
- Packaging and Plugin Admin submission

Revisit planned for July 2026.

## Building

See docs/BUILD*.md and docs/SETUP.md.

## License

MIT