<<<<<<< HEAD
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

=======
# Chess Engine

Simple chess engine written in C++.

Features:

- Bitboard representation
- Move generation
- Alpha-Beta search
- MVV-LVA move ordering
- UCI protocol support

## Build

Requires:

- C++20
- g++

Compile:

g++ -std=c++20 Engine.cpp ...

## Future improvements

- Quiescence search
- Transposition tables
- Iterative deepening

Quando modifichi il codice:
git add .
git commit -m "added quiescence search"
git push
>>>>>>> 0777a26f3b70c85c0c994328753e0c890100ed5a
