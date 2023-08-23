#include <iostream>
#include <vector>
#include "COM_Port.h"

int main() {
    COM_Port printer("COM2");

    std::vector<uint8_t> arr = {0x1B, 0x21, 0x20}; // Emphasis on
    std::string command = "Testing string";
    
    for (char i : command) {
        arr.push_back(int(i));
    }

    arr.insert(arr.end(), {0x1B, 0x21, 0x0}); // Emphasis off
    arr.insert(arr.end(), {0x1B, 0x64, 0x20}); // Print empty lines
    arr.insert(arr.end(), {0x1b, 0x69, 0x0}); // Cut line, null term 

    if (!printer.write(arr.data(), arr.size() -1)) {
        std::cout << "failed to write" << std::endl;
    }

    std::cout << "done" << std::endl;
}
