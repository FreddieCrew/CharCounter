#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <memory>
#include <Windows.h>

namespace fs = std::filesystem;

// Define console colors
enum ConsoleColor {
    COLOR_DEFAULT = 7,
    COLOR_RED = 12,
    COLOR_GREEN = 10,
};

// Constants
const std::string kLogFileName = "log.txt";
const std::string kDirectoryPath = "txt";

// Function to count characters in a text file.
std::size_t CountCharacters(const fs::path& file_path) {
    try {
        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            std::cerr << "Error opening file: " << file_path.string() << std::endl;
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            return 0;
        }

        std::size_t character_count = 0;
        char c;
        while (file.get(c)) {
            character_count++;
        }

        // Explicitly close the file after reading.
        file.close();

        return character_count;
    }
    catch (const std::exception& e) {
        // Handle exceptions related to file operations
        std::cerr << "Exception while processing file: " << e.what() << std::endl;
        return 0;
    }
}

// Function to log a message to a log buffer.
void LogMessage(const std::string& message, std::string& log_buffer) {
    std::time_t now = std::time(nullptr);
    char timestamp[50];
    std::tm time_info;
    localtime_s(&time_info, &now); // Use localtime_s for thread safety
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &time_info);
    log_buffer += timestamp + message + "\n";
}

// Function to write the log buffer to a log file.
void WriteLog(const std::string& log_buffer) {
    try {
        std::ofstream log_file(kLogFileName, std::ios::app);
        if (log_file) {
            log_file << log_buffer;
        }
    }
    catch (const std::exception& e) {
        // Handle exceptions related to file operations
        std::cerr << "Exception while writing to log file: " << e.what() << std::endl;
    }
}

// Function to display an informative message in the console.
void DisplayInfo(const std::string& message) {
    // Set console color to green
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
    std::cout << message << std::endl;
    // Reset to default color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
}

int main() {
    // Check if the directory exists and create it if it doesn't
    if (!fs::exists(kDirectoryPath)) {
        try {
            fs::create_directory(kDirectoryPath);
        }
        catch (const fs::filesystem_error& e) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            // Display an error message as a popup
            MessageBoxA(nullptr, ("Error creating directory: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            std::string log_buffer;
            LogMessage("Error creating directory.", log_buffer);
            WriteLog(log_buffer);
            return 1;
        }
    }

    if (!fs::is_directory(kDirectoryPath)) {
        // Set console color to red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        // Display an error message as a popup
        MessageBoxA(nullptr, "'txt' is not a valid directory.", "Error", MB_ICONERROR);
        // Reset to default color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        std::string log_buffer;
        LogMessage("'txt' is not a valid directory.", log_buffer);
        WriteLog(log_buffer);
        return 1;
    }

    std::size_t total_character_count = 0;
    bool found_text_files = false;
    std::string log_buffer;

    try {
        for (const auto& entry : fs::directory_iterator(kDirectoryPath)) {
            if (entry.is_regular_file() && entry.path().has_extension()) {
                // Check if the file is a text file
                if (entry.path().extension() == ".txt") {
                    found_text_files = true;
                    std::size_t character_count = CountCharacters(entry.path());
                    total_character_count += character_count;
                    // Display an informative message in the console
                    DisplayInfo("File: " + entry.path().string() + " | Characters: " + std::to_string(character_count));

                    // Log to the log buffer
                    LogMessage("File: " + entry.path().string() + " | Characters: " + std::to_string(character_count), log_buffer);
                }
            }
        }

        if (!found_text_files) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            // Display an error message as a popup
            MessageBoxA(nullptr, "No text files found in the 'txt' directory.", "Error", MB_ICONERROR);
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            LogMessage("No text files found in the 'txt' directory.", log_buffer);
            WriteLog(log_buffer);
            return 1;
        }

        // Display the total characters in the console
        DisplayInfo("Total characters: " + std::to_string(total_character_count));
        LogMessage("Total characters: " + std::to_string(total_character_count), log_buffer);
        WriteLog(log_buffer);
    }
    catch (const fs::filesystem_error& e) {
        // Set console color to red
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
        // Display an error message as a popup
        MessageBoxA(nullptr, ("Filesystem error: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
        // Reset to default color
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        LogMessage("Filesystem error: " + std::string(e.what()), log_buffer);
        WriteLog(log_buffer);
        return 1;
    }
    catch (const std::exception& e) {
        // Handle other exceptions
        // You can log the exception details or display an error message here
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    // Pause the console to keep it open.
    system("pause");
    return 0;
}
