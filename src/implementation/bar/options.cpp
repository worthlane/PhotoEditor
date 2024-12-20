#include <iostream>
#include <cassert>
#include <cmath>

#include "api/api_system.hpp"
#include "api/api_sfm.hpp"

#include "implementation/bar/options.hpp"
#include "style/design.hpp"

static const psapi::vec2u STD_PALETTE_SIZE = {200, 28};

using psapi::sfm::Color;

static const std::string BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const std::string NORMAL_RAIL_TEXTURE = "assets/textures/white.jpg";
static const std::string ACTIVE_RAIL_TEXTURE = "assets/textures/white.jpg";
static const std::string NORMAL_TEXTURE = "assets/textures/normal_slider21.png";
static const std::string HOVER_TEXTURE = "assets/textures/hovered_slider21.png";
static const std::string ACTIVE_TEXTURE = "assets/textures/pressed_slider21.png";

static const psapi::vec2u STD_SLIDER_BACKGROUND_SIZE = {240, 36};
static const psapi::sfm::IntRect SLIDER_RECT = {{0, 0}, {21, 21}};
static const psapi::sfm::IntRect RAIL_RECT = {{0, 0}, {200, 4}};


static const std::vector<psapi::sfm::Color> BASE_COLORS = {Color::getStandardColor(Color::Type::Black),
                                                           Color::getStandardColor(Color::Type::White),
                                                           Color::getStandardColor(Color::Type::Red),
                                                           Color::getStandardColor(Color::Type::Green),
                                                           Color::getStandardColor(Color::Type::Blue),
                                                           Color::getStandardColor(Color::Type::Yellow),
                                                           Color::getStandardColor(Color::Type::Magenta),
                                                           Color::getStandardColor(Color::Type::Cyan)};

// ============= COLOR PALETTE ==============

ColorPalette::ColorPalette()
{
    pos_ = {0, 0};
    size_ = STD_PALETTE_SIZE;
    id_ = psapi::kColorPaletteId;

    current_ = psapi::sfm::IRectangleShape::create(STD_PALETTE_SIZE);
    current_->setFillColor(color_);

    for (auto& color : BASE_COLORS)
    {
        auto choice = psapi::sfm::IRectangleShape::create(STD_PALETTE_SIZE);
        choice->setFillColor(color);
        variants_.push_back(std::move(choice));
    }

    placeColors(pos_, size_);
}

void ColorPalette::placeColors(const psapi::sfm::vec2i pos, const psapi::sfm::vec2u size)
{
    static const double CURR_SPACE_COEF = 0.6;

    current_->setSize(psapi::sfm::vec2u(size.y, size.y));
    current_->setPosition(pos);

    size_t side = size.y * CURR_SPACE_COEF;

    int gap = ((size.x - size.y) / BASE_COLORS.size()) - side;

    int y_pos  = pos.y + (size.y - side) / 2;
    int x_start = pos.x + gap + size.y;

    for (auto& variant : variants_)
    {
        variant->setSize(psapi::sfm::vec2u(side, side));
        variant->setPosition(psapi::sfm::vec2i(x_start, y_pos));
        x_start += side + gap;
    }

}

bool ColorPalette::hoverRectangle(const psapi::sfm::vec2i pos, const psapi::sfm::IRectangleShape* rect)
{
    auto rect_pos = rect->getPosition();
    auto rect_size = rect->getSize();

    return (pos.x >= rect_pos.x && pos.x <= rect_pos.x + rect_size.x &&
            pos.y >= rect_pos.y && pos.y <= rect_pos.y + rect_size.y);
}

psapi::wid_t ColorPalette::getId() const
{
    return id_;
}

psapi::IWindow* ColorPalette::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* ColorPalette::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i ColorPalette::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u ColorPalette::getSize() const
{
    return size_;
}

void ColorPalette::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;

    placeColors(pos_, size);
}

void ColorPalette::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;

    placeColors(pos, size_);
}

void ColorPalette::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ColorPalette::forceActivate()
{
    is_active_ = true;
}

void ColorPalette::forceDeactivate()
{
    is_active_ = false;
}

bool ColorPalette::isActive() const
{
    return is_active_;
}

bool ColorPalette::isWindowContainer() const
{
    return false;
}

psapi::sfm::Color ColorPalette::getColor() const
{
    return color_;
}

void ColorPalette::setColor(const psapi::sfm::Color &color)
{
    color_ = color;

    current_->setFillColor(color);
}

void ColorPalette::draw(psapi::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    current_->draw(renderWindow);

    for (auto& variant : variants_)
    {
        variant->draw(renderWindow);
    }
}

std::unique_ptr<psapi::IAction> ColorPalette::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<ColorPaletteAction>(renderWindow, event, this);
}

std::unique_ptr<psapi::IColorPalette> psapi::IColorPalette::create()
{
    return std::make_unique<ColorPalette>();
}


ColorPaletteAction::ColorPaletteAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ColorPalette* palette) :
            AAction(render_window, event), palette_(palette)
{}

bool ColorPaletteAction::execute(const Key& key)
{
    if (!psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left))
    {
        return false;
    }

    auto mouse_pos = psapi::sfm::Mouse::getPosition(render_window_);

    for (auto& variant : palette_->variants_)
    {
        if (palette_->hoverRectangle(mouse_pos, variant.get()))
        {
            palette_->setColor(variant->getFillColor());
            return true;
        }
    }

    return true;
}

bool ColorPaletteAction::isUndoable(const Key& key)
{
    return false;
}

// ================== OPACITY OPTIONS ===================

OpacityOption::OpacityOption()
{
    pos_ = {0, 0};
    size_ = STD_SLIDER_BACKGROUND_SIZE;
    id_ = psapi::kOpacityBarId;

    background_ = psapi::sfm::ITexture::create();
    background_->loadFromFile(BACKGROUND_TEXTURE);

    normal_rail_ = psapi::sfm::ITexture::create();
    normal_rail_->loadFromFile(NORMAL_RAIL_TEXTURE);

    active_rail_ = psapi::sfm::ITexture::create();
    active_rail_->loadFromFile(ACTIVE_RAIL_TEXTURE);

    normal_ = psapi::sfm::ITexture::create();
    normal_->loadFromFile(NORMAL_TEXTURE);

    hover_ = psapi::sfm::ITexture::create();
    hover_->loadFromFile(HOVER_TEXTURE);

    active_ = psapi::sfm::ITexture::create();
    active_->loadFromFile(ACTIVE_TEXTURE);

    rail_pos_ = pos_ + psapi::sfm::vec2i(((int) STD_SLIDER_BACKGROUND_SIZE.x - (int) RAIL_RECT.size.x) / 2,
                                        ((int) STD_SLIDER_BACKGROUND_SIZE.y - (int) RAIL_RECT.size.y) / 2);

    slider_pos_ = psapi::sfm::vec2i(0, -((int) SLIDER_RECT.size.y - (int) RAIL_RECT.size.y) / 2);
}

psapi::wid_t OpacityOption::getId() const
{
    return id_;
}

psapi::IWindow* OpacityOption::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* OpacityOption::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i OpacityOption::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u OpacityOption::getSize() const
{
    return size_;
}

void OpacityOption::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;
}

void OpacityOption::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;

    rail_pos_ = pos_ + psapi::sfm::vec2i(((int) STD_SLIDER_BACKGROUND_SIZE.x - (int) RAIL_RECT.size.x) / 2,
                                        ((int) STD_SLIDER_BACKGROUND_SIZE.y - (int) RAIL_RECT.size.y) / 2);;
}

void OpacityOption::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void OpacityOption::forceActivate()
{
    is_active_ = true;
}

void OpacityOption::forceDeactivate()
{
    is_active_ = false;
}

bool OpacityOption::isActive() const
{
    return is_active_;
}

bool OpacityOption::isWindowContainer() const
{
    return false;
}

float OpacityOption::getOpacity() const
{
    return opacity_;
}

void OpacityOption::setOpacity(float opacity)
{
    opacity_ = opacity;
}

std::unique_ptr<psapi::IAction> OpacityOption::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<OpacityOptionAction>(renderWindow, event, this);
}

std::unique_ptr<psapi::IOpacityOption> psapi::IOpacityOption::create()
{
    return std::make_unique<OpacityOption>();
}

void OpacityOption::loadStateTexture(psapi::sfm::ISprite* sprite) const
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

void OpacityOption::draw(psapi::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    //std::cout << "draw\n";

    psapi::sfm::ISprite* background = psapi::sfm::ISprite::create().release();
    background->setTextureRect({{0, 0}, {size_.x, size_.y}});
    background->setTexture(background_.get());
    background->setPosition(pos_.x, pos_.y);
    background->setColor(Color(41, 44, 49));

    psapi::sfm::ISprite* slider = psapi::sfm::ISprite::create().release();
    slider->setTextureRect(SLIDER_RECT);
    slider->setPosition(rail_pos_.x + slider_pos_.x, rail_pos_.y + slider_pos_.y);
    loadStateTexture(slider);

    psapi::sfm::ISprite* inactive_rail = psapi::sfm::ISprite::create().release();
    inactive_rail->setTextureRect({{0, 0}, {slider_pos_.x, RAIL_RECT.size.y}});
    inactive_rail->setTexture(normal_rail_.get());
    inactive_rail->setPosition(rail_pos_.x, rail_pos_.y);
    inactive_rail->setColor(Color(60, 62, 66));

    psapi::sfm::ISprite* active_rail = psapi::sfm::ISprite::create().release();
    active_rail->setTextureRect({{0, 0}, {RAIL_RECT.size.x - slider_pos_.x, RAIL_RECT.size.y}});
    active_rail->setTexture(active_rail_.get());
    active_rail->setPosition(rail_pos_.x + slider_pos_.x, rail_pos_.y);
    active_rail->setColor(Color(0, 166, 165));

    background->draw(renderWindow);
    inactive_rail->draw(renderWindow);
    active_rail->draw(renderWindow);
    slider->draw(renderWindow);

    delete background;
    delete slider;
    delete inactive_rail;
    delete active_rail;
}

void OpacityOption::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool pressed = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool rail_hovered = (mouse_pos.x >= rail_pos_.x && mouse_pos.x <= rail_pos_.x + RAIL_RECT.size.x) &&
                       (mouse_pos.y >= rail_pos_.y && mouse_pos.y <= rail_pos_.y + RAIL_RECT.size.y);

    bool slider_hovered = (mouse_pos.x >= rail_pos_.x + slider_pos_.x && mouse_pos.x <= rail_pos_.x + slider_pos_.x + SLIDER_RECT.size.x) &&
                          (mouse_pos.y >= rail_pos_.y + slider_pos_.y && mouse_pos.y <= rail_pos_.y + slider_pos_.y + SLIDER_RECT.size.y);

    switch (state_)
    {
        case OpacityOption::State::Normal:
            if (slider_hovered)
            {
                state_ = OpacityOption::State::Hover;
            }

            if (pressed && rail_hovered && !slider_hovered)
            {
                state_ = OpacityOption::State::Active;
                setSliderPos({mouse_pos.x - rail_pos_.x - SLIDER_RECT.size.x / 2, mouse_pos.y - rail_pos_.y - SLIDER_RECT.size.y / 2});
            }

            break;

        case OpacityOption::State::Hover:
            if (pressed)
            {
                state_ = OpacityOption::State::Active;
                catch_pos_ = {mouse_pos.x - rail_pos_.x - slider_pos_.x, mouse_pos.y - rail_pos_.y - slider_pos_.y};
            }

            if (!slider_hovered)
            {
                state_ = OpacityOption::State::Normal;
            }

            break;

        case OpacityOption::State::Active:
            if (!pressed)
            {
                state_ = OpacityOption::State::Normal;
                catch_pos_ = {SLIDER_RECT.size.x / 2, SLIDER_RECT.size.y / 2};
            }
            else
            {
                setSliderPos({mouse_pos.x - rail_pos_.x - catch_pos_.x, mouse_pos.y - rail_pos_.y - catch_pos_.y});
            }

            break;
    }
}

void OpacityOption::setSliderPos(psapi::sfm::vec2i pos)
{
    if (pos.x < -((int) SLIDER_RECT.size.x) / 2)
        pos.x = -((int) SLIDER_RECT.size.x) / 2;

    if (pos.x > (int) RAIL_RECT.size.x - (int) SLIDER_RECT.size.x / 2)
        pos.x = (int) RAIL_RECT.size.x - (int) SLIDER_RECT.size.x / 2;

    opacity_ = (float) (pos.x + SLIDER_RECT.size.x / 2) / (float) RAIL_RECT.size.x;

    slider_pos_ = {pos.x, slider_pos_.y};
}

void OpacityOption::updateSlider()
{
    slider_pos_ = {opacity_ * RAIL_RECT.size.x - SLIDER_RECT.size.x / 2, slider_pos_.y};
}

OpacityOptionAction::OpacityOptionAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, OpacityOption* option) :
            AAction(render_window, event), option_(option)
{}

bool OpacityOptionAction::execute(const Key& key)
{
    if (!option_->isActive())
        return false;

    option_->updateState(render_window_, event_);
    option_->updateSlider();

    return true;
}

bool OpacityOptionAction::isUndoable(const Key& key)
{
    return false;
}

// ========= THICKNESS OPTION ========

ThicknessOption::ThicknessOption()
{
    pos_ = {0, 0};
    size_ = STD_SLIDER_BACKGROUND_SIZE;
    id_ = psapi::kThicknessBarId;

    background_ = psapi::sfm::ITexture::create();
    background_->loadFromFile(BACKGROUND_TEXTURE);

    normal_rail_ = psapi::sfm::ITexture::create();
    normal_rail_->loadFromFile(NORMAL_RAIL_TEXTURE);

    active_rail_ = psapi::sfm::ITexture::create();
    active_rail_->loadFromFile(ACTIVE_RAIL_TEXTURE);

    normal_ = psapi::sfm::ITexture::create();
    normal_->loadFromFile(NORMAL_TEXTURE);

    hover_ = psapi::sfm::ITexture::create();
    hover_->loadFromFile(HOVER_TEXTURE);

    active_ = psapi::sfm::ITexture::create();
    active_->loadFromFile(ACTIVE_TEXTURE);

    rail_pos_ = pos_ + psapi::sfm::vec2i(((int) STD_SLIDER_BACKGROUND_SIZE.x - (int) RAIL_RECT.size.x) / 2,
                                        ((int) STD_SLIDER_BACKGROUND_SIZE.y - (int) RAIL_RECT.size.y) / 2);

    slider_pos_ = psapi::sfm::vec2i(0, -((int) SLIDER_RECT.size.y - (int) RAIL_RECT.size.y) / 2);

    updateSlider();
}

psapi::wid_t ThicknessOption::getId() const
{
    return id_;
}

psapi::IWindow* ThicknessOption::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* ThicknessOption::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i ThicknessOption::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u ThicknessOption::getSize() const
{
    return size_;
}

void ThicknessOption::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;
}

void ThicknessOption::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;

    rail_pos_ = pos_ + psapi::sfm::vec2i(((int) STD_SLIDER_BACKGROUND_SIZE.x - (int) RAIL_RECT.size.x) / 2,
                                        ((int) STD_SLIDER_BACKGROUND_SIZE.y - (int) RAIL_RECT.size.y) / 2);;
}

void ThicknessOption::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void ThicknessOption::forceActivate()
{
    is_active_ = true;
}

void ThicknessOption::forceDeactivate()
{
    is_active_ = false;
}

bool ThicknessOption::isActive() const
{
    return is_active_;
}

bool ThicknessOption::isWindowContainer() const
{
    return false;
}

float ThicknessOption::getThickness() const
{
    return thickness_;
}

void ThicknessOption::setThickness(float thickness)
{
    thickness_ = thickness;
}

std::unique_ptr<psapi::IAction> ThicknessOption::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<ThicknessOptionAction>(renderWindow, event, this);
}

std::unique_ptr<psapi::IThicknessOption> psapi::IThicknessOption::create()
{
    return std::make_unique<ThicknessOption>();
}

void ThicknessOption::loadStateTexture(psapi::sfm::ISprite* sprite) const
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

void ThicknessOption::draw(psapi::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    psapi::sfm::ISprite* background = psapi::sfm::ISprite::create().release();
    background->setTextureRect({{0, 0}, {size_.x, size_.y}});
    background->setTexture(background_.get());
    background->setPosition(pos_.x, pos_.y);
    background->setColor(Color(41, 44, 49));

    psapi::sfm::ISprite* slider = psapi::sfm::ISprite::create().release();
    slider->setTextureRect(SLIDER_RECT);
    slider->setPosition(rail_pos_.x + slider_pos_.x, rail_pos_.y + slider_pos_.y);
    loadStateTexture(slider);

    psapi::sfm::ISprite* inactive_rail = psapi::sfm::ISprite::create().release();
    inactive_rail->setTextureRect({{0, 0}, {slider_pos_.x, RAIL_RECT.size.y}});
    inactive_rail->setTexture(normal_rail_.get());
    inactive_rail->setPosition(rail_pos_.x, rail_pos_.y);
    inactive_rail->setColor(Color(60, 62, 66));

    psapi::sfm::ISprite* active_rail = psapi::sfm::ISprite::create().release();
    active_rail->setTextureRect({{0, 0}, {RAIL_RECT.size.x - slider_pos_.x, RAIL_RECT.size.y}});
    active_rail->setTexture(active_rail_.get());
    active_rail->setPosition(rail_pos_.x + slider_pos_.x, rail_pos_.y);
    active_rail->setColor(Color(0, 166, 165));

    background->draw(renderWindow);
    inactive_rail->draw(renderWindow);
    active_rail->draw(renderWindow);
    slider->draw(renderWindow);

    delete background;
    delete slider;
    delete inactive_rail;
    delete active_rail;
}

void ThicknessOption::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool pressed = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    bool rail_hovered = (mouse_pos.x >= rail_pos_.x && mouse_pos.x <= rail_pos_.x + RAIL_RECT.size.x) &&
                       (mouse_pos.y >= rail_pos_.y && mouse_pos.y <= rail_pos_.y + RAIL_RECT.size.y);

    bool slider_hovered = (mouse_pos.x >= rail_pos_.x + slider_pos_.x && mouse_pos.x <= rail_pos_.x + slider_pos_.x + SLIDER_RECT.size.x) &&
                          (mouse_pos.y >= rail_pos_.y + slider_pos_.y && mouse_pos.y <= rail_pos_.y + slider_pos_.y + SLIDER_RECT.size.y);

    switch (state_)
    {
        case ThicknessOption::State::Normal:
            if (slider_hovered)
            {
                state_ = ThicknessOption::State::Hover;
            }

            if (pressed && rail_hovered && !slider_hovered)
            {
                state_ = ThicknessOption::State::Active;
                setSliderPos({mouse_pos.x - rail_pos_.x - SLIDER_RECT.size.x / 2, mouse_pos.y - rail_pos_.y - SLIDER_RECT.size.y / 2});
            }

            break;

        case ThicknessOption::State::Hover:
            if (pressed)
            {
                state_ = ThicknessOption::State::Active;
                catch_pos_ = {mouse_pos.x - rail_pos_.x - slider_pos_.x, mouse_pos.y - rail_pos_.y - slider_pos_.y};
            }

            if (!slider_hovered)
            {
                state_ = ThicknessOption::State::Normal;
            }

            break;

        case ThicknessOption::State::Active:
            if (!pressed)
            {
                state_ = ThicknessOption::State::Normal;
                catch_pos_ = {SLIDER_RECT.size.x / 2, SLIDER_RECT.size.y / 2};
            }
            else
            {
                setSliderPos({mouse_pos.x - rail_pos_.x - catch_pos_.x, mouse_pos.y - rail_pos_.y - catch_pos_.y});
            }

            break;
    }
}

void ThicknessOption::setSliderPos(psapi::sfm::vec2i pos)
{
    if (pos.x < -((int) SLIDER_RECT.size.x) / 2)
        pos.x = -((int) SLIDER_RECT.size.x) / 2;

    if (pos.x > (int) RAIL_RECT.size.x - (int) SLIDER_RECT.size.x / 2)
        pos.x = (int) RAIL_RECT.size.x - (int) SLIDER_RECT.size.x / 2;

    float thickness = (float) (pos.x + SLIDER_RECT.size.x / 2) / (float) RAIL_RECT.size.x;
    if (thickness < 0) thickness = 0;

    thickness_ = thickness;

    slider_pos_ = {pos.x, slider_pos_.y};
}

void ThicknessOption::updateSlider()
{
    slider_pos_ = {thickness_ * RAIL_RECT.size.x - SLIDER_RECT.size.x / 2, slider_pos_.y};
}

ThicknessOptionAction::ThicknessOptionAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ThicknessOption* option) :
            AAction(render_window, event), option_(option)
{}

bool ThicknessOptionAction::execute(const Key& key)
{
    if (!option_->isActive())
        return false;

    option_->updateState(render_window_, event_);
    option_->updateSlider();

    return true;
}

bool ThicknessOptionAction::isUndoable(const Key& key)
{
    return false;
}
