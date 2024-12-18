#ifndef EDITBAR_PLUGIN_HPP
#define EDITBAR_PLUGIN_HPP

#include "api/api_photoshop.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::wid_t kEditBarWindowId = 269;

#endif // EDITBAR_PLUGIN_HPP
