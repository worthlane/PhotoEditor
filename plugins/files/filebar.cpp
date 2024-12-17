#include <cassert>
#include <iostream>

#include "../plugins/files/filebar.hpp"
#include "../plugins/files/import.hpp"

#include "style/design.hpp"

#include "implementation/bar/bar_base.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {{0, 30}, {180, 61}};
static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {178, 29}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/white.jpg";
static const char* RELEASE_TEXTURE    = "assets/textures/white.jpg";
static const char* PRESS_TEXTURE      = "assets/textures/white.jpg";
static const char* NORMAL_TEXTURE     = "assets/textures/white.jpg";

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

    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, BACKGROUND_RECT, 1, {0, 0});
    back_sprite->setColor(psapi::sfm::Color(1, 1, 1));

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, BUTTON_RECT, 1, {0, 0});
    release_sprite->setColor(psapi::sfm::Color(2, 168, 167));

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, BUTTON_RECT, 1, {0, 0});
    hover_sprite->setColor(psapi::sfm::Color(2, 168, 167));

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, BUTTON_RECT, 1, {0, 0});
    press_sprite->setColor(psapi::sfm::Color(2, 168, 167));

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, BUTTON_RECT, 1, {0, 0});
    normal_sprite->setColor(psapi::sfm::Color(1, 1, 1));

    auto filebar = std::make_unique<FileBar>(kFileBarWindowId,
                                         psapi::vec2i(90, 30),
                                         psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));


    auto root = psapi::getRootWindow();
    auto file = root->getWindowById(kFileBarWindowId);

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> but_sprite = psapi::sfm::ISprite::create();
    but_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> file_sprite = psapi::sfm::ISprite::create();
    file_sprite.get()->setTextureRect(BUTTON_RECT);

    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto menu_pos = menu->getPos();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    std::string litvin = "assets/litvin.jpg";

    auto import = std::make_unique<ImportButton>(777, filebar.get(),
                                                    psapi::vec2i(1, 1),
                                                    psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                                    std::move(file_sprite),
                                                    canvas, litvin);

    auto filebutton = std::make_unique<FileButton>(psapi::kMenuFileId, menu,
                                                     psapi::vec2i(90, 0),
                                                     psapi::vec2u(90, 30),
                                                     std::move(but_sprite), std::move(filebar));



    filebutton->addMenuItem(std::move(import));
    menu->addWindow(std::move(filebutton));
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


// ********************* MENUBAR *********************/

FileBar::FileBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
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


void FileBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

    for (auto& button : buttons_)
    {
        finishButtonDraw(renderWindow, button.get());
        button.get()->draw(renderWindow);
    }

}
std::unique_ptr<psapi::IAction> FileBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<FileBarAction>(renderWindow, event, this);
}

// ******************** TOOLBAR ACTION ***********************

FileBarAction::FileBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FileBar* file_bar) :
AAction(render_window, event), file_bar_(file_bar)
{
}

bool FileBarAction::execute(const Key& key)
{
    if (!file_bar_->is_active_)
        return false;

    bool flag = false;

    auto controller = psapi::getActionController();

    for (size_t i = 0; i < file_bar_->buttons_.size(); ++i)
    {
        auto& button = file_bar_->buttons_[i];

        psapi::IBarButton::State prev_state = button.get()->getState();

        flag |= controller->execute(button.get()->createAction(render_window_, event_));

        psapi::IBarButton::State cur_state = button.get()->getState();

        if (cur_state == psapi::IBarButton::State::Released &&
            prev_state != psapi::IBarButton::State::Released)
        {
            for (size_t j = 0; j < file_bar_->buttons_.size(); ++j)
                if (j != i) file_bar_->buttons_[j].get()->setState(psapi::IBarButton::State::Normal);
        }
    }

    return flag;
}

bool FileBarAction::isUndoable(const Key& key)
{
    return false;
}

// ******** FILTERS BUTTON **********

FileButton::FileButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu) :
                 MenuSwitchButton(id, bar, pos, size, std::move(sprite), std::move(menu))
{
}

std::unique_ptr<psapi::IAction> FileButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
    {
        if (menu_->isActive())
            deactivateMenu();

        return std::make_unique<IdleAction>(renderWindow, event);
    }

    if (!menu_->isActive())
        activateMenu();

    return std::make_unique<FileAction>(renderWindow, event, this);
}

FileAction::FileAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FileButton* file_button) :
                                AAction(render_window, event), file_button_(file_button)
{}

bool FileAction::execute(const Key& key)
{
    auto controller = psapi::getActionController();
    controller->execute(file_button_->menu_->createAction(render_window_, event_));

    return true;
}

bool FileAction::isUndoable(const Key& key)
{
    return false;
}


