#include <iostream>

#include "plugin.hpp"

extern "C"
{

bool loadPlugin()
{
    std::cout << "aue\n";

    return true;
}

void unloadPlugin()
{}

}
