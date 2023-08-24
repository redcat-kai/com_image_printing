# Printing images on Epson ESCPOS receipt printers using C++

Reads .pbm images and prints them on thermal epson receipt printers

includes a wrapper for win32 com handling (see COM_Port.h)

## Usage
```
clang write_image.cxx -o write_image.exe
.\write_image.exe tulips.pbm
```
