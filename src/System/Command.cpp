#include "System/Command.hpp"

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string vwr::System::Command::Execute(const char* cmd)
{
    static constexpr size_t BufferSize = 4096;
    std::array<char, BufferSize> buffer = {};
    std::string result;

#if _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> const pipe(_popen(cmd, "r"), _pclose); // NOLINT(*-env33-c)
#else
    std::unique_ptr<FILE, decltype(&pclose)> const pipe(popen(cmd, "r"), pclose); // NOLINT(*-env33-c)
#endif

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += std::string(buffer.data());
    }

    return result;
}
