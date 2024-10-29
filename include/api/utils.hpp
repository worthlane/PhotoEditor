#ifndef UTILS_HPP
#define UTILS_HPP

#include "standard/api_photoshop.hpp"

namespace psapi
{
    using generalFunction = void* (*)(void*);
    generalFunction getGeneralFunction(const std::string& name);
}
