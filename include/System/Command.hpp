#ifndef VWRTK_CWEL_SYSTEM_COMMAND_HPP
#define VWRTK_CWEL_SYSTEM_COMMAND_HPP

#include <string>

namespace vwr::System::Command
{
    /// <summary>
    /// Executes a command and returns the output.
    /// </summary>
    std::string Execute(const char* cmd);
}

#endif //VWRTK_CWEL_SYSTEM_COMMAND_HPP
