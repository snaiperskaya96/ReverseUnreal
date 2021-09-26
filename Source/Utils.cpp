#include "Utils.h"

#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <wchar.h>

std::string Utils::GetModulePath(HMODULE Module)
{
    char Path[MAX_PATH];
    GetModuleFileNameA(Module, Path, sizeof(Path));

    return Path;
}

void Utils::PrintModules()
{
    const DWORD ProcId = GetCurrentProcessId();
    const HANDLE CurrProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, ProcId);
    HMODULE Modules[1024] = {nullptr};
    DWORD ModulesNumBytes = 0;

    if (!CurrProcessHandle)
    {
        std::cerr << "Cannot obtain current process handle." << std::endl;
        return;
    }

    if (EnumProcessModules(CurrProcessHandle, Modules, sizeof(Modules), &ModulesNumBytes))
    {
        const int NumModules = (ModulesNumBytes / sizeof(HMODULE));
        std::cout << "Found " << NumModules << " loaded modules." << std::endl;
        for (int Index = 0; Index < NumModules; Index++)
        {
            TCHAR ModuleName[MAX_PATH] = {0x0};

            if (GetModuleFileNameEx(CurrProcessHandle, Modules[Index], ModuleName, sizeof(ModuleName) / sizeof(TCHAR)))
            {
                std::wcout << "  - " << ModuleName << std::endl;
            }
        }
    }
}

std::vector<HWND> Utils::GetWindows()
{
    std::vector<HWND> Windows;

    EnumWindows(reinterpret_cast<WNDENUMPROC>(&Utils::UtilsEnumWindows), reinterpret_cast<int64_t>(&Windows));

    return Windows;
}

bool Utils::UtilsEnumWindows(HWND Window, int64_t Param)
{
    std::vector<HWND>* Windows = reinterpret_cast<std::vector<HWND>*>(Param);
    const DWORD CurrentProc = GetCurrentProcessId();

    DWORD OwningProcessId = 0;

    if (Window)
    {
        GetWindowThreadProcessId(Window, &OwningProcessId);

        if (OwningProcessId == CurrentProc)
        {
            Windows->push_back(Window);
        }
    }
    return true;
}

FModuleInfo Utils::GetModuleInfo(HMODULE Module)
{
	FModuleInfo Out;
	MODULEINFO ModuleInfo = {};

	if (GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo)))
	{
		Out.Start = (uint64_t)ModuleInfo.lpBaseOfDll;
		Out.End = (uint64_t)ModuleInfo.lpBaseOfDll + ModuleInfo.SizeOfImage;
	}

	return Out;
}

std::string Utils::GetProcessExeModule()
{
	unsigned long Length = MAX_PATH;
    char Path[MAX_PATH] = {0x0};
    HMODULE Module = nullptr;
    QueryFullProcessImageNameA(GetCurrentProcess(), 0, Path, &Length);

    return std::string(Path, Length);
}

#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))
#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))

unsigned char* Utils::FindPattern(unsigned char* Start, unsigned char* End, const char* Pattern)
{
		const unsigned char* pat = reinterpret_cast<const unsigned char*>(Pattern);
		PBYTE firstMatch = 0;
		for (PBYTE pCur = Start; pCur < End; ++pCur) {
			if (*(PBYTE)pat == (BYTE)'\?' || *pCur == getByte(pat)) {
				if (!firstMatch) {
					firstMatch = pCur;
				}
				pat += (*(PWORD)pat == (WORD)'\?\?' || *(PBYTE)pat != (BYTE)'\?') ? 2 : 1;
				if (!*pat) {
					return firstMatch;
				}
				pat++;
				if (!*pat) {
					return firstMatch;
				}
			} else if (firstMatch) {
				pCur = firstMatch;
				pat = reinterpret_cast<const unsigned char*>(Pattern);
				firstMatch = 0;
			}
		}
		return NULL;
}
