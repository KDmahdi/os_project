
#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    SetConsoleTitleA("Mahdi CMD");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char line[1024];

    while (1) {
        char currentDir[1024];
        GetCurrentDirectoryA(sizeof(currentDir), currentDir); 
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("%s> ", currentDir);


        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\r\n")] = 0;

        
        if (line[0] == 0) continue;

        
        if (strcmp(line, "exit") == 0) break;

        
        if (strncmp(line, "cd ", 3) == 0) {
            const char *path = line + 3;
            if (SetCurrentDirectoryA(path)) {
            } else {
                DWORD err = GetLastError();
                printf("cd failed (error %lu)\n", err);
            }
            continue;
        }

        // creat fille
        if (strncmp(line, "touch ", 6) == 0) {
            const char *filename = line + 6; 

            HANDLE hFile = CreateFileA(
                filename,               // اسم فایلی که می‌خوایم بسازیم
                GENERIC_WRITE,          // دسترسی نوشتن (برای ساختن فایل لازمه)
                0,                      // اجازه اشتراک‌گذاری نده
                NULL,                   // تنظیمات امنیتی پیش‌فرض
                CREATE_NEW,             // مهم: فقط اگه فایل وجود نداره بسازش
                FILE_ATTRIBUTE_NORMAL,  // یه فایل معمولی بساز
                NULL
            );

            if (hFile == INVALID_HANDLE_VALUE) {
                // اگر ساخت فایل شکست خورد
                DWORD err = GetLastError();
                printf("Failed to create file (error %lu)\n", err);
            } else {
                // اگر موفق شد
                printf("File '%s' created successfully.\n", filename);
                CloseHandle(hFile); // خیلی مهم: فایل رو می‌سازیم و بلافاصله می‌بندیمش
            }
            continue;
        }

        // ساخت newe folder
        if (strncmp(line, "mkdir ", 6) == 0) {
            const char *dirname = line + 6; // 6 = طول "mkdir "

            if (CreateDirectoryA(dirname, NULL)) {
                
                printf("Directory '%s' created successfully.\n", dirname);
            } else {
                // اگر شکست خورد
                DWORD err = GetLastError();
                printf("Failed to create directory (error %lu)\n", err);
            }
            continue; 
        }


        // باز کدرن فایل 
        if (strncmp(line, "start ", 6) == 0) {
            const char *filepath = line + 6; 
            HINSTANCE result = ShellExecuteA(
                NULL,       // هندل پنجرهٔ والد، ما نداریم پس NULL
                "open",     // عملیات: "open", "edit", "print" 
                filepath,   // آدرس فایل، پوشه یا حتی وب‌سایت
                NULL,       // پارامتر اضافی نداریم
                NULL,       // دایرکتوری پیش‌فرض
                SW_SHOWNORMAL // حالت نمایش پنجره
            );

            if ((intptr_t)result <= 32) {
                // Failed to start '%s'. Check if the file exists and you have a default program for it.\n
                printf("😒😒😒");
            }
            

            continue;
        }


        if (strncmp(line, "copy ", 5) == 0) {
            char *command = strtok(line, " ");    
            char *source = strtok(NULL, " ");     
            char *destination = strtok(NULL, " ");

            if (source && destination) { // چک می‌کنیم که هر دو آرگومان وجود داشته باشن
                // پارامتر سوم TRUE هست یعنی اگه فایل مقصد وجود داشت، کپی نکن و خطا بده
                if (CopyFileA(source, destination, TRUE)) {
                    printf("afrin drost  shod inam  intori shod 😃 '%s' to '%s'.\n", source, destination);
                } else {
                    DWORD err = GetLastError();
                    printf("heee kharab kardi  (error %lu)\n", err);
                }
            } else {
                printf(" Usage: copy <source_file> <destination_file>\n");
            }
            continue; 
        }

        if (strncmp(line, "write ", 6) == 0) {
            char *args = line + 6; 
            char *filename_end = strchr(args, ' ');

            if (filename_end == NULL) {
                
                printf("dorost vared nakardi ke ");
            } else {
                *filename_end = '\0'; // اینجا موقتا رشته رو میشکنیم تا اسم فایل جدا بشه
                char *filename = args;
                char *content = filename_end + 1; // بقیه رشته میشه محتوای ما

                // باز کردن فایل. اگر وجود نداشت، می‌سازدش. اگر وجود داشت، محتوای قبلی رو پاک می‌کنه
                HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

                if (hFile == INVALID_HANDLE_VALUE) {
                    printf("Failed to open or create file '%s' (error %lu)\n", filename, GetLastError());
                } else {
                    DWORD bytesWritten;
                    // نوشتن محتوا در فایل
                    if (WriteFile(hFile, content, strlen(content), &bytesWritten, NULL)) {
                        printf("Successfully wrote to '%s'.\n", filename);
                    } else {
                        printf("Failed to write to file (error %lu)\n", GetLastError());
                    }
                    CloseHandle(hFile); 
                }
            }
            continue; 
        }



        
        char cmdline[1024];
        printf("CMD system");

        snprintf(cmdline , sizeof(cmdline),"cmd.exe /c %s", line);


        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // CreateProcess: NULL for application name -> سیستم از command line برای پیدا کردن exe استفاده می‌کنه
        BOOL ok = CreateProcessA(
            NULL,       // lpApplicationName
            cmdline,    // lpCommandLine (این رشته ممکنه تغییر یابد)
            NULL,       // lpProcessAttributes
            NULL,       // lpThreadAttributes
            FALSE,      // bInheritHandles
            0,          // dwCreationFlags
            NULL,       // lpEnvironment
            NULL,       // lpCurrentDirectory (NULL = وراثت از پروسه پدر)
            &si,        // lpStartupInfo
            &pi         // lpProcessInformation
        );

        if (!ok) {
            DWORD err = GetLastError();
            printf("CreateProcess failed (error %lu)\n", err);
            continue;
        }

        // صبر تا پروسهٔ فرزند تمام شود
        WaitForSingleObject(pi.hProcess, INFINITE);

        // بستن هندل‌ها
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    return 0;
}
