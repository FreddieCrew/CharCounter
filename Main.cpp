#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <Windows.h>

namespace fs = std::filesystem;

// Define colors for the console
enum ConsoleColor {
    COLOR_DEFAULT = 7,
    COLOR_RED = 12,
    COLOR_GREEN = 10,
};

// Function to count the number of characters in a text file.
std::size_t CountCharactersInFile(const fs::path& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        // Set console color to red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        std::cerr << "Error while opening file: " << filePath.string() << std::endl;
        // Reset to default color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        return 0;
    }

    std::size_t characterCount = 0;
    char c;
    while (file.get(c)) {
        characterCount++;
    }

    return characterCount;
}

// Function to log a message to a log buffer.
void LogMessage(const std::string& message, std::string& logBuffer) {
    std::time_t now = std::time(nullptr);
    char timestamp[50];
    std::tm timeInfo;
    localtime_s(&timeInfo, &now); // Use localtime_s for thread safety
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &timeInfo);
    logBuffer += timestamp + message + "\n";
}

// Function to write the log buffer to a log file.
void WriteLogToFile(const std::string& logBuffer) {
    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile) {
        logFile << logBuffer;
    }
}

// Function to display an informative message in the console.
void DisplayInfoMessage(const std::string& message) {
    // Set console color to green
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
    std::cout << message << std::endl;
    // Reset to default color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
}

int main() {
    // Directory path
    fs::path directoryPath = "txt";

    // Check if the directory exists and create it if it doesn't
    if (!fs::exists(directoryPath)) {
        try {
            fs::create_directory(directoryPath);
        }
        catch (const fs::filesystem_error& e) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            // Display an error message as a popup
            MessageBoxA(nullptr, ("Error creating directory: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            std::string logBuffer;
            LogMessage("Error creating directory.", logBuffer);
            WriteLogToFile(logBuffer);
            return 1;
        }
    }

    if (!fs::is_directory(directoryPath)) {
        // Set console color to red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        // Display an error message as a popup
        MessageBoxA(nullptr, "'txt' is not a valid directory.", "Error", MB_ICONERROR);
        // Reset to default color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        std::string logBuffer;
        LogMessage("'txt' is not a valid directory.", logBuffer);
        WriteLogToFile(logBuffer);
        return 1;
    }

    std::size_t totalCharacterCount = 0;
    bool foundTextFiles = false;
    std::string logBuffer;

    try {
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().has_extension()) {
                foundTextFiles = true;
                std::size_t characterCount = CountCharactersInFile(entry.path());
                totalCharacterCount += characterCount;
                // Display an informative message in the console
                DisplayInfoMessage("File: " + entry.path().string() + " | Characters: " + std::to_string(characterCount));

                // Log to the log buffer
                LogMessage("File: " + entry.path().string() + " | Characters: " + std::to_string(characterCount), logBuffer);
            }
        }

        if (!foundTextFiles) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            // Display an error message as a popup
            MessageBoxA(nullptr, "No text files found in the 'txt' directory.", "Error", MB_ICONERROR);
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            LogMessage("No text files found in the 'txt' directory.", logBuffer);
            WriteLogToFile(logBuffer);
            return 1;
        }

        // Display the total characters in the console
        DisplayInfoMessage("Characters total: " + std::to_string(totalCharacterCount));
        LogMessage("Characters total: " + std::to_string(totalCharacterCount), logBuffer);
        WriteLogToFile(logBuffer);
    }
    catch (const fs::filesystem_error& e) {
        // Set console color to red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        // Display an error message as a popup
        MessageBoxA(nullptr, ("Filesystem error: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
        // Reset to default color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        LogMessage("Filesystem error: " + std::string(e.what()), logBuffer);
        WriteLogToFile(logBuffer);
        return 1;
    }

    // Pause the console to keep it open.
    system("pause");
    return 0;
}
