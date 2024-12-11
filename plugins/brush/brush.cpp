#include <cassert>
#include <iostream>

#include "../plugins/brush/brush.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static const char* BUTTON_TEXTURE = "assets/textures/pen.png";

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {90, 90}};

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
    btn_sprite.get()->setTexture(btn);

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));
    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    auto toolbar_pos = tool_bar->getPos();

    auto brush = std::make_unique<PaintButton>(kBrushButtonId, tool_bar,
                                               psapi::vec2i(toolbar_pos + psapi::vec2i(0, 18)),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(btn_sprite),
                                               psapi::sfm::Color(255, 0, 0, 255), 3, canvas);

    std::unique_ptr<psapi::sfm::ISprite> ers_sprite = psapi::sfm::ISprite::create();
    ers_sprite.get()->setTextureRect(BUTTON_RECT);

    auto eraser = std::make_unique<PaintButton>(kEraserButtonId, tool_bar,
                                                psapi::vec2i(toolbar_pos + psapi::vec2i(0, 36 + BUTTON_RECT.size.y)),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(ers_sprite),
                                               canvas->getCanvasBaseColor(), 20, canvas);


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

PaintAction::PaintAction(const psapi::IRenderWindow* render_window, const psapi::Event& event,
                         const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas) :
                        AAction(render_window, event), color_(color), radius_(radius), array_(), canvas_(canvas)
{}

bool PaintAction::execute(const Key& key)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = canvas_->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas_->getSize();

    psapi::ILayer* layer = canvas_->getTempLayer();
    if (!layer)
            return false;

    if (LMB_down)
    {
        psapi::vec2i pos = mouse_pos;

        if (array_.size() < CATMULL_LEN)
        {
            array_.push_back(pos);
        }
        else
        {
            array_.queue_push(pos);

            double delta = 0.001 * static_cast<double>(radius_);
            double max_point = static_cast<double>(CATMULL_LEN - 2);

            //int radius = static_cast<int>(scale_related_ ? static_cast<double>(radius_) / scale.x : radius_);

            for (double i = 1; i < max_point; i += delta)
            {
                set_point(layer, array_.getInterpolated(i), color_, radius_);
            }
        }
    }
    else
    {
        if (array_.size())
            canvas_->cleanTempLayer();

        array_.clear();
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
                 const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas) :
            SwitchButton(id, bar, pos, size, std::move(sprite)), color_(color), radius_(radius), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> PaintButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<PaintAction>(renderWindow, event, color_, radius_, canvas_);
}

