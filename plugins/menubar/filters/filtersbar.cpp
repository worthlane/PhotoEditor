#include <cassert>
#include <iostream>

#include "../plugins/menubar/filters/filtersbar.hpp"
#include "../plugins/menubar/filters/filters.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/bar/bar_base.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {{0, 30}, {270, 121}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/white.jpg";
static const char* RELEASE_TEXTURE    = "assets/textures/white.jpg";
static const char* PRESS_TEXTURE      = "assets/textures/white.jpg";
static const char* NORMAL_TEXTURE     = "assets/textures/white.jpg";

bool onLoadPlugin()
{
    std::cout << "filtersbar loaded\n";

    back = psapi::sfm::ITexture::create().release();
    back->loadFromFile(BACKGROUND_TEXTURE);

    hover = psapi::sfm::ITexture::create().release();
    hover->loadFromFile(HOVER_TEXTURE);

    press = psapi::sfm::ITexture::create().release();
    press->loadFromFile(PRESS_TEXTURE);

    normal = psapi::sfm::ITexture::create().release();
    normal->loadFromFile(NORMAL_TEXTURE);

    release = psapi::sfm::ITexture::create().release();
    release->loadFromFile(RELEASE_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, BACKGROUND_RECT, 1, {0, 0});
    back_sprite->setColor(BACK_COLOR);

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, SUBBUTTON_RECT, 1, {0, 0});
    release_sprite->setColor(RELEASE_COLOR);

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, SUBBUTTON_RECT, 1, {0, 0});
    hover_sprite->setColor(HOVER_COLOR);

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, SUBBUTTON_RECT, 1, {0, 0});
    press_sprite->setColor(PRESS_COLOR);

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, SUBBUTTON_RECT, 1, {0, 0});
    normal_sprite->setColor(NORMAL_COLOR);

    auto filtersbar = std::make_unique<SubMenuBar>(kFiltersBarWindowId,
                                         psapi::vec2i(270, 30),
                                         psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));


    auto root = psapi::getRootWindow();
    auto filters = root->getWindowById(kFiltersBarWindowId);

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> filters_sprite = psapi::sfm::ISprite::create();
    filters_sprite.get()->setTextureRect(SUBBUTTON_RECT);

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto menu_pos = menu->getPos();

    std::string neg_name = "Negative";
    auto negative = std::make_unique<NegativeButton>(kNegativeFilterButtonId, filtersbar.get(),
                                                    psapi::vec2i(1, 1),
                                                    psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                                    neg_name, psapi::sfm::Color(255, 255, 255),
                                                    -1, canvas);

    std::string barel_name = "Barelief";
    auto barel = std::make_unique<BareliefButton>(kBareliefFilterButtonId, filtersbar.get(),
                                                  psapi::vec2i(1, 2 + SUBBUTTON_RECT.size.y),
                                                  psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                                  barel_name, psapi::sfm::Color(255, 255, 255),
                                                  -1, canvas);

    std::string blur_name = "Blur";
    auto blur = std::make_unique<BlurButton>(kBlurFilterButtonId, filtersbar.get(),
                                             psapi::vec2i(1, 3 + 2 * SUBBUTTON_RECT.size.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               blur_name, psapi::sfm::Color(255, 255, 255),
                                               canvas);

    std::string contrast_name = "Contrast";
    auto contrast = std::make_unique<ContrastButton>(kContrastFilterButtonId, filtersbar.get(),
                                               psapi::vec2i(1, 4 + 3 * SUBBUTTON_RECT.size.y),
                                               psapi::vec2u(SUBBUTTON_RECT.size.x, SUBBUTTON_RECT.size.y),
                                               contrast_name, psapi::sfm::Color(255, 255, 255),
                                               canvas);
    std::string filter_name = "Filter";
    auto filterbutton = std::make_unique<CallSubMenuButton>(psapi::kMenuFilterId, menu,
                                                        psapi::vec2i(270, 0),
                                                        psapi::vec2u(90, 30),
                                                        filter_name, psapi::sfm::Color(255, 255, 255),
                                                        std::move(filtersbar));

    filterbutton->addMenuItem(std::move(negative));
    filterbutton->addMenuItem(std::move(barel));
    filterbutton->addMenuItem(std::move(blur));
    filterbutton->addMenuItem(std::move(contrast));
    menu->addWindow(std::move(filterbutton));
}

void onUnloadPlugin()
{
    delete back;
    delete release;
    delete hover;
    delete press;
    delete normal;

    delete btn;
}



