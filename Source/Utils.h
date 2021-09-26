#pragma once

#include <vector>
#include <string>

#ifndef _WINDEF_
#define WIN32_HANDLE(name) struct name##__; typedef struct name##__ *name

WIN32_HANDLE(HWND);
WIN32_HANDLE(HINSTANCE);

typedef HINSTANCE HMODULE;
#endif

struct FModuleInfo
{
    uint64_t Start = 0x0;
    uint64_t End = 0x0;
};

class Utils
{
public:
    static std::string GetModulePath(HMODULE Module);
    static void PrintModules();
    static std::vector<HWND> GetWindows();
    static unsigned char* FindPattern(unsigned char* Start, unsigned char* End, const char* Pattern);
    static FModuleInfo GetModuleInfo(HMODULE Module);
    static std::string GetProcessExeModule();
protected:
    static bool UtilsEnumWindows(HWND Window, int64_t Param);
};