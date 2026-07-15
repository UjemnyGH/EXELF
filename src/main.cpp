#include <iostream>
#include "elf_format.h"
#include <string>
#include <fstream>
#include <vector>

void Pause(const std::string& text) {
    std::string input;
    std::cout << text << "\n";
    std::getline(std::cin, input);
}

int main() {
    std::string magicletter = "MZ";
    std::string dosmode = "This program cannot be run in DOS mode.\r\n$";
    
    std::string name;
    std::cout << "File name: ";
    std::getline(std::cin, name);
    
    // Open file
    std::ifstream file(name, std::ios::binary);
    if (!file) {
        std::cout << "ERROR\n";

        // Could remove if you want.
        Pause("EXIT CODE: 1");
        return 1;
    }

    // Read the file contents into a buffer
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), 
                              std::istreambuf_iterator<char>());
    file.close();

    // Check if the file is large enough and has the ELF magic number
    if (buffer.size() >= 4 && 
        buffer[0] == 0x7F && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
        
        std::cout << "ELF header detected. Modifying stub...\n";

        // Check if file failed
        std::ofstream outfile(name, std::ios::binary);
        if (!outfile) {
            std::cout << "ERROR: Could not write to file.\n";
            Pause("EXIT CODE: 1");
            return 1;
        }

        // Write the DOS and MZ header
        outfile.write(magicletter.c_str(), magicletter.size());
        outfile.write(dosmode.c_str(), dosmode.size());
        
        // Write the data
        outfile.write(buffer.data(), buffer.size());
        outfile.close();
    } 
    else 
    {
        std::cout << "Not a valid ELF file or too small\n";
    }

    Pause("EXIT CODE: 0 (SUCCESS)");
    return 0;
}
