#pragma once

#include "../IProcess.hpp"
#include <initializer_list>
#include <string>

// win
#include <windows.h>
#include <processthreadsapi.h>

namespace std::filesystem
{
    class path;
}

class Process : public IProcess
{
    struct {
        STARTUPINFO startup;
        PROCESS_INFORMATION process;
    } m_info;

public:
    Process( const std::filesystem::path&, std::initializer_list<std::string_view> cmdArgs );
    ~Process();
};