#ifndef UTIL_H
#define UTIL_H

#include <windows.h>
#include <conio.h>
#include <iostream>

int sendread(HANDLE hSerial, uint8_t l);
void init(HANDLE& hSerial, DCB& dcbSerialParams, COMMTIMEOUTS& timeouts);

#endif // UTIL_H
