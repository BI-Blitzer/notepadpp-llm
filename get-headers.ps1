# Helper to fetch required Notepad++ plugin headers
$ErrorActionPreference = "Stop"

$nppSrc = "https://github.com/notepad-plus-plus/notepad-plus-plus.git"
$temp = "npp-temp"

if (Test-Path $temp) { Remove-Item $temp -Recurse -Force }
git clone --depth 1 $nppSrc $temp

$dest = "include"
New-Item -ItemType Directory -Force $dest | Out-Null

Copy-Item "$temp/PowerEditor/src/WinControls/PluginInterface/PluginInterface.h" $dest
Copy-Item "$temp/PowerEditor/src/MISC/PluginsManager/Notepad_plus_msgs.h" $dest
Copy-Item "$temp/PowerEditor/src/WinControls/StaticDialog/StaticDialog.h" $dest  # if needed
# Add Scintilla.h and menuCmdID.h as needed from appropriate locations

Remove-Item $temp -Recurse -Force
Write-Host "Headers copied to $dest. You may need Scintilla.h and menuCmdID.h manually."
