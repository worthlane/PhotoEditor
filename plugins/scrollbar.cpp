#include "../plugins/scrollbar.hpp"

AScrollBar::AScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ITexture> background,
            std::unique_ptr<psapi::sfm::ITexture> normal,
            std::unique_ptr<psapi::sfm::ITexture> active,
            Scrollable* object) :
    id_(id),
    pos_(pos), scroller_pos_(pos),
    size_(size.x, size.y), scroller_size_(size.x, size.y),
    background_(std::move(background)),
    normal_(std::move(normal)),
    active_(std::move(active)),
    object_(object)
{}

psapi::wid_t AScrollBar::getId() const
{
    return id_;
}

psapi::IWindow* AScrollBar::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? static_cast<psapi::IWindow*>(this) : nullptr;
}
const psapi::IWindow* AScrollBar::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? static_cast<const psapi::IWindow*>(this) : nullptr;
}
psapi::vec2i AScrollBar::getPos() const
{
    return pos_;
}
psapi::vec2u AScrollBar::getSize() const
{
    return {size_.x, size_.y};
}
void AScrollBar::setParent(const psapi::IWindow* parent)
{
    return; // no parent for scrollbar
}

void AScrollBar::forceActivate()
{
    is_active_ = true;
}
void AScrollBar::forceDeactivate()
{
    is_active_ = false;
}

bool AScrollBar::isActive() const
{
    return is_active_;
}
bool AScrollBar::isWindowContainer() const
{
    return false;
}

void AScrollBar::loadStateTexture(psapi::sfm::ISprite* sprite) const
{
    switch (state_)
    {
        case State::Normal:
            sprite->setTexture(normal_.get());
            break;
        case State::Active:
            sprite->setTexture(active_.get());
            break;
    }
}

HorizontalScrollBar::HorizontalScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                                        std::unique_ptr<psapi::sfm::ITexture> background,
                                        std::unique_ptr<psapi::sfm::ITexture> normal,
                                        std::unique_ptr<psapi::sfm::ITexture> active,
                                        Scrollable* object) :
    AScrollBar(id, pos, size, std::move(background), std::move(normal), std::move(active), object)
{

}

void HorizontalScrollBar::draw(psapi::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    psapi::sfm::ISprite* background = psapi::sfm::ISprite::create().release();
    background->setTextureRect({0, 0, size_.x, size_.y});
    background->setTexture(background_.get());
    background->setPosition(pos_.x, pos_.y);

    psapi::sfm::ISprite* scroller = psapi::sfm::ISprite::create().release();
    scroller->setTextureRect({0, 0, scroller_size_.x, scroller_size_.y});
    scroller->setPosition(scroller_pos_.x, scroller_pos_.y);
    loadStateTexture(scroller);

    background->draw(renderWindow);
    scroller->draw(renderWindow);

    delete background;
    delete scroller;
}

bool HorizontalScrollBar::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return false;

    psapi::sfm::vec2f scale = object_->getScale();
    psapi::sfm::vec2i coord_start = object_->getCoordStart();
    psapi::sfm::vec2u obj_size = object_->getObjectSize();

    scroller_size_ = {size_.x / scale.x, size_.y};
    double part = (static_cast<double>(coord_start.x) / static_cast<double>(obj_size.x));
    scroller_pos_  = {pos_.x + static_cast<int>(part * size_.x), pos_.y};

    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool pressed = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool bar_hovered = (mouse_pos.x >= pos_.x && mouse_pos.x <= pos_.x + size_.x) &&
                       (mouse_pos.y >= pos_.y && mouse_pos.y <= pos_.y + size_.y);

    bool scroller_hovered = (mouse_pos.x >= scroller_pos_.x && mouse_pos.x <= scroller_pos_.x + scroller_size_.x) &&
                            (mouse_pos.y >= scroller_pos_.y && mouse_pos.y <= scroller_pos_.y + scroller_size_.y);


    switch (state_)
    {
        case AScrollBar::State::Normal:
            if (pressed && scroller_hovered)
                state_ = AScrollBar::State::Active;
            break;

        case AScrollBar::State::Active:
            if (!pressed)
                state_ = AScrollBar::State::Normal;
            break;
    }

    return false;
}
