#include <cassert>
#include <iostream>

#include "../plugins/menubar/edit/edit.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

bool onLoadPlugin()
{
    std::cout << "editbar loaded\n";

    auto root = psapi::getRootWindow();
    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto editbutton = static_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuToolsId));
    auto editbar = editbutton->getMenu();
}

void onUnloadPlugin()
{
}



