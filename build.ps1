$ErrorActionPreference = "Stop"

$cmakeCandidates = @(
  "cmake",
  "C:\Program Files\Microsoft Visual Studio\18\Insiders\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\Preview\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
  "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
)

$cmake = $null
foreach ($candidate in $cmakeCandidates) {
  $command = Get-Command $candidate -ErrorAction SilentlyContinue
  if ($command) {
    $cmake = $command.Source
    break
  }
}

if (-not $cmake) {
  throw "CMake was not found. Install CMake or add cmake.exe to PATH."
}

& $cmake --build build --config Debug
