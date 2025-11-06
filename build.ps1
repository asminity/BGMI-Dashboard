$ErrorActionPreference = 'Stop'
Write-Host "Building BGMI Dashboard (C++17)" -ForegroundColor Cyan

$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $gpp) {
    Write-Error "g++ not found. Please install MinGW-w64 and ensure g++ is in PATH."
}

$sources = @(
    'main.cpp',
    'user/Dashboard.cpp',
    'user/Player.cpp',
    'user/Match.cpp',
    'user/Inventory.cpp',
    'user/Achievement.cpp',
    'user/Leaderboard.cpp',
    'admin/AdminDashboard.cpp',
    'UserAuth.cpp',
    'CentralInventory.cpp',
    'FileUtil.cpp'
)

$cmd = "g++ -std=c++17 -O2 -o bgmi " + ($sources -join ' ')
Write-Host $cmd -ForegroundColor DarkGray
& g++ -std=c++17 -O2 -o bgmi @sources 2>&1 | Tee-Object -Variable buildOut | Out-String | Write-Host

if ($LASTEXITCODE -ne 0) {
    Write-Host $buildOut -ForegroundColor Red
    Write-Error "Build failed. See errors above."
}
else {
    Write-Host "Build succeeded: ./bgmi.exe" -ForegroundColor Green
}
