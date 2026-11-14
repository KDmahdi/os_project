<div align="center">

# 💻 Mahdi CMD (`os_p1_mahdi_hashemi`)

**A lightweight, custom Windows Command-Line Interpreter (CLI) written in C using native Win32 APIs.**

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![API](https://img.shields.io/badge/API-Win32_Native-2ea44f?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

</div>

---

## 📖 Overview

**Mahdi CMD** is an Operating Systems project implementing a custom command-line shell for Windows. Built directly on top of the **Windows API (`windows.h`)**, it provides an interactive red-accented terminal prompt with built-in file management commands while seamlessly delegating external system commands to child processes via `CreateProcessA`.

---

## ✨ Key Features

- 🎨 **Custom Terminal Styling**: Dynamically sets the console window title to `"Mahdi CMD"` and configures high-intensity red foreground text via `SetConsoleTitleA` and `SetConsoleTextAttribute`.
- 📂 **Native Directory Navigation & Creation**: Implements built-in `cd` and `mkdir` commands using `SetCurrentDirectoryA` and `CreateDirectoryA` without spawning external processes.
- 📄 **Direct File Manipulation**: Supports creating (`touch`), deleting (`rm`), copying (`copy`), and writing text directly into files (`write`) via native Windows file handles (`CreateFileA`, `WriteFile`, `DeleteFileA`, `CopyFileA`).
- 🚀 **GUI & File Launching**: Integrates `ShellExecuteA` (`start`) to open files, documents, and external applications using default Windows shell associations.
- ⚙️ **External Process Execution**: Falls back to `CreateProcessA` (`cmd.exe /c <command>`) with inherited standard I/O handles (`STD_INPUT_HANDLE`, `STD_OUTPUT_HANDLE`, `STD_ERROR_HANDLE`) to run any standard Windows CLI utility synchronously.
- 🛡️ **Explicit Win32 Error Reporting**: Captures and displays detailed system error codes (`GetLastError()`) whenever file, directory, or process operations fail.

---

## 🛠️ Tech Stack

| Layer | Technology | Purpose |
| :--- | :--- | :--- |
| **Language** | C (C99 / C11) | Core shell loop, string parsing, and memory management |
| **System API** | Win32 API (`windows.h`) | Console control, file I/O, directory operations, and process creation |
| **Standard Library** | C Standard Library (`stdio.h`, `string.h`) | Buffered terminal I/O and command tokenization |
| **Target OS** | Microsoft Windows | Native execution environment |

---

## 📁 Directory Tree

```text
os_project/
├── os_p1_mahdi_hashemi.c   # Main shell entry point, built-in command handlers, and Win32 process spawner
└── README.md               # Project documentation and usage guide
```

---

## 🚀 Setup & Run Commands

### Prerequisites

- **Operating System**: Windows 10 or Windows 11
- **C Compiler**: MinGW-w64 (`gcc`), Clang, or MSVC (`cl.exe`)

### 1. Clone the Repository

```bash
git clone https://github.com/KDmahdi/os_project.git
cd os_project
```

### 2. Compile the Source Code

**Using GCC (MinGW-w64):**

```bash
gcc os_p1_mahdi_hashemi.c -o mahdi_cmd.exe -Wall -Wextra
```

**Using MSVC (`Developer Command Prompt`):**

```bash
cl os_p1_mahdi_hashemi.c /Fe:mahdi_cmd.exe shell32.lib
```

### 3. Run the Shell

```bash
./mahdi_cmd.exe
```

---

## 💻 Built-in Commands & Usage Examples

### Command Reference

| Command | Syntax | Win32 API Used | Description |
| :--- | :--- | :--- | :--- |
| `cd` | `cd <path>` | `SetCurrentDirectoryA` | Changes the current working directory |
| `mkdir` | `mkdir <dirname>` | `CreateDirectoryA` | Creates a new directory in the current path |
| `touch` | `touch <filename>` | `CreateFileA` | Creates a new empty file (fails if it already exists) |
| `write` | `write <filename> <content>` | `CreateFileA` + `WriteFile` | Creates or overwrites a file with the specified text content |
| `copy` | `copy <source> <destination>` | `CopyFileA` | Copies a file to a new destination (fails if destination exists) |
| `rm` | `rm <filename>` | `DeleteFileA` | Permanently deletes the specified file |
| `start` | `start <filepath>` | `ShellExecuteA` | Opens a file, folder, or executable via Windows Shell |
| `clear` | `clear` | `system("cls")` | Clears the console screen buffer |
| `exit` | `exit` | — | Terminates the shell session cleanly |
| `<external>` | `<any windows command>` | `CreateProcessA` | Executes external commands via `cmd.exe /c` |

### Interactive Session Example

```text
C:\Users\Mahdi\Desktop> mkdir test_workspace
Directory 'test_workspace' created successfully.

C:\Users\Mahdi\Desktop> cd test_workspace
C:\Users\Mahdi\Desktop\test_workspace> touch notes.txt
File 'notes.txt' created successfully.

C:\Users\Mahdi\Desktop\test_workspace> write notes.txt Hello from Mahdi CMD!
Successfully wrote to 'notes.txt'.

C:\Users\Mahdi\Desktop\test_workspace> copy notes.txt backup.txt
Copied 'notes.txt' to 'backup.txt' successfully.

C:\Users\Mahdi\Desktop\test_workspace> dir
 Volume in drive C is OS
 Directory of C:\Users\Mahdi\Desktop\test_workspace

11/14/2026  03:07 PM    <DIR>          .
11/14/2026  03:07 PM    <DIR>          ..
11/14/2026  03:07 PM                25 backup.txt
11/14/2026  03:07 PM                25 notes.txt

C:\Users\Mahdi\Desktop\test_workspace> rm backup.txt
File 'backup.txt' deleted successfully.

C:\Users\Mahdi\Desktop\test_workspace> exit
```

---

## 🏗️ Architecture & Execution Flow

1. **Initialization**: Configures console title (`Mahdi CMD`) and sets text color to bright red (`FOREGROUND_RED | FOREGROUND_INTENSITY`).
2. **REPL Loop**:
   - **Read**: Fetches the active working directory via `GetCurrentDirectoryA` to render the prompt (`<path>> `) and reads up to `1024` bytes from `stdin`.
   - **Evaluate**: Matches the input prefix against built-in commands (`cd`, `touch`, `rm`, `mkdir`, `start`, `copy`, `write`, `clear`, `exit`).
   - **Delegate**: If no built-in command matches, formats `cmd.exe /c <line>` and launches a synchronous child process via `CreateProcessA`, waiting for completion with `WaitForSingleObject(..., INFINITE)`.

---

## 📄 License

This project is open-source and available under the [MIT License](LICENSE).
