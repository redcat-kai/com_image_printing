#include <iostream>
#include <vector>
#include <fstream>
#include "COM_Port.h"
#include <array>
#include <iterator>

struct Pbm_Header {
    std::string encoding;
    uint16_t x;
    uint16_t y;
};

// Split 2 byte decimal value into 2 little endian values
std::array<uint8_t, 2> to_endian(uint16_t inp_number) {
    std::array<uint8_t, 2> out_arr;
    out_arr[0] = inp_number & 0xFF; // Low byte
    out_arr[1] = (inp_number >> 8) & 0xFF; // High byte

    return out_arr;
}

Pbm_Header parse_pbm_header(std::ifstream& is) {
    auto skip = [&]{ for (char ch; is.get(ch) && ch != '\n';);};

    // Read encoding
    Pbm_Header img;
    is >> img.encoding;
    skip();

    // skip comments
    while (true) {
        char chr;
        if (is.peek() == '#') {
            skip();
        } else {
            break;
        }
    }

    // Get Pixel dimensions 
    is >> img.x >> img.y;
    skip();

    return img;
}

int main() {
    COM_Port printer("COM2");

    // Read image 
    std::ifstream fs("man.pbm", std::ios::binary);

    Pbm_Header header = parse_pbm_header(fs);

    if (header.encoding != "P4") {
        std::cout << "error, incorrect image format. Encode as raw pbm" 
                  << std::endl;
        return 0;
    }
                            //  GS    v     0     0
    std::vector<uint8_t> arr = {0x1D, 0x76, 0x30, 0x00};
    uint16_t width = (header.x + 7) / 8;

    // Calculate xL, xH and yL, yH
    std::array<uint8_t, 2> x_low_high = to_endian(width);
    std::array<uint8_t, 2> y_low_high = to_endian(header.y);

    arr.insert(arr.end(), std::begin(x_low_high), std::end(x_low_high));
    arr.insert(arr.end(), std::begin(y_low_high), std::end(y_low_high));

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
