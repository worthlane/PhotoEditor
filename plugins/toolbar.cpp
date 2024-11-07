#include <cassert>
#include <iostream>

#include "../plugins/toolbar.hpp"
#include "api/root_window.hpp"

static psapi::sfm::Texture back;
static psapi::sfm::Texture release;
static psapi::sfm::Texture hover;
static psapi::sfm::Texture press;
static psapi::sfm::Texture normal;

static const psapi::sfm::IntRect BACKGROUND_RECT = {900, 0, 128, 800};
static const psapi::sfm::IntRect BUTTON_RECT     = {0, 0, 90, 90};

static const char* BACKGROUND_TEXTURE = "assets/textures/gradient2.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/hovered_icon.png";
static const char* RELEASE_TEXTURE    = "assets/textures/active_icon.png";
static const char* PRESS_TEXTURE      = "assets/textures/pressed_icon.png";
static const char* NORMAL_TEXTURE     = "assets/textures/normal_icon.png";


bool loadPlugin()
{
    std::cout << "toolbar loaded\n";

    back.loadFromFile(BACKGROUND_TEXTURE);
    hover.loadFromFile(HOVER_TEXTURE);
    press.loadFromFile(PRESS_TEXTURE);
    normal.loadFromFile(NORMAL_TEXTURE);
    release.loadFromFile(RELEASE_TEXTURE);

    psapi::sfm::Sprite back_sprite;
    back_sprite.setTextureRect(BACKGROUND_RECT);
    back_sprite.setTexture(&back);
    //back_sprite.setColor(psapi::sfm::Color(213, 34, 124, 255));

    psapi::sfm::Sprite release_sprite;
    release_sprite.setTextureRect(BUTTON_RECT);
    release_sprite.setTexture(&release);
    //release_sprite.setColor(psapi::sfm::Color(255, 255, 255, 255));

    psapi::sfm::Sprite hover_sprite;
    hover_sprite.setTextureRect(BUTTON_RECT);
    hover_sprite.setTexture(&hover);
    //hover_sprite.setColor(psapi::sfm::RED);

    psapi::sfm::Sprite press_sprite;
    press_sprite.setTextureRect(BUTTON_RECT);
    press_sprite.setTexture(&press);
    //press_sprite.setColor(psapi::sfm::GREEN);

    psapi::sfm::Sprite normal_sprite;
    normal_sprite.setTextureRect(BUTTON_RECT);
    normal_sprite.setTexture(&normal);
    //normal_sprite.setColor(psapi::sfm::BLUE);

    auto bar = std::make_unique<ToolBar>(psapi::vec2i(0, 0),
                                         psapi::vec2u(128, 32),
                                         std::make_unique<psapi::sfm::Sprite>(back_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(normal_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(hover_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(press_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(release_sprite));

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    root->addWindow(std::move(bar));
}

void unloadPlugin()
{
}

// ********************* TOOLBAR *********************


void ToolBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    psapi::sfm::RenderWindow* desktop = static_cast<psapi::sfm::RenderWindow*>(renderWindow);

    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(desktop);

    for (auto& button : buttons_)
    {
        finishButtonDraw(renderWindow, button.get());
        button.get()->draw(renderWindow);
    }

}
bool ToolBar::update(const psapi::sfm::IRenderWindow* renderWindow, const psapi::sfm::Event& event)
{
    if (!is_active_)
        return false;

    bool flag = false;

    for (auto& button : buttons_)
    {
        flag |= button.get()->update(renderWindow, event);
    }

    return flag;
}

psapi::ChildInfo ToolBar::getNextChildInfo() const
{
    if (next_child_ < buttons_.size())
    {
        psapi::vec2u size = buttons_[next_child_]->getSize();
        psapi::vec2i pos = buttons_[next_child_]->getPos();

        psapi::ChildInfo info = {{pos.x,  pos.y},
                                 {size.x, size.y}};

        return info;
    }
    else
    {
        return {{0, 0}, {0, 0}};
    }
}

ToolBar::ToolBar(const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    pos_(pos), size_(size),
    background_(std::move(background)),
    hovered_(std::move(hovered)),
    pressed_(std::move(pressed)),
    normal_(std::move(normal)),
    released_(std::move(released))
{
    parent_ = nullptr;
    is_active_ = true;
    next_child_ = 0;
}

void ToolBar::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    psapi::IBarButton* button = dynamic_cast<psapi::IBarButton*>(window.release());

    buttons_.push_back(std::unique_ptr<psapi::IBarButton>(button));
}

void ToolBar::removeWindow(psapi::wid_t id)
{
    for (size_t i = 0; i < buttons_.size(); ++i)
    {
        if (buttons_[i]->getId() == id)
        {
            buttons_.erase(buttons_.begin() + i);
            break;
        }
    }
}

void ToolBar::finishButtonDraw(psapi::sfm::IRenderWindow* renderWindow, const psapi::IBarButton* button) const
{
    psapi::vec2i pos = button->getPos();
    psapi::sfm::RenderWindow* desktop = static_cast<psapi::sfm::RenderWindow*>(renderWindow);

    switch (button->getState())
    {
        case psapi::IBarButton::State::Normal:
            normal_.get()->setPosition(pos.x, pos.y);
            normal_.get()->draw(desktop);
            break;
        case psapi::IBarButton::State::Hover:
            hovered_.get()->setPosition(pos.x, pos.y);
            hovered_.get()->draw(desktop);
            break;
        case psapi::IBarButton::State::Press:
            pressed_.get()->setPosition(pos.x, pos.y);
            pressed_.get()->draw(desktop);
            break;
        case psapi::IBarButton::State::Released:
            released_.get()->setPosition(pos.x, pos.y);
            released_.get()->draw(desktop);
            break;
    }
}

psapi::IWindow* ToolBar::getWindowById(psapi::wid_t id)
{
    if (id == getId())
        return this;

    for (auto& button : buttons_)
    {
        if (button->getId() == id)
            return button.get();
    }

    return nullptr;
};

const psapi::IWindow* ToolBar::getWindowById(psapi::wid_t id) const
{
    if (id == getId())
        return this;

    for (auto& button : buttons_)
    {
        if (button->getId() == id)
            return button.get();
    }

    return nullptr;
};

psapi::vec2i ToolBar::getPos() const
{
    return pos_;
}
psapi::vec2u ToolBar::getSize() const
{
    return size_;
}

void ToolBar::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ToolBar::forceActivate()
{
    is_active_ = true;
}
void ToolBar::forceDeactivate()
{
    is_active_ = false;
}
bool ToolBar::isActive() const
{
    return is_active_;
}
