#include <iostream>
#include "elf_format.h"
#include <string>
#include <fstream>


void Pause(std::string text)
{
  std::string input // Just so it does not return
  std::cout << text << '\n';
  std::getline(std::cin, input);
  return;
}

int main()
{
  std::string name;

  std::cout << "File name: ";
  std::getline(std::cin, name);

  std::ofstream file(name, std::ios::binary);

  if (!file)
  {
    std::cout << "ERROR";

    // Could remove if you want.
    Pause("EXIT CODE: 1");
    return 1;
  }

  // Main logic (You would write this here)

  file.close();
  Pause("EXIT CODE: 0 (SUCCESS)");
  return 0;
}
