# RoentgenClient

Приложение для просмотра рентгеновских снимков в формате raw

### Сборка на Linux x86-64

#### Что нужно

- CMake 4.3.1 или новее, Ninja и компилятор с поддержкой C++23
- Git, vcpkg и системные библиотеки для Qt (X11/XCB, OpenGL и EGL)

```bash
sudo apt install build-essential git ninja-build curl zip unzip tar pkg-config \
  '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev \
  libxi-dev libxkbcommon-dev libxkbcommon-x11-dev libegl1-mesa-dev
```

#### Установка vcpkg

Если vcpkg уже установлен, задайте `VCPKG_ROOT` с путём к своей установке. Для новой установки выполните следующие команды. [Документация vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash)

```bash
export VCPKG_ROOT="$HOME/tools/vcpkg"
mkdir -p "$HOME/tools"
git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
"$VCPKG_ROOT/bootstrap-vcpkg.sh"
```

#### Конфигурация, сборка и запуск

Из корня RoentgenClient в том же терминале:

```bash
cmake --preset linux
cmake --build --preset linux-build
./cmake-build-linux-roentgen/app/roentgen_app
```

Пресет `linux` настраивает debug-сборку с триплетом `x64-linux-roentgen`. На этапе конфигурации vcpkg устанавливает Qt и ITK из `vcpkg.json` в `cmake-build-linux-roentgen/vcpkg_installed/`

Советую создать CMakeUserPresets.json