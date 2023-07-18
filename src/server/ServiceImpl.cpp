#include "ServiceImpl.hpp"

struct Performer
{
    ServerContext* context;
    grpc::ServerWriter<Line>* writer;
};

ServiceImpl::ServiceImpl( const std::vector<ScriptLine>& script ) : m_script(script)
{
    for ( const auto&[role,line] : m_script )
        m_registeredActors[role] = {};
    
    std::cout << "Waiting for actors for following roles: ";
    std::string roles;
    std::string delimeter = ", ";
    for ( auto&[role,performer] : m_registeredActors )
        roles += role + delimeter;
    
    if ( !roles.empty() )
        roles = roles.substr( 0, roles.size() - delimeter.size() );
    std::cout << roles << std::endl;
}

ServiceImpl::~ServiceImpl()
{

}

Status ServiceImpl::Attend( ServerContext* context, const Role* request, ::grpc::ServerWriter< Line>* writer )
{
    std::unique_lock lock(m_mutex);
    auto it = m_registeredActors.find(request->role());
    if ( it == m_registeredActors.end() )
        return { grpc::StatusCode::INVALID_ARGUMENT, std::string("Sorry, there's no such a role: ") + request->role() };

    if ( it->second )
        return { grpc::StatusCode::ALREADY_EXISTS, std::string("Sorry, the role: ") + request->role() + " is busy" };
    
    std::cout << request->role() << " joined" << std::endl;
    it->second.reset( new Performer{ context, writer } );

    std::vector<std::string> waiting;
    for ( auto&[role,performer] : m_registeredActors )
        if (!performer)
            waiting.push_back(role);

    m_cv.wait( lock, [this]
    {
        for ( auto&[role,performer] : m_registeredActors )
            if (!performer)
                return false;
        return true;
    });

    { // Play scene
        Line response;
        for ( auto&[role,line] : m_script )
        {
            auto& performer = m_registeredActors[role];
            response.set_line(line);
            if ( !performer->writer->Write(response) )
                std::cout << "Failed to send line: " << line << " to " << role << std::endl;
            std::chrono::seconds duration(1);
            std::this_thread::sleep_for( duration );
        }
        m_registeredActors.erase(request->role());
        m_script.clear();
    }

    m_cv.notify_one();

    if (m_registeredActors.empty())
    {
        shutdown.flag = true;
        shutdown.cv.notify_one();
    }

    return Status::OK;
}