param(
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

Write-Host "Building NppLlm ($Configuration)..."

if (-not (Test-Path "build")) {
    mkdir build | Out-Null
}

cmake -S . -B build -G "Visual Studio 17 2022" -A x64

cmake --build build --config $Configuration --target NppLlm

Write-Host "Build complete. DLL in build/$Configuration/NppLlm.dll"
