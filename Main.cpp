#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <conio.h>
#include <Windows.h>

namespace fs = std::filesystem;

enum ConsoleColor {
    COLOR_DEFAULT = 7,
    COLOR_RED = 12,
    COLOR_GREEN = 10,
};

const char* kLogFileName = "log.txt";
const char* kDirectoryPath = "txt";
const std::size_t kMaxFileSize = 10 * 1024 * 1024; // 10 MB

// Class to manage log file
class LogFile {
public:
    LogFile(const std::string& fileName) : fileName_(fileName) {
        logFile_.open(fileName_, std::ios::app);
        if (!logFile_.is_open()) {
            throw std::runtime_error("Error opening file: " + fileName_);
        }
    }

    ~LogFile() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    void Write(const std::string& message) {
        logFile_ << message << "\n";
    }

private:
    std::string fileName_;
    std::ofstream logFile_;
};

// Function to log a message with a timestamp
void LogMessage(const std::string& message, LogFile& logFile) {
    std::time_t now = std::time(nullptr);
    char timestamp[50];
    std::tm time_info;
    localtime_s(&time_info, &now);
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &time_info);
    logFile.Write(timestamp + message);
}

// Function to count characters in a text file
std::size_t CountCharacters(const fs::path& file_path) {
    try {
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Error opening file: " + file_path.string());
        }

        std::size_t character_count = 0;
        char c;

        while (file.get(c)) {
            character_count++;
            if (character_count > kMaxFileSize) {
                throw std::runtime_error("File size exceeds the limit (10 MB): " + file_path.string());
            }
        }

        return character_count;
    }
    catch (const std::exception& ex) {
        throw; // Re-throw the exception
    }
}

// Function to display a message in the console with color
void DisplayInfo(const std::string& message, ConsoleColor color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    std::cout << message << std::endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
}

int main() {
    char choice;
    bool restart = false;

    do {
        try {
            // Check if the directory exists and create it if it doesn't
            if (!fs::exists(kDirectoryPath)) {
                fs::create_directory(kDirectoryPath);
            }

            // Check if the path is a valid directory
            if (!fs::is_directory(kDirectoryPath)) {
                throw std::runtime_error("'txt' is not a valid directory.");
            }

            std::size_t total_character_count = 0;
            bool found_text_files = false;
            LogFile logFile(kLogFileName);

            // Iterate through files in the directory
            for (const auto& entry : fs::directory_iterator(kDirectoryPath)) {
                if (entry.is_regular_file() && entry.path().has_extension() && entry.path().extension() == ".txt") {
                    found_text_files = true;
                    std::size_t character_count = CountCharacters(entry.path());
                    total_character_count += character_count;

                    // Display information about the file in the console
                    char message[256];
                    sprintf_s(message, sizeof(message), "File: %s | Characters: %zu", entry.path().string().c_str(), character_count);
                    DisplayInfo(message, COLOR_GREEN);

                    // Log the message with timestamp
                    LogMessage(message, logFile);
                }
            }

            // Handle case where no text files were found
            if (!found_text_files) {
                throw std::runtime_error("No text files found in the 'txt' directory.");
            }

            // Display the total character count in the console
            char total_message[256];
            sprintf_s(total_message, sizeof(total_message), "Total characters: %zu", total_character_count);
            DisplayInfo(total_message, COLOR_DEFAULT);

            // Log the total character count with timestamp
            LogMessage(total_message, logFile);
        }
        catch (const std::exception& ex) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_RED);
            std::cerr << "Exception: " << ex.what() << std::endl;

            // Log the exception with timestamp
            LogFile logFile(kLogFileName);
            LogMessage("Exception: " + std::string(ex.what()), logFile);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
        }

        std::cout << "Press 'R' to restart or 'Q' to quit: \n";
        choice = _getch();

        if (choice == 'R' || choice == 'r') {
            restart = true;
        }
        else {
            restart = false;
        }
    } while (restart);

    return 0;
}
