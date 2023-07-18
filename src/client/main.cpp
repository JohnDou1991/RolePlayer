#include <iostream>
#include <memory>

#include <grpcpp/grpcpp.h>

#include "HelloThere.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using hellothere::Play;
using hellothere::Line;
using hellothere::Role;

class Client
{
public:
    Client( std::shared_ptr<Channel> channel ) : stub_( Play::NewStub(channel) )
    {

    }

    void PlayRole( const std::string& role )
    {
        Role request;
        request.set_role(role);
        ClientContext m_context;
        auto reader = stub_->Attend( &m_context, request );
        Line response;
        while ( reader->Read(&response) )
            std::cout << '[' << role << ']' << ' ' << response.line() << std::endl;
    }

private:
    std::unique_ptr<Play::Stub> stub_;
};

int main( int argc, char** argv )
{
    if ( argc != 3 )
    {
        std::cerr << "Expected input: [Role] [IP]" << std::endl;
        return 1;
    }
    const std::string role = argv[1];
    const std::string ip = argv[2];
    Client client( grpc::CreateChannel( ip, grpc::InsecureChannelCredentials() ));
    client.PlayRole( role );
    return 0;
}