@echo off

if not exist bin\debug mkdir bin\debug

"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" ^
-fdiagnostics-color=always ^
-g ^
*.c ^
-IC:/Users/17898981/SDL3/include ^
-IC:/Users/17898981/SDL3_image/include ^
-LC:/Users/17898981/SDL3/lib ^
-LC:/Users/17898981/SDL3_image/lib ^
-lSDL3 ^
-lSDL3_image ^
-o bin\debug\main.exe

echo Build finalizado.