#include "script_reader.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

const std::vector<ScriptLine> ReadScript( const std::filesystem::path& script )
{
    std::vector<ScriptLine> result;

    if ( !std::filesystem::exists(script) )
    {
        std::cerr << "File not found: " << script << std::endl;
        return {};
    }

    std::ifstream infile( script.c_str() );
    std::string buffer;

    auto Parse = [&result]( const std::string& buffer )
    {
        const size_t pos = buffer.find(':');
        if ( std::string::npos == pos )
            std::cout << "Incorrect format: " << buffer << std::endl;
        else
            result.push_back( { buffer.substr( 0, pos ), buffer.substr( pos + 1 ) } );
    };

    while ( std::getline( infile, buffer ) )
        Parse( buffer );

    return result;
}