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

static const psapi::wid_t kFileBarWindowId = 267;

static const psapi::wid_t kFileImportBarWindowId = 777;
static const psapi::wid_t kFileExportBarWindowId = 778;

static const psapi::wid_t kFileImportMenuId = 780;
static const psapi::wid_t kFileExportMenuId = 781;

#endif // FILTERSBAR_PLUGIN_HPP
