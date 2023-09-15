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
const std::size_t kMaxFileSize = 10 * 1024 * 1024; // 10 MB

// Function to log a message to a log buffer.
void LogMessage(const std::string& message, std::string& log_buffer) {
    // Get the current timestamp
    std::time_t now = std::time(nullptr);
    char timestamp[50];
    std::tm time_info;
    localtime_s(&time_info, &now); // Use localtime_s for thread safety
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &time_info);

    // Append the message to the log buffer with timestamp
    log_buffer += timestamp + message + "\n";
}

// Function to write the log buffer to a log file.
void WriteLog(const std::string& log_buffer) {
    try {
        // Open the log file in append mode
        std::ofstream log_file(kLogFileName, std::ios::app);
        if (log_file) {
            // Write the log buffer to the file
            log_file << log_buffer;
        }
    }
    catch (const std::exception& e) {
        // Handle exceptions related to file operations
        std::cerr << "Exception while writing to log file: " << e.what() << std::endl;
    }
}

// Function to count characters in a text file.
std::size_t CountCharacters(const fs::path& file_path) {
    try {
        // Check file size
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file) {
            // Handle file opening error by throwing an exception
            throw std::runtime_error("Error opening file: " + file_path.string());
        }

        std::size_t file_size = static_cast<std::size_t>(file.tellg());
        if (file_size > kMaxFileSize) {
            // Handle file size exceeding the limit by throwing an exception
            throw std::runtime_error("File size exceeds the limit (10 MB): " + file_path.string());
        }

        std::size_t character_count = 0;
        char c;
        file.seekg(0); // Move the file pointer to the beginning
        while (file.get(c)) {
            character_count++;
        }

        // Explicitly close the file after reading.
        file.close();

        return character_count;
    }
    catch (const std::exception& e) {
        // Handle exceptions related to file operations and re-throw them
        std::cerr << "Exception while processing file: " << e.what() << std::endl;
        throw;
    }
}

// Function to display an informative message in the console.
void DisplayInfo(const std::string& message, ConsoleColor color) {
    // Set console color
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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
                // Handle directory creation error
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
                MessageBoxA(nullptr, ("Error creating directory: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
                std::string log_buffer;
                LogMessage("Error creating directory: " + std::string(e.what()), log_buffer);
                WriteLog(log_buffer);
                return 1;
            }
        }

        if (!fs::is_directory(kDirectoryPath)) {
            // Handle invalid directory error
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            MessageBoxA(nullptr, "'txt' is not a valid directory.", "Error", MB_ICONERROR);
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
                        const char* format = "File: %s | Characters: %zu";
                        char message[256];
                        sprintf_s(message, sizeof(message), format, entry.path().string().c_str(), character_count);
                        DisplayInfo(message, COLOR_GREEN);

                        // Log the message
                        LogMessage(message, log_buffer);
                    }
                }
            }

            if (!found_text_files) {
                // Handle no text files found error
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
                MessageBoxA(nullptr, "No text files found in the 'txt' directory.", "Error", MB_ICONERROR);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
                const char* log_message = "No text files found in the 'txt' directory.";
                LogMessage(log_message, log_buffer);
            }

            // Display the total characters in the console
            const char* total_message = "Total characters: ";
            char total_message_buffer[256];
            sprintf_s(total_message_buffer, sizeof(total_message_buffer), "%s%zu", total_message, total_character_count);
            DisplayInfo(total_message_buffer, COLOR_DEFAULT);
            LogMessage(total_message_buffer, log_buffer);
            WriteLog(log_buffer);
        }
        catch (const fs::filesystem_error& e) {
            // Handle filesystem error
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            MessageBoxA(nullptr, ("Filesystem error: " + std::string(e.what())).c_str(), "Error", MB_ICONERROR);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
            std::string log_buffer;
            LogMessage(("Filesystem error: " + std::string(e.what())), log_buffer);
            WriteLog(log_buffer);
            return 1;
        }
        catch (const std::exception& e) {
            // Handle other exceptions
            std::cerr << "Exception: " << e.what() << std::endl;
            std::string log_buffer;
            LogMessage("Exception: " + std::string(e.what()), log_buffer);
            WriteLog(log_buffer);
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
