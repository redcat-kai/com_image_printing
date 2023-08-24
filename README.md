# Image printing for epson thermal printers

Reads .pbm images and prints them on thermal epson receipt printers

includes a wrapper for win32 com handling (see com_wrapper.h)

## Usage
```
clang write_image.cxx -o write_image.exe
.\write_image.exe tulips.pbm
```
