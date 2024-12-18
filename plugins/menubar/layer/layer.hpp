#ifndef LAYERBAR_PLUGIN_HPP
#define LAYERBAR_PLUGIN_HPP

#include "api/api_photoshop.hpp"

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

static const psapi::wid_t kLayerBarWindowId = 267;

#endif // LAYERBAR_PLUGIN_HPP
