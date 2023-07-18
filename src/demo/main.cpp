#include <iostream>

#include "demo/process/win/Process.hpp"
#include "utils/system.hpp"

#include <filesystem>

int main( int argc, char** argv )
{
    std::filesystem::path path = utils::system::GetCurrentExecutablePath();

    std::string_view ip = "localhost:50551";
    std::vector<std::string_view> roles = { "\"Obi-Wan Kenobi\"", "\"General Grievous\"" };

    try
    {
        std::vector<std::unique_ptr<IProcess>> processes;
        
        std::initializer_list<std::string_view> server_args = { ip };
        processes.emplace_back( std::make_unique<Process>( path.replace_filename( "Server.exe" ), server_args ) );

        for ( const auto& role : roles )
        {
            std::initializer_list<std::string_view> client_args = { role, ip };
            processes.emplace_back( std::make_unique<Process>( path.replace_filename( "Client.exe" ), client_args ) );
        }
    }
    catch( std::string msg )
    {
        std::cerr << "[Exception]: " << msg << std::endl;
        return 1;
    }
    return 0;
}