#include <cassert>
#include <iostream>

#include "../plugins/brush/brush.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static const char* BUTTON_TEXTURE = "assets/textures/pen.png";

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {60, 60}};

void set_point(psapi::ILayer* layer, const psapi::vec2i& pos,
                      const psapi::sfm::Color& color, const int radius);

static const size_t CATMULL_LEN = 4;


bool onLoadPlugin()
{
    std::cout << "brush loaded\n";

    btn = psapi::sfm::ITexture::create().release();

    btn->loadFromFile(BUTTON_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> btn_sprite = psapi::sfm::ISprite::create();
    btn_sprite.get()->setTextureRect(BUTTON_RECT);
    //btn_sprite.get()->setTexture(btn);

    auto root = psapi::getRootWindow();
    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));
    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto toolbar_pos = tool_bar->getPos();

    auto brush = std::make_unique<PaintButton>(kBrushButtonId, tool_bar,
                                               psapi::vec2i(18, 18),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(btn_sprite),
                                               psapi::sfm::Color(255, 0, 0, 255), 3);

    std::unique_ptr<psapi::sfm::ISprite> ers_sprite = psapi::sfm::ISprite::create();
    ers_sprite.get()->setTextureRect(BUTTON_RECT);

    auto eraser = std::make_unique<PaintButton>(kEraserButtonId, tool_bar,
                                                psapi::vec2i(36 + BUTTON_RECT.size.x, 18),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(ers_sprite),
                                               canvas->getCanvasBaseColor(), 20);

    psapi::sfm::Color col = canvas->getCanvasBaseColor();

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(brush));
        tool_bar->addWindow(std::move(eraser));
    }
}

void onUnloadPlugin()
{
    delete btn;
}

// ======================================================

void set_point(psapi::ILayer* layer, const psapi::vec2i& pos,
                      const psapi::sfm::Color& color, const int radius)
{
    int rad2 = radius * radius;

    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            if (i * i + j * j <= rad2)
                layer->setPixel(pos + psapi::vec2i(i, j), color);
        }
    }
}

PaintAction::PaintAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, PaintButton* button) :
                        AAction(render_window, event), button_(button)
{
}

bool PaintAction::execute(const Key& key)
{
    auto canvas = button_->canvas_;
    auto array = button_->array_;
    auto color = button_->color_;
    auto radius = button_->radius_;

    psapi::vec2i mouse_pos = canvas->getMousePosition();
    bool LMB_down = canvas->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas->getSize();
    psapi::ILayer* layer = canvas->getTempLayer();
    if (!layer)
            return false;

    if (canvas->isPressedLeftMouseButton() && array.size() >= CATMULL_LEN)
    {
        double delta = 0.001 * static_cast<double>(radius);
        double max_point = static_cast<double>(CATMULL_LEN - 2);

        for (double i = 1; i < max_point; i += delta)
        {
            set_point(layer, array.getInterpolated(i), color, radius);
        }
    }

    return true;
}

bool PaintAction::isUndoable(const Key& key)
{
    return false;
}

// *********** PAINT BUTTON ***************

PaintButton::PaintButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 const psapi::sfm::Color& color, const size_t radius) :
            SwitchButton(id, bar, pos, size, std::move(sprite)), color_(color), radius_(radius), array_()
{
    auto root = psapi::getRootWindow();
    canvas_ = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));
    options_bar_ = static_cast<psapi::IOptionsBar*>(root->getWindowById(psapi::kOptionsBarWindowId));
}

std::unique_ptr<psapi::IAction> PaintButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
    {
        /*if (prev_state_ == SwitchButton::State::Released)
        {
            options_bar_->removeAllOptions();
        }*/

        has_options_ = false;
        return std::make_unique<IdleAction>(renderWindow, event);
    }

    if (!has_options_)
        replaceOptions();

    if (canvas_->isPressedLeftMouseButton())
    {

        if (array_.size() < CATMULL_LEN)
            array_.push_back(canvas_->getMousePosition());
        else
            array_.queue_push(canvas_->getMousePosition());
    }
    else
    {
        if (array_.size())
            canvas_->cleanTempLayer();

        array_.clear();
    }

    return std::make_unique<PaintAction>(renderWindow, event, this);
}

void PaintButton::replaceOptions()
{
    options_bar_->removeAllOptions();

    createOptions();

    for (auto& option : options_)
    {
        options_bar_->addWindow(std::move(option));
    }

    options_.clear();
    has_options_ = true;
}

void PaintButton::createOptions()
{
    auto palette = psapi::IColorPalette::create();

    palette->setColor(color_);

    options_.push_back(std::move(palette));
}

