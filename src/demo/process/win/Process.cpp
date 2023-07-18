#include "Process.hpp"

#include <iostream>
#include <string>
#include <filesystem>

std::string ToString( const std::filesystem::path& from )
{
    std::wstring path = from.c_str();
    return std::string( path.begin(), path.end() );
}

Process::Process( const std::filesystem::path& path, std::initializer_list<std::string_view> cmdArgs )
{
    const bool exists = std::filesystem::exists(path);
    if ( !exists )
        throw ToString(path) + " does not exists";

    ZeroMemory( &m_info.startup, sizeof(m_info.startup));
    m_info.startup.cb = sizeof(m_info.startup);
    ZeroMemory( &m_info.process, sizeof(m_info.process));

    std::string args;
    for ( auto cmd : cmdArgs )
        args = args + " " + cmd.data();

    const bool success = CreateProcess( ToString(path).c_str()
        , const_cast<char*>(args.c_str())
        , nullptr
        , nullptr
        , false
        , 0
        , nullptr
        , nullptr
        , &m_info.startup
        , &m_info.process
    );

    if ( !success )
        throw ToString(path) + " failed to start process. Error: " + std::to_string( GetLastError() );
}

Process::~Process()
{
    WaitForSingleObject( m_info.process.hProcess, INFINITE );

    CloseHandle( m_info.process.hProcess );
    CloseHandle( m_info.process.hThread );
}