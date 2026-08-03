# tinky_winkey

## What is this program

**tinky_winkey** is a C project for Windows consisting of two executables:
- `svc.exe`: Manages a Windows service (install, start, stop, and remove).
- `winkey.exe`: Runs a message loop and installs a low-level keyboard hook (`WH_KEYBOARD_LL`) to capture key events and log activity to a file.

The goal is to demonstrate, in an academic context, how different Windows APIs can work together in a real project.

## What you learn from this project

With this project, you can study:
- Structure of a C project with multiple files (`srcs/` and `incs/`).
- Creation and management of Windows services.
- Message loop (`GetMessage`, `TranslateMessage`, `DispatchMessage`).
- Use of low-level keyboard hooks.
- Basic process/token manipulation in the Win32 ecosystem.
- Organization of logging and separation of responsibilities by module.

## Important Notice

This project was created **for educational purposes only**.

It must not be used for illegal, malicious activities, or to harm third parties.

The author is not responsible for any misuse of this content.
