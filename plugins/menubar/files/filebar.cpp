#include <cassert>
#include <iostream>

#include "../plugins/menubar/files/filebar.hpp"
#include "../plugins/menubar/files/import.hpp"
#include "../plugins/menubar/files/export.hpp"

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

static std::unique_ptr<SubMenuBar> create_subbar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size);

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

    auto root = psapi::getRootWindow();

    auto filebutton = dynamic_cast<psapi::IMenuButton*>(root->getWindowById(psapi::kMenuFileId));

    auto filebar = (filebutton)->getMenu();
    auto filebar_pos = filebar->getPos();
    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto importbar = create_subbar(kFileImportBarWindowId,
                                    filebar_pos + psapi::vec2i(SUBBUTTON_RECT.size.x + GAP.x, 0),
                                    psapi::vec2u(BACKGROUND_RECT.size.x, 31));
    auto exportbar = create_subbar(kFileExportBarWindowId,
                                    filebar_pos + psapi::vec2i(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y + GAP.y),
                                    psapi::vec2u(BACKGROUND_RECT.size.x, 31));

    std::string call_import_name = "Import";
    auto call_import_button = std::make_unique<CallSubMenuButton>(kFileImportMenuId, filebar,
                                                     psapi::vec2i(GAP.x / 2, GAP.y),
                                                     SUBBUTTON_RECT.size,
                                                     call_import_name, psapi::sfm::Color(255, 255, 255),
                                                     std::move(importbar), false);

    std::string call_export_name = "Export";
    auto call_export_button = std::make_unique<CallSubMenuButton>(kFileExportMenuId, filebar,
                                                     psapi::vec2i(GAP.x / 2, 2 * GAP.y + SUBBUTTON_RECT.size.y),
                                                     SUBBUTTON_RECT.size,
                                                     call_export_name, psapi::sfm::Color(255, 255, 255),
                                                     std::move(exportbar), false);

    createFileButtons<ImportButton>(call_import_button.get(), canvas);
    createFileButtons<ExportButton>(call_export_button.get(), canvas);

    /*std::string litvin = "assets/litvin.jpg";
    auto import = std::make_unique<ImportButton>(800, importbar.get(),
                                                    psapi::vec2i(GAP.x / 2, GAP.y),
                                                    SUBBUTTON_RECT.size,
                                                    litvin, psapi::sfm::Color(255, 255, 255), canvas);

    std::string aboba = "assets/aboba.png";
    auto export_b = std::make_unique<ExportButton>(801, exportbar.get(),
                                                    psapi::vec2i(GAP.x / 2, GAP.y),
                                                    SUBBUTTON_RECT.size,
                                                    aboba, psapi::sfm::Color(255, 255, 255), canvas);*/

    expandMenu(filebar, call_import_button.get());
    filebutton->addMenuItem(std::move(call_import_button));

    expandMenu(filebar, call_export_button.get());
    filebutton->addMenuItem(std::move(call_export_button));
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

static std::unique_ptr<SubMenuBar> create_subbar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size)
{
    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, {{0, 0}, {size.x, size.y}}, 1, {0, 0});
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


