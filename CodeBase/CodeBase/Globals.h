#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <windows.h>
#include <stdio.h>

#define LOG(format,...) log(__FILE__, __LINE__, format, __VA_ARGS__);
void log(const char file[], int line, const char* format, ...);


#define CODEBASE_VERSION 0.1f

#define uint unsigned int


#endif // !__GLOBALS_H__
