@echo off

rem Check if the build directory exists; if not, create it
if not exist build mkdir build

rem Navigate to the build directory
cd build

rem Run CMake to configure the project with Visual Studio 2022
cmake .. -G "Visual Studio 17 2022"

rem Compile the project using MSBuild in "Release" mode
msbuild CharCounter.sln /p:Configuration=Release

rem Check the return code of MSBuild to determine if the compilation was successful and display an appropriate message
if %errorlevel% equ 0 (
    echo Compilation successful.
) else (
    echo There was an error during compilation.
)

rem Run the compiled program
start Release\CharCounter.exe

rem Return to the original directory
cd ..

rem Pause for message visibility (optional)
pause
