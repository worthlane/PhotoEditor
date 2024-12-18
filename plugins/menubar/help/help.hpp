#ifndef HELPBAR_PLUGIN_HPP
#define HELPBAR_PLUGIN_HPP

#include "api/api_photoshop.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::wid_t kHelpBarWindowId = 268;

#endif // HELPBAR_PLUGIN_HPP
