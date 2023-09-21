@echo off

REM Output executable name
set OUTPUT=CharCounter.exe

REM Compilation using the default system compiler (assumed to be Visual C++ Compiler)
cl /std:c++17 /Fe:%OUTPUT% main.cpp

REM Check if compilation was successful
if %ERRORLEVEL% == 0 (
    echo Compilation successful. Run %OUTPUT% to execute the program.
) else (
    echo Error during compilation.
)
