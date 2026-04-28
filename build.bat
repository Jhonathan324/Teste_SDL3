@echo off

if not exist bin\debug mkdir bin\debug

"C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe" ^
-fdiagnostics-color=always ^
-g ^
*.c ^
-IC:/Users/17898981/SDL3/include ^
-IC:/Users/17898981/SDL3_image/include ^
-IC:/Users/17898981/SDL3_ttf/include ^
-LC:/Users/17898981/SDL3/lib ^
-LC:/Users/17898981/SDL3_image/lib ^
-LC:/Users/17898981/SDL3_ttf/lib ^
-lSDL3 ^
-lSDL3_image ^
-lSDL3_ttf ^
-o bin\debug\main.exe

echo Build finalizado.