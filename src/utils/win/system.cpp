#include "utils/system.hpp"

#include <windows.h>
#include <filesystem>

namespace utils::system
{
    std::filesystem::path GetCurrentExecutablePath()
    {
        const DWORD MAX_LENGTH = 256;
        char buffer[MAX_LENGTH];
        DWORD result = GetModuleFileName(NULL, buffer, MAX_LENGTH);
        return buffer;
    }
}