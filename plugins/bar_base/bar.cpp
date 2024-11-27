#include <cassert>
#include <iostream>

#include "../plugins/bar_base/bar.hpp"
#include "../plugins/style/design.hpp"
#include "../plugins/bar_base/bar_button.hpp"

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {0, 0, 128, 700};

static const char* BACKGROUND_TEXTURE = "assets/textures/background_gray.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/hovered_icon.png";
static const char* RELEASE_TEXTURE    = "assets/textures/active_icon.png";
static const char* PRESS_TEXTURE      = "assets/textures/pressed_icon.png";
static const char* NORMAL_TEXTURE     = "assets/textures/normal_icon.png";

bool loadPlugin()
{
    std::cout << "toolbar loaded\n";

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
    //back_sprite.get()->setTextureRect(BACKGROUND_RECT);
    //back_sprite.get()->setTexture(back);

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, BUTTON_RECT, 1);

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, BUTTON_RECT, 1);

    auto bar = std::make_unique<ToolBar>(psapi::vec2i(20, 20),
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

// ********************* TOOLBAR *********************/

ToolBar::ToolBar(const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    ABar(pos, size, std::move(background),
                    std::move(normal),
                    std::move(hovered),
                    std::move(pressed),
                    std::move(released))
{
}


void ToolBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

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

    for (size_t i = 0; i < buttons_.size(); ++i)
    {
        auto& button = buttons_[i];

        psapi::IBarButton::State prev_state = button.get()->getState();

        flag |= button.get()->update(renderWindow, event);

        psapi::IBarButton::State cur_state = button.get()->getState();

        if (cur_state == psapi::IBarButton::State::Released &&
            prev_state != psapi::IBarButton::State::Released)
        {
            for (size_t j = 0; j < buttons_.size(); ++j)
                if (j != i) buttons_[j].get()->setState(psapi::IBarButton::State::Normal);
        }
    }

    return flag;
}

// ******************** ABAR ***********************

psapi::ChildInfo ABar::getNextChildInfo() const
{
    if (curr_child_ >= buttons_.size())
    {
        return {{0, 0}, {0, 0}};
    }

    psapi::vec2u size = buttons_[curr_child_]->getSize();
    psapi::vec2i pos = buttons_[curr_child_]->getPos();

    if (curr_child_ == 0)
    {
        curr_child_++;
        curr_child_ = curr_child_ % buttons_.size();

        return {{pos_.x + gap_.x, pos_.y + gap_.y}, {size.x, size.y}};
    }

    psapi::vec2u prev_size = buttons_[curr_child_ - 1]->getSize();
    psapi::vec2i prev_pos = buttons_[curr_child_ - 1]->getPos();

    psapi::ChildInfo info = {{prev_pos.x + prev_size.x + gap_.x,  prev_pos.y},
                             {size.x, size.y}};

    if (info.pos.x + gap_.x > pos_.x + size_.x)
    {
        info.pos = {pos_.x + gap_.x, prev_pos.y + gap_.y + prev_size.y};
    }

    curr_child_++;
    curr_child_ = curr_child_ % buttons_.size();

    return info;

}

psapi::wid_t ABar::getId() const
{
    return psapi::kToolBarWindowId;
}

ABar::ABar(const psapi::vec2i& pos, const psapi::vec2u& size,
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
    curr_child_ = 0;
}

void ABar::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    psapi::IBarButton* button = dynamic_cast<psapi::IBarButton*>(window.release());
    button->setParent(this);

    buttons_.push_back(std::unique_ptr<psapi::IBarButton>(button));
}

void ABar::removeWindow(psapi::wid_t id)
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

void ABar::finishButtonDraw(psapi::sfm::IRenderWindow* renderWindow, const psapi::IBarButton* button) const
{
    psapi::vec2i pos = button->getPos();

    switch (button->getState())
    {
        case psapi::IBarButton::State::Normal:
            normal_.get()->setPosition(pos.x, pos.y);
            normal_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Hover:
            hovered_.get()->setPosition(pos.x, pos.y);
            hovered_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Press:
            pressed_.get()->setPosition(pos.x, pos.y);
            pressed_.get()->draw(renderWindow);
            break;
        case psapi::IBarButton::State::Released:
            released_.get()->setPosition(pos.x, pos.y);
            released_.get()->draw(renderWindow);
            break;
    }
}

psapi::IWindow* ABar::getWindowById(psapi::wid_t id)
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

const psapi::IWindow* ABar::getWindowById(psapi::wid_t id) const
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

psapi::vec2i ABar::getPos() const
{
    return pos_;
}
psapi::vec2u ABar::getSize() const
{
    return size_;
}

void ABar::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ABar::forceActivate()
{
    is_active_ = true;
}
void ABar::forceDeactivate()
{
    is_active_ = false;
}
bool ABar::isActive() const
{
    return is_active_;
}

bool ABar::isWindowContainer() const
{
    return true;
}
