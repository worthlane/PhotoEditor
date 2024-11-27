#include <cassert>
#include <iostream>

#include "../plugins/filters/filters_bar.hpp"

#include "../plugins/bar_base/bar.hpp"
#include "../plugins/bar_base/bar_button.hpp"
#include "../plugins/style/design.hpp"

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {0, 0, 128, 400};

static const char* BACKGROUND_TEXTURE = "assets/textures/background_gray.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/hovered_icon.png";
static const char* RELEASE_TEXTURE    = "assets/textures/active_icon.png";
static const char* PRESS_TEXTURE      = "assets/textures/pressed_icon.png";
static const char* NORMAL_TEXTURE     = "assets/textures/normal_icon.png";

bool loadPlugin()
{
    std::cout << "filterbar loaded\n";

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
    make_styled_sprite(back_sprite.get(), back, BACKGROUND_RECT);

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, BUTTON_RECT, 1);

    auto bar = std::make_unique<ToolBar>(psapi::kOptionsBarWindowId,
                                         psapi::vec2i(1032, 20),
                                         psapi::vec2u(BACKGROUND_RECT.width, BACKGROUND_RECT.height),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));

    auto root = psapi::getRootWindow();

    root->addWindow(std::move(bar));
}

void unloadPlugin()
{
    delete back;
    delete release;
    delete hover;
    delete press;
    delete normal;
}
