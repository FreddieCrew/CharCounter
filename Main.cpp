#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;

// Function to count the number of characters in a text file specified by filePath.
int countCharactersInFile(const std::wstring& filePath) {
    wifstream file(filePath);
    if (!file.is_open()) {
        wcerr << L"Error while opening file: " << filePath << endl;
        return -1;
    }

    int characterCount = 0;
    wchar_t c;
    while (file.get(c)) {
        characterCount++;
    }

    return characterCount;
}

int main() {
    // Variables for listing files in the 'txt' folder.
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(L"txt\\*.*", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        wcerr << L"Error while listing the folder 'txt'." << endl;
        return 1;
    }

    int totalCharacterCount = 0;

    // Loop through files in the 'txt' folder.
    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            wstring filePath = L"txt\\" + wstring(findFileData.cFileName);

            // Check if the file has a ".txt" extension.
            if (wcsstr(findFileData.cFileName, L".txt")) {
                int characterCount = countCharactersInFile(filePath);
                if (characterCount != -1) {
                    totalCharacterCount += characterCount;
                    wcout << L"File: " << filePath << L" | Characters: " << characterCount << endl;
                }
            }
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Display the total character count.
    wcout << L"Characters total: " << totalCharacterCount << endl;

    // Pause the console to keep it open.
    system("pause");
    return 0;
}
