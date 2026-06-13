#ifndef AUTH_H
#define AUTH_H

#include <string>

namespace Auth {
    // Prompts for administrator credentials and authenticates.
    // Allows up to 3 attempts. Returns true if successful, false otherwise.
    bool authenticateAdmin();

    // Prompts and reads input masking characters with asterisk.
    std::string readPassword(const std::string& prompt);
}

#endif // AUTH_H
