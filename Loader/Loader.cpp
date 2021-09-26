#include <Windows.h>
#include <thread>
#include <filesystem>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

std::filesystem::file_time_type GetLastWriteTime(const std::filesystem::path& Path)
{
    return std::filesystem::last_write_time(Path);
}

std::filesystem::path CreateTemporaryCopy(const std::filesystem::path& Path)
{
    std::string TmpName = std::string(std::tmpnam(nullptr)) + ".dll";
    std::filesystem::path Out = std::filesystem::temp_directory_path().append(TmpName);
    std::filesystem::copy_file(Path, Out);

    return Out;
}

void LoaderThread()
{
    HINSTANCE LoadedDll = nullptr;
    std::filesystem::path DllPath = STR(BINARY_DIR) "/Source/Debug/ReverseUnreal.dll";
    std::filesystem::file_time_type LastWriteTime = GetLastWriteTime(DllPath);

    while (true)
    {
        std::filesystem::file_time_type TmpWriteTime = GetLastWriteTime(DllPath);

        if (!LoadedDll || TmpWriteTime != LastWriteTime)
        {
            if (LoadedDll)
            {
                FreeLibrary(LoadedDll);
            }
            LoadedDll = LoadLibraryW(CreateTemporaryCopy(DllPath).c_str());
            LastWriteTime = TmpWriteTime;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
 //   DisableThreadLibraryCalls(hinstDLL);

    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoaderThread, NULL, 0, NULL);
            break;
        }
        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
