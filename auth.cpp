#include "auth.h"
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace Auth {

std::string readPassword(const std::string& prompt) {
    std::cout << prompt;
    std::string password = "";
    char ch;

#ifdef _WIN32
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') { // Enter key
            break;
        } else if (ch == '\b') { // Backspace
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        } else if (ch == 0 || static_cast<unsigned char>(ch) == 224) {
            // Function keys or arrow keys (they send multi-byte keys on Windows)
            _getch(); // Consume next byte of function key
        } else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
#else
    // POSIX terminal implementation for Linux/macOS
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON); // Turn off local echoing and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n <= 0) break;
        if (ch == '\n' || ch == '\r') {
            break;
        } else if (ch == 127 || ch == 8) { // Backspace
            if (!password.empty()) {
                std::cout << "\b \b" << std::flush;
                password.pop_back();
            }
        } else {
            password += ch;
            std::cout << '*' << std::flush;
        }
    }
    std::cout << std::endl;

    // Restore original terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    return password;
}

bool authenticateAdmin() {
    const std::string correctUser = "admin";
    const std::string correctPass = "1234";
    const int maxAttempts = 3;

    std::cout << "=====================================\n";
    std::cout << "       STUDENT MANAGEMENT PRO        \n";
    std::cout << "             ADMIN LOGIN             \n";
    std::cout << "=====================================\n";

    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        std::string username;
        std::cout << "Enter Username: ";
        if (!std::getline(std::cin, username)) {
            std::cin.clear();
            return false;
        }

        std::string password = readPassword("Enter Password: ");

        if (username == correctUser && password == correctPass) {
            std::cout << "\n>>> [SUCCESS] Access Granted. Logged in successfully!\n\n";
            return true;
        } else {
            std::cout << "\n>>> [ERROR] Incorrect Username or Password.\n";
            if (attempt < maxAttempts) {
                std::cout << "Remaining attempts: " << (maxAttempts - attempt) << "\n\n";
            }
        }
    }

    std::cout << "\n>>> [FATAL] Too many failed authentication attempts. Program terminating.\n";
    return false;
}

} // namespace Auth
