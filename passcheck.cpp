#include <iostream>
#include <crypt.h>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::string> string_split(std::string& input, char delim){
   std::vector<std::string> split;
   std::string part;
   std::istringstream tokenStream(input);
   while (std::getline(tokenStream, part, delim))
   {
      split.push_back(part);
   }
   return split;
}

int compare_with_hash(char* username, char* hash){
    // Read shadow file
    std::ifstream shadow_file;
    std::string line, salt;
    std::vector<std::string> split, subsplit;
    shadow_file.open("/etc/shadow", std::ifstream::in);
    while (std::getline(shadow_file, line)){
        split = string_split(line, ':');
        if (split[0] == username){
            subsplit = string_split(split[1], '$');
            salt = "$" + subsplit[0] + "$" + subsplit[1];
            std::cout << subsplit[0];

        }
    }
    shadow_file.close();
}
int main(){
    char username[64];
    char password[64];
    char salt[] = "$6$nPBmtIcb";
    // Fetch username from stdin
    std::cout << "Enter username:" << std::endl;
    std::cin >> username;
    // Fetch password from stdin
    std::cout << "Enter password:" << std::endl;
    std::cin >> password;
    char* encrypted_password = crypt(password, salt);
    std::cout << encrypted_password << std::endl;
    compare_with_hash(username, encrypted_password);
    return 0;
}