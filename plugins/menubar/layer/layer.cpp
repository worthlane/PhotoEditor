#include <cassert>
#include <iostream>

#include "../plugins/menubar/layer/layer.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

bool onLoadPlugin()
{
    std::cout << "layerbar loaded\n";

    auto root = psapi::getRootWindow();
    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto layerbutton = static_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuLayerId));
    auto layerbar = layerbutton->getMenu();
}

void onUnloadPlugin()
{
}



