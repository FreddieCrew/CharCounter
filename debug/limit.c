#include <stdio.h>
#include <stdlib.h>

// Function to generate a text file filled with null bytes
void generateTextFile(const char* filePath, const long long fileSize) {
    FILE* file;
    errno_t err;

    // Open the file
    if ((err = fopen_s(&file, filePath, "wb")) != 0) {
        perror("Failed to open the file for writing");
        exit(EXIT_FAILURE);
    }

    const int bufferSize = 1024;
    char* buffer = (char*)malloc(bufferSize);

    if (buffer == NULL) {
        perror("Failed to allocate memory for writing");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Initialize the buffer with null bytes
    memset(buffer, 0, bufferSize);

    long long bytesWritten = 0;

    while (bytesWritten < fileSize) {
        long long bytesToWrite = fileSize - bytesWritten;
        if (bytesToWrite > bufferSize) {
            bytesToWrite = bufferSize;
        }

        size_t bytesWrittenThisTime = fwrite(buffer, 1, bytesToWrite, file);

        if (bytesWrittenThisTime != (size_t)bytesToWrite) {
            perror("Error writing to file");
            fclose(file);
            free(buffer);
            exit(EXIT_FAILURE);
        }

        bytesWritten += bytesWrittenThisTime;
    }

    free(buffer); // Deallocate the buffer
    fclose(file);

    printf("File generated successfully: %s\n", filePath);
}

int main() {
    const char* folderName = "txt";
    const char* fileName = "txt/textfile.txt";
    const long long fileSize = 10LL * 1024 * 1024; // 10MB

    // Generate the text file
    generateTextFile(fileName, fileSize);

    return 0;
}
