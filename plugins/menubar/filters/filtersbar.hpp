#ifndef FILTERSBAR_PLUGIN_HPP
#define FILTERSBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::wid_t kFiltersBarWindowId = 266;

#endif // FILTERSBAR_PLUGIN_HPP
