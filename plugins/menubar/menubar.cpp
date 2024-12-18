#include <cassert>
#include <iostream>
#include <string>

#include "../plugins/menubar/menubar.hpp"
#include "../plugins/menubar/submenubar.hpp"

#include "style/design.hpp"

#include "implementation/bar/bar_base.hpp"

static psapi::sfm::ITexture* back    = psapi::sfm::ITexture::create().release();
static psapi::sfm::ITexture* release = psapi::sfm::ITexture::create().release();
static psapi::sfm::ITexture* hover   = psapi::sfm::ITexture::create().release();
static psapi::sfm::ITexture* press   = psapi::sfm::ITexture::create().release();
static psapi::sfm::ITexture* normal  = psapi::sfm::ITexture::create().release();

static const psapi::sfm::IntRect BACKGROUND_RECT = {{0, 0}, {1200, 30}};
static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {90, 30}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/white.jpg";
static const char* RELEASE_TEXTURE    = "assets/textures/white.jpg";
static const char* PRESS_TEXTURE      = "assets/textures/white.jpg";
static const char* NORMAL_TEXTURE     = "assets/textures/white.jpg";

void create_submenu(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size, psapi::IBar* menu, std::string name);

bool onLoadPlugin()
{
    std::cout << "menubar loaded\n";

    back->loadFromFile(BACKGROUND_TEXTURE);
    hover->loadFromFile(HOVER_TEXTURE);
    press->loadFromFile(PRESS_TEXTURE);
    normal->loadFromFile(NORMAL_TEXTURE);
    release->loadFromFile(RELEASE_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, BACKGROUND_RECT, 1, {0, 0});
    back_sprite->setColor(psapi::sfm::Color(31, 34, 37));

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, BUTTON_RECT, 1, {0, 0});
    release_sprite->setColor(psapi::sfm::Color(1, 1, 1));

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, BUTTON_RECT, 1, {0, 0});
    hover_sprite->setColor(psapi::sfm::Color(1, 1, 1));

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, BUTTON_RECT, 1, {0, 0});
    press_sprite->setColor(psapi::sfm::Color(1, 1, 1));

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, BUTTON_RECT, 1, {0, 0});
    normal_sprite->setColor(psapi::sfm::Color(31, 34, 37));

    auto bar = std::make_unique<MenuBar>(psapi::kMenuBarWindowId,
                                         psapi::vec2i(0, 0),
                                         psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));

    create_submenu(psapi::kMenuFileId, psapi::vec2i(0, 0),
                   psapi::vec2u(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y +  2 * GAP.y),
                   bar.get(), "File");
    create_submenu(psapi::kMenuToolsId, psapi::vec2i(BUTTON_RECT.size.x, 0),
                   psapi::vec2u(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y + 2 * GAP.y),
                   bar.get(), "Edit");
    create_submenu(psapi::kMenuLayerId, psapi::vec2i(2 * BUTTON_RECT.size.x, 0),
                   psapi::vec2u(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y + 2 * GAP.y),
                   bar.get(), "Layer");
    create_submenu(psapi::kMenuFilterId, psapi::vec2i(3 * BUTTON_RECT.size.x, 0),
                   psapi::vec2u(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y + 2 * GAP.y),
                   bar.get(), "Filter");
    create_submenu(psapi::kMenuHelpId, psapi::vec2i(4 * BUTTON_RECT.size.x, 0),
                   psapi::vec2u(SUBBUTTON_RECT.size.x + GAP.x, SUBBUTTON_RECT.size.y + 2 * GAP.y),
                   bar.get(), "Help");

    auto root = psapi::getRootWindow();

    root->addWindow(std::move(bar));
}

void onUnloadPlugin()
{
    delete back;
    delete release;
    delete hover;
    delete press;
    delete normal;
}


// ********************* MENUBAR *********************/

MenuBar::MenuBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    ABar(id, pos, size, std::move(background),
                    std::move(normal),
                    std::move(hovered),
                    std::move(pressed),
                    std::move(released))
{
}


void MenuBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

    for (auto& button : buttons_)
    {
        finishButtonDraw(renderWindow, button.get());
        button.get()->draw(renderWindow);
    }

}
std::unique_ptr<psapi::IAction> MenuBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<MenuBarAction>(renderWindow, event, this);
}

// ******************** TOOLBAR ACTION ***********************

MenuBarAction::MenuBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, MenuBar* menu_bar) :
AAction(render_window, event), menu_bar_(menu_bar)
{
}

bool MenuBarAction::execute(const Key& key)
{
    if (!menu_bar_->is_active_)
        return false;

    bool flag = false;

    auto controller = psapi::getActionController();

    for (size_t i = 0; i < menu_bar_->buttons_.size(); ++i)
    {
        auto& button = menu_bar_->buttons_[i];

        psapi::IBarButton::State prev_state = button.get()->getState();

        flag |= controller->execute(button.get()->createAction(render_window_, event_));

        psapi::IBarButton::State cur_state = button.get()->getState();

        if (cur_state == psapi::IBarButton::State::Released &&
            prev_state != psapi::IBarButton::State::Released)
        {
            for (size_t j = 0; j < menu_bar_->buttons_.size(); ++j)
                if (j != i) menu_bar_->buttons_[j].get()->setState(psapi::IBarButton::State::Normal);
        }
    }

    return flag;
}

bool MenuBarAction::isUndoable(const Key& key)
{
    return false;
}

void create_submenu(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size, psapi::IBar* menu, std::string name)
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

    auto bar = std::make_unique<SubMenuBar>(id + 50,
                                            pos + psapi::sfm::vec2i(0, BUTTON_RECT.size.y),
                                            size,
                                            std::move(back_sprite),
                                            std::move(normal_sprite),
                                            std::move(hover_sprite),
                                            std::move(press_sprite),
                                            std::move(release_sprite));

    auto call_bar = std::make_unique<CallSubMenuButton>(id, menu,
                                                        pos,
                                                        BUTTON_RECT.size,
                                                        name, psapi::sfm::Color(255, 255, 255),
                                                        std::move(bar));

    menu->addWindow(std::move(call_bar));
}


