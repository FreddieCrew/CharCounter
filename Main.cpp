#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;

int countCharactersInFile(const std::wstring& filePath) {
    wifstream file(filePath);
    if (!file.is_open()) {
        wcerr << L"Error while listing files: " << filePath << endl;
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
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(L"txt\\*.*", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        wcerr << L"Error while listing the folder 'txt'." << endl;
        return 1;
    }

    int totalCharacterCount = 0;

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            wstring filePath = L"txt\\" + wstring(findFileData.cFileName);

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

    wcout << L"Characters total: " << totalCharacterCount << endl;

    system("pause");
    return 0;
}
