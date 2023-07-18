#pragma once

#include <vector>

#include "ScriptLine.hpp"

namespace std::filesystem
{
    class path;
}
const std::vector<ScriptLine> ReadScript( const std::filesystem::path& );