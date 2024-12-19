#include <cassert>
#include <iostream>

#include "../plugins/menubar/help/help.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

bool onLoadPlugin()
{
    std::cout << "helpbar loaded\n";

    auto root = psapi::getRootWindow();
    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto helpbutton = static_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuHelpId));
    auto helpbar = helpbutton->getMenu();
}

void onUnloadPlugin()
{

}



