# 🎓 Quiz Master (Professional CLI Version)

A robust, modular Command Line Interface (CLI) Quiz Application written in **C**. This project features a real-time timer, multi-user authentication, and persistent data storage across multiple subjects.

## 🚀 Key Features
* **Modular Architecture:** Code is split into logical modules (`auth`, `quiz_engine`, `utils`) for better maintainability and professional structure.
* **Real-time Asynchronous Timer:** Implementation of a non-blocking countdown timer that allows for user input while the clock ticks.
* **Persistent Storage:** Uses standard File I/O to manage:
    * **Question Banks:** External `.txt` files for Math, Science, GK, CS, and English.
    * **Leaderboard:** Tracks high scores globally across all users.
    * **User History:** Stores past attempts with timestamps.
* **Defensive Programming:** Robust input validation using `fgets` and `sscanf` to prevent buffer overflows and handle invalid user inputs gracefully.

---

## 📂 Project Structure

| File | Purpose |
| :--- | :--- |
| `main.c` | Entry point and main menu navigation. |
| `common.h` | Shared macros, color codes, and data structures. |
| `auth.c` / `.h` | User login, signup, and masked password logic. |
| `quiz_engine.c` / `.h` | Core game logic, timer, and leaderboard updates. |
| `utils.c` / `.h` | UI helpers (borders, centering, screen clearing). |
| `*.txt` | Subject-specific question banks (Math, Science, etc.). |

---

## 🛠️ Installation & Compilation

### Prerequisites
* **OS:** Windows (uses `windows.h` and `conio.h`).
* **Compiler:** GCC (MinGW).

### Compiling the Project
Since this is a modular project, all source files must be compiled together. Open your terminal in the project folder and run:

```bash
gcc main.c auth.c quiz_engine.c utils.c -o QuizMaster.exe (press enter)
```bash
./QuizMaster.exe(press enter)
