#include <cassert>
#include <iostream>

#include "../plugins/menubar/filters/filtersbar.hpp"
#include "../plugins/menubar/filters/filters.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "implementation/bar/bar_base.hpp"

bool onLoadPlugin()
{
    std::cout << "filtersbar loaded\n";

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto filtersbutton = dynamic_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuFilterId));
    auto filtersbar = (filtersbutton)->getMenu();

    std::string neg_name = "Negative";
    auto negative = std::make_unique<NegativeButton>(kNegativeFilterButtonId, filtersbar,
                                                    psapi::vec2i(GAP.x / 2, GAP.y),
                                                    psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                                    neg_name, psapi::sfm::Color(255, 255, 255),
                                                    -1, canvas);

    std::string barel_name = "Barelief";
    auto barel = std::make_unique<BareliefButton>(kBareliefFilterButtonId, filtersbar,
                                                  psapi::vec2i(GAP.x / 2, 2 * GAP.y + SUBBUTTON_RECT.size.y),
                                                  psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                                  barel_name, psapi::sfm::Color(255, 255, 255),
                                                  -1, canvas);

    std::string blur_name = "Blur";
    auto blur = std::make_unique<BlurButton>(kBlurFilterButtonId, filtersbar,
                                             psapi::vec2i(GAP.x / 2, 3 * GAP.y + 2 * SUBBUTTON_RECT.size.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               blur_name, psapi::sfm::Color(255, 255, 255),
                                               canvas);

    std::string contrast_name = "Contrast";
    auto contrast = std::make_unique<ContrastButton>(kContrastFilterButtonId, filtersbar,
                                               psapi::vec2i(GAP.x / 2, 4 * GAP.y + 3 * SUBBUTTON_RECT.size.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               contrast_name, psapi::sfm::Color(255, 255, 255),
                                               canvas);

    expandMenu(filtersbar, negative.get());
    filtersbutton->addMenuItem(std::move(negative));

    expandMenu(filtersbar, barel.get());
    filtersbutton->addMenuItem(std::move(barel));

    expandMenu(filtersbar, blur.get());
    filtersbutton->addMenuItem(std::move(blur));

    expandMenu(filtersbar, contrast.get());
    filtersbutton->addMenuItem(std::move(contrast));
}

void onUnloadPlugin()
{
}



