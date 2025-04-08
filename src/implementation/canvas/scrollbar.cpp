#include "implementation/canvas/scrollbar.hpp"

static const double MAX_SCALE = 80;
static const double MIN_SCALE = 1;

psapi::sfm::vec2i Scrollable::getCoordStart() const
{
    return coord_start_;
}

psapi::sfm::vec2f Scrollable::getScale() const
{
    return scale_;
}

void Scrollable::setCoordStart(psapi::sfm::vec2i coord_start)
{
    psapi::sfm::vec2u size = getObjectSize();

    psapi::sfm::vec2i right_lower_corner = coord_start + psapi::sfm::vec2i(size.x / scale_.x, size.y / scale_.y);

    if (right_lower_corner.x > size.x - 1) coord_start.x = size.x - (size.x / scale_.x);
    if (right_lower_corner.y > size.y - 1) coord_start.y = size.y - (size.y / scale_.y);
    if (coord_start.x < 0)                 coord_start.x = 0;
    if (coord_start.y < 0)                 coord_start.y = 0;

    coord_start_ = coord_start;
}

void Scrollable::setScale(psapi::sfm::vec2f scale)
{
    scale_ = scale;

    if (scale_.x < MIN_SCALE) scale_.x = MIN_SCALE;
    if (scale_.y < MIN_SCALE) scale_.y = MIN_SCALE;

    if (scale_.x > MAX_SCALE) scale_.x = MAX_SCALE;
    if (scale_.y > MAX_SCALE) scale_.y = MAX_SCALE;
}

AScrollBar::AScrollBar(const psapi::wid_t id, const psapi::sfm::vec2i& pos, const psapi::sfm::vec2u& size,
            std::unique_ptr<psapi::sfm::ITexture> background,
            std::unique_ptr<psapi::sfm::ITexture> normal,
            std::unique_ptr<psapi::sfm::ITexture> hover,
            std::unique_ptr<psapi::sfm::ITexture> active,
            Scrollable* object) :
    AWindow(id, pos, size),
    scroller_pos_(0, 0),
    scroller_size_(size.x, size.y),
    background_(std::move(background)),
    normal_(std::move(normal)),
    active_(std::move(active)),
    hover_(std::move(hover)),
    object_(object)
{}

void AScrollBar::loadStateTexture(psapi::sfm::ISprite* sprite) const
{
    switch (state_)
    {
        case State::Normal:
            sprite->setTexture(normal_.get());
            break;

        case State::Hover:
            sprite->setTexture(hover_.get());
            break;

        case State::Active:
            sprite->setTexture(active_.get());
            break;
    }
}

void AScrollBar::draw(psapi::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    psapi::sfm::ISprite* background = psapi::sfm::ISprite::create().release();
    background->setTextureRect({{0, 0}, {size_.x, size_.y}});
    background->setTexture(background_.get());
    background->setPosition(pos_.x, pos_.y);

    psapi::sfm::ISprite* scroller = psapi::sfm::ISprite::create().release();
    scroller->setTextureRect({{0, 0}, {scroller_size_.x, scroller_size_.y}});
    scroller->setPosition(pos_.x + scroller_pos_.x, pos_.y + scroller_pos_.y);
    loadStateTexture(scroller);

    background->draw(renderWindow);
    scroller->draw(renderWindow);

    delete background;
    delete scroller;
}

void AScrollBar::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event&)
{
    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool pressed = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool bar_hovered = (mouse_pos.x >= pos_.x && mouse_pos.x <= pos_.x + size_.x) &&
                       (mouse_pos.y >= pos_.y && mouse_pos.y <= pos_.y + size_.y);

    bool scroller_hovered = (mouse_pos.x >= pos_.x + scroller_pos_.x && mouse_pos.x <= pos_.x + scroller_pos_.x + scroller_size_.x) &&
                            (mouse_pos.y >= pos_.y + scroller_pos_.y && mouse_pos.y <= pos_.y + scroller_pos_.y + scroller_size_.y);

    switch (state_)
    {
        case AScrollBar::State::Normal:
            if (scroller_hovered)
                state_ = AScrollBar::State::Hover;

            if (pressed && bar_hovered && !scroller_hovered)
            {
                state_ = AScrollBar::State::Active;
                setScrollerPos({mouse_pos.x - pos_.x - scroller_size_.x / 2, mouse_pos.y - pos_.y - scroller_size_.y / 2});
            }

            break;

        case AScrollBar::State::Hover:
            if (pressed)
            {
                state_ = AScrollBar::State::Active;
                catch_pos_ = {mouse_pos.x - pos_.x - scroller_pos_.x, mouse_pos.y - pos_.y - scroller_pos_.y};
            }

            if (!scroller_hovered)
                state_ = AScrollBar::State::Normal;

            break;

        case AScrollBar::State::Active:
            if (!pressed)
            {
                state_ = AScrollBar::State::Normal;
                catch_pos_ = {scroller_size_.x / 2, scroller_size_.y / 2};
            }
            else
                setScrollerPos({mouse_pos.x - pos_.x - catch_pos_.x,
                                mouse_pos.y - pos_.y - catch_pos_.y});

            break;
    }
}

std::unique_ptr<psapi::IAction> AScrollBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<ScrollAction>(renderWindow, event, this);
}

// =============== HORIZONTAL SCROLLBAR ===============

HorizontalScrollBar::HorizontalScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                                        std::unique_ptr<psapi::sfm::ITexture> background,
                                        std::unique_ptr<psapi::sfm::ITexture> normal,
                                        std::unique_ptr<psapi::sfm::ITexture> hover,
                                        std::unique_ptr<psapi::sfm::ITexture> active,
                                        Scrollable* object) :
    AScrollBar(id, pos, size, std::move(background), std::move(normal), std::move(hover), std::move(active), object)
{}

void HorizontalScrollBar::setScrollerPos(psapi::sfm::vec2i pos)
{
    if (pos.x < 0)
        pos.x = 0;

    if (pos.x > size_.x - scroller_size_.x)
        pos.x = size_.x - scroller_size_.x;

    double delta = (static_cast<double>(pos.x) / static_cast<double>(size_.x));

    psapi::sfm::vec2i coord_start = object_->getCoordStart();
    psapi::sfm::vec2u obj_size   = object_->getObjectSize();

    coord_start = {static_cast<int>(delta * static_cast<double>(obj_size.x)), coord_start.y};

    object_->setCoordStart(coord_start);

    scroller_pos_ = pos;
}

void HorizontalScrollBar::updateScroller()
{
    psapi::sfm::vec2f scale       = object_->getScale();
    psapi::sfm::vec2i coord_start = object_->getCoordStart();
    psapi::sfm::vec2u obj_size    = object_->getObjectSize();

    scroller_size_ = {size_.x / scale.x, size_.y};

    double part = (static_cast<double>(coord_start.x) / static_cast<double>(obj_size.x));

    scroller_pos_  = {static_cast<int>(part * size_.x), 0};
}

// ========== VERTICAL SCROLLBAR ==========

VerticalScrollBar::VerticalScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                                        std::unique_ptr<psapi::sfm::ITexture> background,
                                        std::unique_ptr<psapi::sfm::ITexture> normal,
                                        std::unique_ptr<psapi::sfm::ITexture> hover,
                                        std::unique_ptr<psapi::sfm::ITexture> active,
                                        Scrollable* object) :
    AScrollBar(id, pos, size, std::move(background), std::move(normal), std::move(hover), std::move(active), object)
{}

void VerticalScrollBar::setScrollerPos(psapi::sfm::vec2i pos)
{
    if (pos.y < 0)
        pos.y = 0;

    if (pos.y > size_.y - scroller_size_.y)
        pos.y = size_.y - scroller_size_.y;

    double delta = (static_cast<double>(pos.y) / static_cast<double>(size_.y));

    psapi::sfm::vec2i coord_start = object_->getCoordStart();
    psapi::sfm::vec2u obj_size   = object_->getObjectSize();

    coord_start = {coord_start.x, static_cast<int>(delta * static_cast<double>(obj_size.y))};

    object_->setCoordStart(coord_start);

    scroller_pos_ = pos;
}

void VerticalScrollBar::updateScroller()
{
    psapi::sfm::vec2f scale       = object_->getScale();
    psapi::sfm::vec2i coord_start = object_->getCoordStart();
    psapi::sfm::vec2u obj_size    = object_->getObjectSize();

    scroller_size_ = {size_.x, size_.y / scale.y};

    double part = (static_cast<double>(coord_start.y) / static_cast<double>(obj_size.y));

    scroller_pos_  = {0, static_cast<int>(part * size_.y)};
}

// ********* SCROLL ACTION *******

ScrollAction::ScrollAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, AScrollBar* scrollbar) :
    AAction(render_window, event), scrollbar_(scrollbar)
{}

bool ScrollAction::execute(const Key& key)
{
    if (!scrollbar_->isActive())
        return false;

    scrollbar_->updateState(render_window_, event_);

    scrollbar_->updateScroller();

    return true;
}

bool ScrollAction::isUndoable(const Key& key)
{
    return false;
}
