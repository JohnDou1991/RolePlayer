#pragma once

#include <condition_variable>
#include <mutex>

#include "ScriptLine.hpp"

#include "HelloThere.grpc.pb.h"

using grpc::ServerContext;
using grpc::Status;

using hellothere::Play;
using hellothere::Line;
using hellothere::Role;

struct Performer;

class ServiceImpl : public Play::Service
{
    virtual Status Attend( ServerContext*, const Role*, ::grpc::ServerWriter< Line>* ) override;

public:
    ServiceImpl( const std::vector<ScriptLine>& script );
    ~ServiceImpl();

    struct {
        bool flag = false;
        std::mutex mutex;
        std::condition_variable cv;
    } shutdown;

private:
    std::unordered_map<std::string, std::unique_ptr<Performer>> m_registeredActors;
    std::vector<ScriptLine> m_script;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};