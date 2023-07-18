#pragma once

namespace std::filesystem
{
    class path;
}

namespace utils::system
{
    std::filesystem::path GetCurrentExecutablePath();
}