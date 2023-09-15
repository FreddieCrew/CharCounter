# CharCounter
A simple program written in C++ to count the characters in a .txt file.

## How to Build

### Using Visual Studio Code 2022(Not Recommended)
1. Download Visual Studio 2022 from the [Microsoft website](https://visualstudio.microsoft.com/vs/).
2. Open Visual Studio 2022 and select 'Create a new project' and create a new project by choosing 'Create Console App'.
3. Download [GIT](https://git-scm.com/downloads).
4. Open your terminal and run the command `git clone https://github.com/FreddieCrew/CharCounter.git` to clone this repository.
5. Drag and drop the Main.cpp file into the Visual Studio 2022 Solution Explorer.
6. Click on the 'Build' option in the Visual Studio 2022 navigation menu.
7. Right-click on the project name in the Solution Explorer and select 'Properties' or press **ALT + Enter**.
8. In the project settings, double-click on the 'C/C++' option to expand its contents.
9. Click on 'Language'.
10. Then click on the 'C++ Standard Language' option and select 'Standard ISO C++20'.
11. Then click on the 'Build Solution' option in the Visual Studio 2022 navigation menu or press **CTRL + Shift + B**.

### Using CMake(Recommended)
1. Download CMake from the [official CMake website](https://cmake.org/download/).
2. Download [GIT](https://git-scm.com/downloads).
3. Open your terminal (On Windows, press CTRL + R and type 'cmd' in the Run window).
4. Run the command `git clone https://github.com/FreddieCrew/CharCounter.git` to clone this repository.
5. Then run the command `cd CharCounter` to navigate to the cloned repository folder.
6. Create a folder named 'build' by running the command `mkdir build`.
7. Enter the 'build' folder by running the command `cd build`.
8. Run the command `cmake ..`.
9. Then build the project by running the command `cmake --build .`.

## Usage(After Building)
1. Clone this repository by following the build steps above.
2. Place the .txt file(s) inside the "txt" folder.
3. Then run the program.

Useless and pointless? Yeah, I know, this program was created for the sole purpose of being an exercise.
