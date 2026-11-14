#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    SetConsoleTitleA("Mahdi CMD");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char line[1024];


    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

    while (1) {
        char currentDir[1024];
        GetCurrentDirectoryA(sizeof(currentDir), currentDir); 
        printf("%s> ", currentDir);

        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == 0) continue;

   
        if (strcmp(line, "exit") == 0) break;

  
        if (strncmp(line, "cd ", 3) == 0) {
            const char *path = line + 3;
            if (!SetCurrentDirectoryA(path)) {
                DWORD err = GetLastError();
                printf("cd failed (error %lu)\n", err);
            }
            continue;
        }


        if (strncmp(line, "touch ", 6) == 0) {
            const char *filename = line + 6; 
            HANDLE hFile = CreateFileA(
                filename,
                GENERIC_WRITE,  // permeson 
                0,
                NULL,
                CREATE_NEW,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );

            if (hFile == INVALID_HANDLE_VALUE) {
                DWORD err = GetLastError();
                printf("Failed to create file (error %lu)\n", err);
            } else {
                printf("File '%s' created successfully.\n", filename);
                CloseHandle(hFile);
            }
            continue;
        }


        if (strncmp(line, "rm ", 3) == 0) {
            const char *filename = line + 3;
            if (DeleteFileA(filename)) {
                printf("File '%s' deleted successfully.\n", filename);
            } else {
                DWORD err = GetLastError();
                printf("Failed to delete file '%s' (error %lu)\n", filename, err);
            }
            continue;
        }


        if (strncmp(line, "mkdir ", 6) == 0) {
            const char *dirname = line + 6;
            if (CreateDirectoryA(dirname, NULL)) {
                printf("Directory '%s' created successfully.\n", dirname);
            } else {
                DWORD err = GetLastError();
                printf("Failed to create directory (error %lu)\n", err);
            }
            continue; 
        }

        if (strncmp(line, "start ", 6) == 0) {
            const char *filepath = line + 6; 
            HINSTANCE result = ShellExecuteA(
                NULL,
                "open",
                filepath,
                NULL,
                NULL,
                SW_SHOWNORMAL
            );

            if ((intptr_t)result <= 32) {
                printf("Failed to open file.\n");
            }
            continue;
        }


        if (strncmp(line, "copy ", 5) == 0) {
            char tempLine[1024];
            strncpy(tempLine, line, sizeof(tempLine));
            strtok(tempLine, " ");    
            char *source = strtok(NULL, " ");   //mabda  
            char *destination = strtok(NULL, " "); // maqsad

            if (source && destination) {
                if (CopyFileA(source, destination, TRUE)) {
                    printf("Copied '%s' to '%s' successfully.\n", source, destination);
                } else {
                    DWORD err = GetLastError();
                    printf("Copy failed (error %lu)\n", err);
                }
            } else {
                printf("Usage: copy <source_file> <destination_file>\n");
            }
            continue; 
        }


        if (strncmp(line, "write ", 6) == 0) {
            char *args = line + 6; 
            char *filename_end = strchr(args, ' ');

            if (filename_end == NULL) {
                printf("Usage: write <filename> <content>\n");
            } else {
                *filename_end = '\0';
                char *filename = args;
                char *content = filename_end + 1;

                HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

                if (hFile == INVALID_HANDLE_VALUE) {
                    printf("Failed to open or create file '%s' (error %lu)\n", filename, GetLastError());
                } else {
                    DWORD bytesWritten;
                    if (WriteFile(hFile, content, (DWORD)strlen(content), &bytesWritten, NULL)) {
                        printf("Successfully wrote to '%s'.\n", filename);
                    } else {
                        printf("Failed to write to file (error %lu)\n", GetLastError());
                    }
                    CloseHandle(hFile); 
                }
            }
            continue; 
        }
        if (strcmp(line,"clear")== 0){
            system("cls");
            continue;
        }
        char cmdline[2048];
        snprintf(cmdline, sizeof(cmdline), "cmd.exe /c %s", line);

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

       
        si.dwFlags |= STARTF_USESTDHANDLES;// تنظیمات خروجی  خروجی خطا متصل کن به این ترمینال 
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

        ZeroMemory(&pi, sizeof(pi));


        BOOL ok = CreateProcessA(
            NULL,
            cmdline,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &si,
            &pi
        );

        if (!ok) {
            DWORD err = GetLastError();
            printf("CreateProcess failed (error %lu)\n", err);
            continue;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}