@echo off
REM ============================
REM make.bat - Windows build script for Ironbrew Inn
REM ============================

REM Set the compiler
set CC=cl

REM Set include and output folders (adjust if needed)
set INCLUDES=.

REM Compile all .c files into a single executable
%CC% /Fe:ironbrew.exe main.c sounds.c game.c tools.c /I %INCLUDES%

if %ERRORLEVEL% NEQ 0 (
    echo Build failed.
) else (
    echo Build succeeded. Run ironbrew.exe to play!
)