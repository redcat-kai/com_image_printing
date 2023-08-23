#include <iostream>
#include <vector>
#include <fstream>
#include "COM_Port.h"

template<typename T>
void double_array(std::vector<T>& vec) {
    int old_count = vec.size();
    vec.resize(2*old_count);
    std::copy_n(vec.begin(), old_count, vec.begin() + old_count);
}

void skip_img_header(std::ifstream& is) {
    for (int i=0; i<3; i++) {
        for (char ch; is.get(ch) && ch != '\n';);
    }
}

int main() {
    COM_Port printer("COM2");

    uint8_t img_type = 0;

    std::vector<uint8_t> arr;
    switch (img_type) {
        case 0: 
                  //GS   v     0     \x00 e     \x00' \x00  \x04' // man.pmb
            arr = {0x1D, 0x76, 0x30, 0x0, 0x65, 0x00, 0x00, 0x04}; 
            break;
        case 1: 
                  //GS   v     0     \x00 E     \x00' o     \x01' // tulips.pmb
            arr = {0x1D, 0x76, 0x30, 0x0, 0x45, 0x00, 0x6F, 0x01}; 
            break;
    }

    // Read image in
    std::ifstream fs("man.pbm", std::ios::binary);

    // Skip first 3 lines (header)
    skip_img_header(fs);

    // Load image into byte array
    std::vector<uint8_t> img(std::istreambuf_iterator<char>(fs), {});
    arr.insert(arr.end(), img.begin(), img.end());

    std::cout << "reached" << std::endl;

    arr.insert(arr.end(), {0xA, 0x1B, 0x64, 0x40}); // Print empty lines
    arr.insert(arr.end(), {0x1b, 0x69, 0x0}); // Cut line, null term 

    // Write data to printer
    if (!printer.write(arr.data(), arr.size() -1)) {
        std::cout << "failed to write" << std::endl;
    }

    std::cout << "done" << std::endl;
}
