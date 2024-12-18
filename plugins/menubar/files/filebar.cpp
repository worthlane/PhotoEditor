#include <cassert>
#include <iostream>

#include "../plugins/menubar/files/filebar.hpp"
#include "../plugins/menubar/files/import.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/bar/bar_base.hpp"

static psapi::sfm::ITexture* btn = nullptr;

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

static std::unique_ptr<SubMenuBar> create_submenu(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size);

bool onLoadPlugin()
{
    std::cout << "filebar loaded\n";

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

    auto filebar = create_submenu(kFileBarWindowId, psapi::vec2i(0, 30), psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y));

    auto importbar = create_submenu(kFileImportBarWindowId, psapi::vec2i(270, 30), psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y));


    auto root = psapi::getRootWindow();
    auto file = root->getWindowById(kFileBarWindowId);

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> but_sprite = psapi::sfm::ISprite::create();
    but_sprite.get()->setTextureRect(SUBBUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> file_sprite = psapi::sfm::ISprite::create();
    file_sprite.get()->setTextureRect(SUBBUTTON_RECT);

    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto menu_pos = menu->getPos();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    std::string litvin = "assets/litvin.jpg";
    auto import = std::make_unique<ImportButton>(kFileImportBarWindowId, importbar.get(),
                                                    psapi::vec2i(1, 1),
                                                    SUBBUTTON_RECT.size,
                                                    litvin, psapi::sfm::Color(255, 255, 255), canvas);

    std::string call_import_name = "Import";
    auto call_import_button = std::make_unique<CallSubMenuButton>(psapi::kMenuFileId, filebar.get(),
                                                     psapi::vec2i(1, 1),
                                                     SUBBUTTON_RECT.size,
                                                     call_import_name, psapi::sfm::Color(255, 255, 255),
                                                     std::move(importbar));

    std::string file_name = "File";
    auto file_button = std::make_unique<CallSubMenuButton>(psapi::kMenuFileId, menu,
                                                     psapi::vec2i(0, 0),
                                                     psapi::vec2u(90, 30),
                                                     file_name, psapi::sfm::Color(255, 255, 255),
                                                     std::move(filebar));

    call_import_button->addMenuItem(std::move(import));
    file_button->addMenuItem(std::move(call_import_button));
    menu->addWindow(std::move(file_button));
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

static std::unique_ptr<SubMenuBar> create_submenu(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size)
{
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

    auto filebar = std::make_unique<SubMenuBar>(id,
                                                pos,
                                                size,
                                                std::move(back_sprite),
                                                std::move(normal_sprite),
                                                std::move(hover_sprite),
                                                std::move(press_sprite),
                                                std::move(release_sprite));
    return filebar;
}


