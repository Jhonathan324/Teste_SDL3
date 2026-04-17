@echo off

if not exist bin\debug mkdir bin\debug

"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" ^
-fdiagnostics-color=always ^
-g ^
*.c ^
-IC:/Users/17898981/SDL3/include ^
-LC:/Users/17898981/SDL3/lib ^
-lSDL3 ^
-o bin\debug\main.exe

echo Build finalizado.