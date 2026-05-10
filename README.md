# 3D Renderer

Небольшой 3D-renderer на C++17, CMake, OpenGL, GLEW, SFML, Eigen и Assimp.

Проект собирается в исполняемый файл `3d-renderer`. Зависимости описаны в `vcpkg.json`, поэтому их не нужно вручную искать по сайтам: при сборке через vcpkg CMake скачает и соберет нужные библиотеки сам.

## Что нужно установить один раз

### Windows

Для сборки под Windows допускается использование mingw-w64. Также проект можно собрать через Visual Studio 2022, если установлен компонент C++.

1. Установите Visual Studio 2022 с компонентом **Desktop development with C++**.
2. Установите CMake, если он не был установлен вместе с Visual Studio.
3. Установите vcpkg:

```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

Если vcpkg лежит не в `C:\vcpkg`, укажите путь к нему:

```powershell
$env:VCPKG_ROOT = "D:\path\to\vcpkg"
```

### Linux Ubuntu

Для сборки под Linux нужен `gcc/g++` версии 12 или новее либо `clang/clang++` версии 16.0.0 или новее.

1. Установите инструменты сборки:

```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config zip unzip tar curl libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libudev-dev libfreetype-dev libopenal-dev libflac-dev libvorbis-dev
```

2. Установите vcpkg:

```bash
git clone https://github.com/microsoft/vcpkg "$HOME/vcpkg"
"$HOME/vcpkg/bootstrap-vcpkg.sh"
```

Если vcpkg лежит не в `$HOME/vcpkg`, укажите путь к нему:

```bash
export VCPKG_ROOT="/path/to/vcpkg"
```

## Сборка и запуск
```bash
git clone https://github.com/Kartoshkasmyasom/3D-renderer.git
cd 3D-renderer
```
### Windows

Откройте PowerShell в папке проекта и выполните:

```powershell
.\build.ps1
```

Если PowerShell пишет, что выполнение сценариев отключено, запустите так:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

Скрипт запустит CMake, прочитает зависимости из `vcpkg.json` и соберет проект.

Чтобы запустить само приложение, выполните:

```powershell
.\build-vcpkg\Debug\3d-renderer.exe
```

### Linux Ubuntu

Откройте терминал в папке проекта и выполните:

```bash
chmod +x ./build.sh
./build.sh
```

Скрипт запустит CMake, прочитает зависимости из `vcpkg.json` и соберет проект.

Чтобы запустить само приложение, выполните:

```bash
./build-vcpkg/3d-renderer
```

## Где лежат зависимости

Список библиотек проекта находится в файле `vcpkg.json`:

- `assimp` — импорт 3D-моделей.
- `eigen3` — математические операции.
- `glew` — работа с расширениями OpenGL.
- `sfml` — окно, графика и системные функции.

Если понадобится новая библиотека, ее нужно добавить в `vcpkg.json`, после чего заново запустить сборку.
