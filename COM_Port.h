#ifndef COM_PORT_H
#define COM_PORT_H
#include <comutil.h>
#include <winbase.h>
#include <iostream>
#include <string>

// Wrapper around using win32 api to write to COM port
class COM_Port {
private:
    DCB dcb_serial_params = {0};
    COMMTIMEOUTS timeout = {0};

public:
    HANDLE h_serial;

    COM_Port(const char com_port[]) {
        h_serial = CreateFile(com_port, GENERIC_READ | GENERIC_WRITE, 0, 0,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if (h_serial == INVALID_HANDLE_VALUE) {
            if (GetLastError() == ERROR_FILE_NOT_FOUND) {
                std::cout << "com port not found" << std::endl;
            }
            std::cout << "some serial port error" << std::endl;
        }
        
        dcb_serial_params.DCBlength=sizeof(dcb_serial_params);

        if (!GetCommState(h_serial, &dcb_serial_params)) {
            std::cout << "Couldn't get COM state" << std::endl;
        }

        dcb_serial_params.BaudRate=CBR_19200;
        dcb_serial_params.ByteSize=8;
        dcb_serial_params.StopBits=ONESTOPBIT;
        dcb_serial_params.Parity=NOPARITY;

        if(!SetCommState(h_serial, &dcb_serial_params)) {
            std::cout << "couldn't set state" << std::endl;
        }

        timeout.ReadIntervalTimeout=60;
        timeout.ReadTotalTimeoutConstant=60;
        timeout.ReadTotalTimeoutMultiplier=15;
        timeout.WriteTotalTimeoutConstant=60;
        timeout.WriteTotalTimeoutMultiplier=8;

        if(!SetCommTimeouts(h_serial, &timeout)){
            std::cout << "couldn't set timeouts" << std::endl;
        }
    }
    
    bool write(
            _In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID data, 
            _In_ DWORD n
            ) 
    {
        DWORD dw_read = 0;

        if (!WriteFile(h_serial, data, n, &dw_read, NULL)) {
            std::cout << "failed to write to COM port, dw_read: " 
                      << dw_read << std::endl;
            return false;
        }
        return true;
    }

    // unimplemented
    bool read(const char data[], uint8_t n) {
        DWORD dw_read = 0;
        if (!WriteFile(h_serial, data, n, &dw_read, NULL)) {
            std::cout << "failed to write to COM port" << std::endl;
            return false;
        }
        return true;
    }
};

#endif //COM_PORT_H
