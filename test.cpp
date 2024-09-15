#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <string>  
#include <map>  
#include <vector>  
  dq3xdw4rc
int main() {  
    std::map<std::string, std::string> parameters;  
    std::map<std::string, std::vector<std::string>> commands;  
    std::string currentBlock = "";  
    std::string line;  
  
    std::ifstream file("config.ini");  
    if (!file.is_open()) {  
        std::cerr << "Unable to open file" << std::endl;  
        return 1;  
    }  
  
    while (getline(file, line)) {  
        // Trim whitespace if needed  
        line.erase(0, line.find_first_not_of(" \t\n"));  
        line.erase(line.find_last_not_of(" \t\n") + 1);  
  
        if (line.empty()) continue; // Skip empty lines  
  
        if (line.find("parm begin") != std::string::npos) {  
            currentBlock = "parm";  
        } else if (line.find("parm end") != std::string::npos) {  
            currentBlock = "";  
        } else if (line.find("git begin") != std::string::npos) {  
            currentBlock = "git";  
        } else if (line.find("git end") != std::string::npos) {  
            currentBlock = "";  
        } else if (line.find("code begin") != std::string::npos) {  
            currentBlock = "code";  
        } else if (line.find("code end") != std::string::npos) {  
            currentBlock = "";  
        }  
  
        if (!currentBlock.empty()) {  
            if (currentBlock == "parm") {  
                std::istringstream iss(line);  
                std::string key, value;  
                if (std::getline(iss, key, ':') && std::getline(iss, value)) {  
                    parameters[key] = value;  
                }  
            } else if ((currentBlock == "git" || currentBlock == "code")&&(line!="code begin"&&line!="git begin")) {  
                commands[currentBlock].push_back(line);  
            }  
        }  
    }  
  
    file.close();  
  
    // Output parameters  
    std::cout << "Parameters:" << std::endl;  
    for (const auto& pair : parameters) {  
        std::cout << pair.first << ": " << pair.second << std::endl;  
    }  
  
    // Output commands  
    std::cout << "Commands:" << std::endl;  
    for (const auto& cmd_pair : commands) {  
        std::cout << cmd_pair.first << " commands:" << std::endl;  
        for (const auto& cmd : cmd_pair.second) {  
            std::cout << cmd << std::endl;  
        }  
    }  
  xs64cb
    return 0;  
}