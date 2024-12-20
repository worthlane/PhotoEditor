#include <cassert>
#include <iostream>

#include "../plugins/brush/brush.hpp"

#include "api/api_bar.hpp"

static psapi::sfm::ITexture* brsh_text = nullptr;
static psapi::sfm::ITexture* ers_text = nullptr;

static const char* BRUSH_TEXTURE = "assets/textures/brush.png";
static const char* ERASER_TEXTURE = "assets/textures/eraser.png";

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {30, 30}};

void set_point(psapi::ILayer* layer, const psapi::vec2i& pos,
                      const psapi::sfm::Color& color, const int radius);

static const size_t CATMULL_LEN = 4;

bool onLoadPlugin()
{
    std::cout << "brush loaded\n";

    brsh_text = psapi::sfm::ITexture::create().release();
    brsh_text->loadFromFile(BRUSH_TEXTURE);

    ers_text = psapi::sfm::ITexture::create().release();
    ers_text->loadFromFile(ERASER_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> btn_sprite = psapi::sfm::ISprite::create();
    btn_sprite.get()->setTextureRect(BUTTON_RECT);
    btn_sprite.get()->setTexture(brsh_text);

    auto root = psapi::getRootWindow();
    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));
    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto toolbar_pos = tool_bar->getPos();

    auto brush = std::make_unique<PaintButton>(kBrushButtonId, tool_bar,
                                               psapi::vec2i(2, 17),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(btn_sprite), 100);

    std::unique_ptr<psapi::sfm::ISprite> ers_sprite = psapi::sfm::ISprite::create();
    ers_sprite.get()->setTextureRect(BUTTON_RECT);
    ers_sprite.get()->setTexture(ers_text);

    auto eraser = std::make_unique<PaintButton>(kEraserButtonId, tool_bar,
                                                psapi::vec2i(6 + BUTTON_RECT.size.x, 17),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(ers_sprite), 100, true);

    psapi::sfm::Color col = canvas->getCanvasBaseColor();

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(brush));
        tool_bar->addWindow(std::move(eraser));
    }
}

void onUnloadPlugin()
{
    delete brsh_text;
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
    auto radius = button_->radius_ * button_->thickness_->getThickness();
    if (radius < 1) radius = 1;

    psapi::vec2i mouse_pos = canvas->getMousePosition();
    bool LMB_down = canvas->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas->getSize();
    psapi::ILayer* layer = canvas->getTempLayer();
    if (!layer)
            return false;

    if (!button_->is_eraser_)
        button_->color_ = button_->palette_->getColor();

    button_->color_.a = 255 * button_->opacity_->getOpacity();

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
                 std::unique_ptr<psapi::sfm::ISprite> sprite, const size_t radius, const bool is_eraser) :
            SwitchButton(id, bar, pos, size, std::move(sprite)), radius_(radius), array_(), is_eraser_(is_eraser)
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
        has_options_ = false;
        return std::make_unique<IdleAction>(renderWindow, event);
    }

    if (!has_options_)
        replaceOptions();

    auto pos = canvas_->getMousePosition();
    bool is_on_canvas = pos.x >= 0 && pos.x < canvas_->getSize().x &&
                        pos.y >= 0 && pos.y < canvas_->getSize().y;

    if (canvas_->isPressedLeftMouseButton() && is_on_canvas)
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

    if (is_eraser_)
        return std::make_unique<EraseAction>(renderWindow, event, this);

    return std::make_unique<PaintAction>(renderWindow, event, this);
}

void PaintButton::replaceOptions()
{
    options_bar_->removeAllOptions();

    createOptions();

    has_options_ = true;
}

void PaintButton::createOptions()
{
    auto root = psapi::getRootWindow();

    if (!is_eraser_)
    {
        palette_ = dynamic_cast<psapi::IColorPalette*>(root->getWindowById(psapi::kColorPaletteId));
        palette_->forceActivate();
        assert(palette_);

        opacity_ = dynamic_cast<psapi::IOpacityOption*>(root->getWindowById(psapi::kOpacityBarId));
        opacity_->forceActivate();
        assert(opacity_);
    }

    thickness_ = dynamic_cast<psapi::IThicknessOption*>(root->getWindowById(psapi::kThicknessBarId));
    thickness_->forceActivate();
    assert(thickness_);
}

EraseAction::EraseAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, PaintButton* button)
    : AAction(render_window, event), button_(button)
{
}

bool EraseAction::execute(const Key& key)
{
    auto canvas = button_->canvas_;
    auto array = button_->array_;
    auto color = button_->color_;
    auto radius = button_->radius_ * button_->thickness_->getThickness();
    if (radius < 1) radius = 1;

    psapi::vec2i mouse_pos = canvas->getMousePosition();
    bool LMB_down = canvas->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas->getSize();
    psapi::ILayer* layer = canvas->getTempLayer();
    if (!layer)
            return false;

    psapi::ILayer* active = canvas->getLayer(canvas->getActiveLayerIndex());

    if (canvas->isPressedLeftMouseButton() && array.size() >= CATMULL_LEN)
    {
        double delta = 0.001 * static_cast<double>(radius);
        double max_point = static_cast<double>(CATMULL_LEN - 2);

        for (double i = 1; i < max_point; i += delta)
        {
            auto pos = array.getInterpolated(i);

            set_point(active, pos, psapi::sfm::Color(0, 0, 0, 0), radius);
        }
    }

    return true;
}
bool EraseAction::isUndoable(const Key& key)
{
    return false;
}
