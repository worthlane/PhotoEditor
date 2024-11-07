#include <cassert>
#include <iostream>

#include "../plugins/brush.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static const psapi::sfm::IntRect BUTTON_RECT = {0, 0, 90, 90};
static const char* BUTTON_TEXTURE = "assets/textures/pen.png";

static void set_point(psapi::ILayer* layer, const psapi::vec2i& pos);

static const size_t CATMULL_LEN = 4;


bool loadPlugin()
{
    std::cout << "brush loaded\n";

    btn = psapi::sfm::ITexture::create().release();

    btn->loadFromFile(BUTTON_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> btn_sprite = psapi::sfm::ISprite::create();
    btn_sprite.get()->setTextureRect(BUTTON_RECT);
    btn_sprite.get()->setTexture(btn);

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto brush = std::make_unique<BrushButton>(psapi::vec2i(19, 19),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(btn_sprite),
                                               canvas);


    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(brush));
    }
}

void unloadPlugin()
{
    delete btn;
}


// ======================================================

BrushButton::BrushButton(const psapi::vec2i& pos, const psapi::vec2u& size,
                         std::unique_ptr<psapi::sfm::ISprite> sprite, psapi::ICanvas* canvas) :
    state_(BrushButton::State::Normal), size_(size), pos_(pos),
    sprite_(std::move(sprite)), parent_(nullptr), is_active_(true), canvas_(canvas), array_()
{
}

void BrushButton::setState(BrushButton::State state)
{
    prev_state_ = state_;
    state_ = state;
}

BrushButton::State BrushButton::getState() const
{
    return state_;
}

void BrushButton::draw(psapi::IRenderWindow* renderWindow)
{
    if (!is_active_)
        return;

    sprite_->setPosition(pos_.x, pos_.y);
    sprite_->draw(renderWindow);
}

bool BrushButton::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!is_active_)
        return false;

    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);

    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    updateState(renderWindow, event, mouse_pos, LMB_down);

    if (state_ != BrushButton::State::Released)
    {
        array_.clear();
        return true;
    }

    psapi::vec2i canvas_pos  = canvas_->getPos();
    psapi::vec2u canvas_size = canvas_->getSize();

    bool mouse_in_canvas = (mouse_pos.x >= canvas_pos.x) && (mouse_pos.x < canvas_pos.x + canvas_size.x) &&
                           (mouse_pos.y >= canvas_pos.y) && (mouse_pos.y < canvas_pos.y + canvas_size.y);

    if (LMB_down)
    {
        psapi::ILayer* layer = canvas_->getLayer(0);

        if (!layer)
            return false;

        psapi::vec2i pos = mouse_pos - canvas_pos;

        if (array_.size() < CATMULL_LEN)
        {
            array_.push_back(pos);
        }
        else
        {
            array_.queue_push(pos);

            for (double i = 1; i < 2; i += 0.001)
            {
                set_point(layer, array_.getInterpolated(i));
            }
        }
    }
    else
    {
        array_.clear();
    }

    return true;
}

static void set_point(psapi::ILayer* layer, const psapi::vec2i& pos)
{
    layer->setPixel(pos, sfm::RED);

    layer->setPixel(pos + psapi::vec2i(1, 0), sfm::RED);
    layer->setPixel(pos + psapi::vec2i(0, 1), sfm::RED);
    layer->setPixel(pos + psapi::vec2i(0, -1), sfm::RED);
    layer->setPixel(pos + psapi::vec2i(-1, 0), sfm::RED);
}

void BrushButton::updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event,
                              const psapi::vec2i& mouse_pos, const bool LMB_down)
{
    bool is_hovered = (mouse_pos.x >= pos_.x) && (mouse_pos.x < pos_.x + size_.x) &&
                      (mouse_pos.y >= pos_.y) && (mouse_pos.y < pos_.y + size_.y);

    //std::cout << "hovered: " << is_hovered << " LMB down: " << LMB_down << std::endl;

    switch (state_)
    {
        case BrushButton::State::Normal:

            if (is_hovered)
                setState(BrushButton::State::Hover);

            break;

        case BrushButton::State::Hover:

            if (LMB_down)
                state_ = BrushButton::State::Press;
            else if (!is_hovered)
                state_ = BrushButton::State::Normal;

            break;

        case BrushButton::State::Press:

            if (is_hovered && !LMB_down)
            {
                if (prev_state_ == BrushButton::State::Normal)
                    state_ = BrushButton::State::Released;
                else if (prev_state_ == BrushButton::State::Released)
                    state_ = BrushButton::State::Normal;
            }

            if (!is_hovered)
            {
                setState(prev_state_);
            }

            break;

        case BrushButton::State::Released:

            if (prev_state_ == BrushButton::State::Press)
            {
                if (!LMB_down)
                    prev_state_ = BrushButton::State::Released;
            }
            else
            {
                if (is_hovered && LMB_down)
                    setState(BrushButton::State::Press);
            }

            break;
    }
}

psapi::IWindow* BrushButton::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
};

const psapi::IWindow* BrushButton::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
};

psapi::vec2i BrushButton::getPos() const
{
    return pos_;
}

psapi::vec2u BrushButton::getSize() const
{
    return size_;
}

void BrushButton::setPos(const psapi::vec2i pos)
{
    pos_ = pos;
    sprite_->setPosition(pos.x, pos.y);
}

void BrushButton::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void BrushButton::forceActivate()
{
    is_active_ = true;
}

void BrushButton::forceDeactivate()
{
    is_active_ = false;
}

bool BrushButton::isActive() const
{
    return is_active_;
}

bool BrushButton::isWindowContainer() const
{
    return false;
};
