# Quick verification after building and installing the DLL
param([switch]$CheckOllama = $true)

if ($CheckOllama) {
    try {
        $tags = Invoke-RestMethod -Uri "http://localhost:11434/api/tags" -TimeoutSec 3
        Write-Host "Ollama reachable. Models found."
    } catch {
        Write-Warning "Ollama not reachable on :11434. Start it and pull a model."
    }
}

Write-Host "Manual steps:"
Write-Host "1. Ensure NppLlm.dll is in Notepad++ plugins/NppLlm/"
Write-Host "2. Restart Notepad++"
Write-Host "3. Test Ctrl+Shift+O or the chat panel"
