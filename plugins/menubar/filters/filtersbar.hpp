#ifndef FILTERSBAR_PLUGIN_HPP
#define FILTERSBAR_PLUGIN_HPP

#include "api/api_photoshop.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::wid_t kFiltersBarWindowId = 266;

#endif // FILTERSBAR_PLUGIN_HPP
