#include <iostream>

#include "api/utils.hpp"

namespace psapi
{
    generalFunction getGeneralFunction(const std::string& name)
    {
        std::cout << "getGeneralFunction: " << name << std::endl;
        return nullptr;
    }

} // namespace psapi
