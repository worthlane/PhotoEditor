#include <cassert>
#include <iostream>

#include "plugins/toolbar.hpp"
#include "api/root_window.hpp"

static psapi::sfm::Texture back;

bool loadPlugin()
{
    std::cout << "aue2\n";

    back.loadFromFile("assets/textures/bar_template.png");

    psapi::sfm::Sprite back_sprite;
    back_sprite.setTexture(&back);
    back_sprite.setColor(psapi::sfm::Color(213, 34, 124, 255));

    psapi::sfm::Sprite hover_sprite;
    hover_sprite.setColor(psapi::sfm::RED);

    psapi::sfm::Sprite press_sprite;
    press_sprite.setColor(psapi::sfm::GREEN);

    psapi::sfm::Sprite normal_sprite;
    normal_sprite.setColor(psapi::sfm::BLUE);

    auto bar = std::make_unique<ToolBar>(psapi::vec2i(0, 0),
                                         psapi::vec2u(128, 32),
                                         std::make_unique<psapi::sfm::Sprite>(back_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(hover_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(press_sprite),
                                         std::make_unique<psapi::sfm::Sprite>(normal_sprite));

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    root->addWindow(std::move(bar));
}

void unloadPlugin()
{
}

// ****************** TOOLBARBUTTON ******************


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
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> normal) :
    pos_(pos), size_(size),
    background_(std::move(background)),
    hovered_(std::move(hovered)),
    pressed_(std::move(pressed)),
    normal_(std::move(normal))
{
    parent_ = nullptr;
    is_active_ = true;
    next_child_ = 0;
}

void ToolBar::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    //std::unique_ptr<psapi::IBarButton> button = reinterpret_cast<std::unique_ptr<psapi::IBarButton>>(window);

    psapi::IBarButton* button = dynamic_cast<psapi::IBarButton*>(window.get());

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
            normal_.get()->setPosition(pos.x, pos.y);
            normal_.get()->draw(desktop);
            break;
    }
}

psapi::IWindow* ToolBar::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* ToolBar::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
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
