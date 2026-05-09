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

$buildDir = "build-vcpkg"
$configureArgs = @("-S", "src", "-B", $buildDir)
$vcpkgToolchain = "C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
if (Test-Path $vcpkgToolchain) {
  $configureArgs += "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain"
}

& $cmake @configureArgs
if ($LASTEXITCODE -ne 0) {
  exit $LASTEXITCODE
}

& $cmake --build $buildDir --config Debug
if ($LASTEXITCODE -ne 0) {
  exit $LASTEXITCODE
}
