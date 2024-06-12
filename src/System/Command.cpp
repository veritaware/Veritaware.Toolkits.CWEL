#include "System/Command.hpp"

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string vwr::System::Command::Execute(const char* cmd)
{
    std::array<char, 4096> buffer = {0};
    std::string result;

#if _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}
