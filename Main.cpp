#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <Windows.h>
#include <conio.h>

namespace fs = std::filesystem;

// Define console colors
enum ConsoleColor {
    COLOR_DEFAULT = 7,
    COLOR_RED = 12,
    COLOR_GREEN = 10,
};

// Constants
const char* kLogFileName = "log.txt";
const char* kDirectoryPath = "txt";

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
void LogMessage(const char* message, char*& log_buffer) {
    std::time_t now = std::time(nullptr);
    char timestamp[50];
    std::tm time_info;
    localtime_s(&time_info, &now); // Use localtime_s for thread safety
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &time_info);

    // Allocate memory for the new log message
    size_t len_timestamp = strlen(timestamp);
    size_t len_message = strlen(message);
    char* new_message = new char[len_timestamp + len_message + 2]; // 2 for '\n' and null terminator
    strcpy_s(new_message, len_timestamp + len_message + 2, timestamp);
    strcat_s(new_message, len_timestamp + len_message + 2, message);
    strcat_s(new_message, len_timestamp + len_message + 2, "\n");

    // Append the new message to the log buffer
    if (log_buffer) {
        size_t len_log_buffer = strlen(log_buffer);
        char* temp = new char[len_log_buffer + len_timestamp + len_message + 2];
        strcpy_s(temp, len_log_buffer + len_timestamp + len_message + 2, log_buffer);
        strcat_s(temp, len_log_buffer + len_timestamp + len_message + 2, new_message);
        delete[] log_buffer;
        log_buffer = temp;
    }
    else {
        log_buffer = new_message;
    }
}

// Function to write the log buffer to a log file.
void WriteLog(const char* log_buffer) {
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
void DisplayInfo(const char* message) {
    // Set console color to green
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GREEN);
    std::cout << message << std::endl;
    // Reset to default color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
}

int main() {
    char choice;
    bool restart = false;

    do {
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
                char* log_buffer = nullptr;
                LogMessage("Error creating directory.", log_buffer);
                WriteLog(log_buffer);
                delete[] log_buffer;
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
            char* log_buffer = nullptr;
            LogMessage("'txt' is not a valid directory.", log_buffer);
            WriteLog(log_buffer);
            delete[] log_buffer;
            return 1;
        }

        std::size_t total_character_count = 0;
        bool found_text_files = false;
        char* log_buffer = nullptr;

        try {
            for (const auto& entry : fs::directory_iterator(kDirectoryPath)) {
                if (entry.is_regular_file() && entry.path().has_extension()) {
                    // Check if the file is a text file
                    if (entry.path().extension() == ".txt") {
                        found_text_files = true;
                        std::size_t character_count = CountCharacters(entry.path());
                        total_character_count += character_count;
                        // Display an informative message in the console
                        const char* format = "File: %s | Characters: %zu";
                        char message[256];
                        sprintf_s(message, sizeof(message), format, entry.path().string().c_str(), character_count);
                        DisplayInfo(message);

                        // Log to the log buffer
                        LogMessage(message, log_buffer);
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
                const char* log_message = "No text files found in the 'txt' directory.";
                LogMessage(log_message, log_buffer);
            }

            // Display the total characters in the console
            const char* total_message = "Total characters: ";
            char total_message_buffer[256];
            sprintf_s(total_message_buffer, sizeof(total_message_buffer), "%s%zu", total_message, total_character_count);
            DisplayInfo(total_message_buffer);
            LogMessage(total_message_buffer, log_buffer);
            WriteLog(log_buffer);
            delete[] log_buffer;
        }
        catch (const fs::filesystem_error& e) {
            // Set console color to red
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            // Display an error message as a popup
            MessageBoxA(nullptr, ("Filesystem error: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
            // Reset to default color
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            char* log_buffer = nullptr;
            LogMessage(("Filesystem error: " + std::string(e.what())).c_str(), log_buffer);
            WriteLog(log_buffer);
            delete[] log_buffer;
            return 1;
        }
        catch (const std::exception& e) {
            // Handle other exceptions
            std::cerr << "Exception: " << e.what() << std::endl;
            return 1;
        }

        // Ask the user if they want to restart or quit
        std::cout << "Press 'R' to restart or 'Q' to quit: \n";
        choice = _getch(); // Get a single character from the user

        if (choice == 'R' || choice == 'r') {
            restart = true;
        }
        else if (choice == 'Q' || choice == 'q') {
            restart = false;
        }
        else {
            restart = false;
        }
    } while (restart);

    return 0;
}
