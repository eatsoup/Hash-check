#include <iostream>
#include <crypt.h>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::string> string_split(std::string &input, char delim)
{
    // Helper function for splitting a string by delimitter
    std::vector<std::string> split;
    std::string part;
    std::istringstream tokenStream(input);
    while (std::getline(tokenStream, part, delim))
    {
        split.push_back(part);
    }
    return split;
}

int compare_with_hash(char *username, char *password)
{
    // Read shadow file
    std::ifstream shadow_file;
    std::string line, salt;
    std::vector<std::string> split, subsplit;
    shadow_file.open("/etc/shadow", std::ifstream::in);
    while (std::getline(shadow_file, line))
    {
        split = string_split(line, ':');
        // Find username in shadow file
        if (split[0] == username)
        {
            // Extract salt and hash
            subsplit = string_split(split[1], '$');
            salt = "$" + subsplit[1] + "$" + subsplit[2];
            std::string hashed_password = salt + "$" + subsplit[3];
            char salt_char[salt.size() + 1];
            salt.copy(salt_char, salt.size() + 1);
            salt_char[salt.size()] = '\0';
            // Generate our own hash
            char *encrypted_password = crypt(password, salt_char);
            // Compare the hashes
            if (encrypted_password == hashed_password)
            {
                return true;
            }
        }
    }
    shadow_file.close();
    return false;
}
int main()
{
    char username[8];
    char password[128];
    // Fetch username from stdin
    std::cout << "Enter username:" << std::endl;
    std::cin >> username;
    // Fetch password from stdin
    std::cout << "Enter password:" << std::endl;
    std::cin >> password;
    // Test the password
    if (compare_with_hash(username, password))
    {
        std::cout << "PASSWORD MATCHES" << std::endl;
    }
    else
    {
        std::cout << "PASSWORD DOES NOT MATCH" << std::endl;
    }
    return 0;
}
