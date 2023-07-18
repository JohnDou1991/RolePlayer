#include <iostream>
#include <filesystem>

#include "script_reader.hpp"
#include "utils/system.hpp"

#include <grpcpp/grpcpp.h>

#include "ServiceImpl.hpp"

void RunServer( const std::string& server_address )
{
    std::filesystem::path script = utils::system::GetCurrentExecutablePath().replace_filename( "Script.txt" );
    ServiceImpl service( ReadScript( script ) );

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    std::unique_lock lock(service.shutdown.mutex);
    service.shutdown.cv.wait( lock, [&service]
    {
        return service.shutdown.flag;
    });
    server->Shutdown();
}

int main( int argc, char** argv )
{
    if ( argc != 2 )
    {
        std::cerr << "Expected input: [IP]" << std::endl;
        return 1;
    }
    RunServer( argv[1] );
    return 0;
}