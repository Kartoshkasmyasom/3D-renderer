# 3D Renderer

Небольшой 3D-renderer на C++17, CMake, OpenGL, GLEW, SFML, Eigen и Assimp.

Проект собирается в исполняемый файл `3d-renderer`. Все зависимости нужно
установить заранее: CMake ожидает, что заголовочные файлы и библиотеки уже
доступны в системе.

## Зависимости

Для сборки нужны:

- `assimp` - импорт 3D-моделей.
- `eigen3` - математические операции.
- `glew` - работа с расширениями OpenGL.
- `sfml` - окно, графика и системные функции.
- OpenGL.

## Что нужно установить один раз

### Windows

1. Установите компилятор:
   - Visual Studio 2022+ с компонентом **Desktop development with C++** или mingw-w64.
2. Установите CMake.
3. Установите библиотеки `assimp`, `eigen3`, `glew` и `sfml` под тот же
   компилятор, которым будете собирать проект.

Если библиотеки установлены не в системные пути, передайте их расположение в
CMake через `CMAKE_PREFIX_PATH`. Например:

```powershell
cmake -S .\src -B .\build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="C:\libs\assimp;C:\libs\eigen3;C:\libs\glew;C:\libs\sfml"
```

### Linux Ubuntu

Для сборки под Linux нужен `gcc/g++` версии 12 или новее либо `clang/clang++`
версии 16.0.0 или новее.

1. Установите инструменты сборки и OpenGL:

```bash
sudo apt update
sudo apt install -y build-essential cmake git pkg-config libgl1-mesa-dev
```

2. Установите зависимости проекта:

```bash
sudo apt install -y libassimp-dev libeigen3-dev libglew-dev libsfml-dev
```

## Сборка и запуск

```bash
git clone https://github.com/Kartoshkasmyasom/3D-renderer.git
cd 3D-renderer
```

### Windows

Откройте PowerShell в папке проекта и выполните:

```powershell
cmake -S .\src -B .\build -DCMAKE_BUILD_TYPE=Debug
cmake --build .\build --config Debug
```

Если CMake не находит зависимости, укажите путь к ним:

```powershell
cmake -S .\src -B .\build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH="C:\path\to\dependencies"
cmake --build .\build --config Debug
```

Чтобы запустить приложение:

```powershell
.\build\Debug\3d-renderer.exe
```

### Linux Ubuntu

Откройте терминал в папке проекта и выполните:

```bash
cmake -S ./src -B ./build -DCMAKE_BUILD_TYPE=Debug
cmake --build ./build
```

Чтобы запустить приложение:

```bash
./build/3d-renderer
```

## Если добавилась новая библиотека

Новую библиотеку нужно установить тем же способом, которым установлены остальные
зависимости проекта, а затем добавить `find_package(...)` и линковку в CMake.
