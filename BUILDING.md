# Building CustomAp

## Ubuntu command-line build

Install these prerequisites:

- CMake 3.22 or newer
- Ninja
- STM32CubeCLT 1.18.0, containing GNU Tools for STM32
  `13.3.rel1.20240926-1715`

The build locates the toolchain in this order:

1. `-DSTM32_TOOLCHAIN_BIN_DIR=/path/to/GNU-tools-for-STM32/bin`
2. the `STM32_TOOLCHAIN_BIN_DIR` environment variable
3. the standard STM32CubeCLT 1.18.0 location under `$HOME/st` or `/opt/st`
4. `arm-none-eabi-gcc` on `PATH`

The exact compiler banner is checked during configuration. Configuration stops
instead of silently building with another compiler version.

Configure and build:

```sh
cmake --preset debug
cmake --build --preset debug
```

For a nonstandard installation:

```sh
cmake --preset debug \
  -DSTM32_TOOLCHAIN_BIN_DIR=/path/to/GNU-tools-for-STM32/bin
cmake --build --preset debug
```

The build creates these files under `build/gcc-13.3-debug/`:

- `customAutopilotBoard.elf`
- `customAutopilotBoard.bin`
- `customAutopilotBoard.hex`

## macOS STM32CubeIDE build

Import or open the repository as the existing STM32CubeIDE project, select the
`Debug` configuration, and use **Project > Build Project** (or the Build
button). STM32CubeIDE uses `.project` and `.cproject`; it does not use the
CMake configuration.

The generated `Debug/` and `Release/` directories are intentionally ignored.
STM32CubeIDE recreates them locally using its own bundled toolchain.
