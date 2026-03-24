# Engine (Scacchi)

Build standard: **MSYS2 MinGW64** (`g++`) tramite **CMake**.

## Build da terminale (PowerShell)

```powershell
cmake --preset mingw-debug
cmake --build --preset mingw-debug
.\build\bin\engine.exe
```

## VSCode

- Task: `Configure (CMake)`
- Task: `Build (CMake)`
- Task: `Run Engine`
- Debug: `Debug Engine (gdb)`

