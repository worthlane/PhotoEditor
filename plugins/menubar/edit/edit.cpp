#include <cassert>
#include <iostream>

#include "../plugins/menubar/edit/edit.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {{0, 30}, {270, 61}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/white.jpg";
static const char* RELEASE_TEXTURE    = "assets/textures/white.jpg";
static const char* PRESS_TEXTURE      = "assets/textures/white.jpg";
static const char* NORMAL_TEXTURE     = "assets/textures/white.jpg";

bool onLoadPlugin()
{
    std::cout << "editbar loaded\n";

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

    auto editbar = std::make_unique<SubMenuBar>(kEditBarWindowId,
                                         psapi::vec2i(90, 30),
                                         psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));

    auto root = psapi::getRootWindow();
    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));

    std::string edit_name = "Edit";
    auto editbutton = std::make_unique<CallSubMenuButton>(psapi::kMenuToolsId, menu,
                                                        psapi::vec2i(90, 0),
                                                        psapi::vec2u(90, 30),
                                                        edit_name, psapi::sfm::Color(255, 255, 255),
                                                        std::move(editbar));

    menu->addWindow(std::move(editbutton));
}

void onUnloadPlugin()
{
    delete back;
    delete release;
    delete hover;
    delete press;
    delete normal;
}



