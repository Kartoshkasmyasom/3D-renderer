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

$repoRoot = $PSScriptRoot
$buildDir = Join-Path $repoRoot "build-vcpkg"
$sourceDir = Join-Path $repoRoot "src"

$vcpkgRoot = $env:VCPKG_ROOT
if (-not $vcpkgRoot) {
  $vcpkgRoot = "C:\vcpkg"
}

$vcpkgToolchain = Join-Path $vcpkgRoot "scripts\buildsystems\vcpkg.cmake"
if (-not (Test-Path $vcpkgToolchain)) {
  throw "vcpkg toolchain was not found: $vcpkgToolchain. Install vcpkg or set VCPKG_ROOT to the folder where vcpkg is installed."
}

$configureArgs = @(
  "-S", $sourceDir,
  "-B", $buildDir,
  "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain",
  "-DVCPKG_MANIFEST_DIR=$repoRoot"
)

& $cmake @configureArgs
if ($LASTEXITCODE -ne 0) {
  exit $LASTEXITCODE
}

& $cmake --build $buildDir --config Debug
if ($LASTEXITCODE -ne 0) {
  exit $LASTEXITCODE
}
