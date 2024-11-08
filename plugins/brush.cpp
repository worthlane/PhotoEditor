#include <cassert>
#include <iostream>

#include "../plugins/brush.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static const psapi::sfm::IntRect BUTTON_RECT = {0, 0, 90, 90};
static const char* BUTTON_TEXTURE = "assets/textures/pen.png";

void set_point(psapi::ILayer* layer, const psapi::vec2i& pos,
                      const psapi::sfm::Color& color, const int radius);

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

    auto brush = std::make_unique<SwitchButton>(kBrushButtonId,
                                                psapi::vec2i(19, 19),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(btn_sprite),
                                               canvas,
                                               std::make_unique<PaintAction>(sfm::RED, 3));

    std::unique_ptr<psapi::sfm::ISprite> ers_sprite = psapi::sfm::ISprite::create();
    ers_sprite.get()->setTextureRect(BUTTON_RECT);

    auto eraser = std::make_unique<SwitchButton>(kEraserButtonId,
                                                psapi::vec2i(19, 128),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(ers_sprite),
                                               canvas,
                                               std::make_unique<PaintAction>(sfm::WHITE, 20));


    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(brush));
        tool_bar->addWindow(std::move(eraser));
    }
}

void unloadPlugin()
{
    delete btn;
}

// ======================================================

PaintAction::PaintAction(const psapi::sfm::Color& color, size_t radius) :
color_(color), radius_(radius), array_()
{}

bool PaintAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event,
                             psapi::ICanvas* canvas)
{
    psapi::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow);
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    /*
    if (state != BrushButton::State::Released)
    {
        array_.clear();
        return true;
    }
    */

    psapi::vec2i canvas_pos  = canvas->getPos();
    psapi::vec2u canvas_size = canvas->getSize();

    bool mouse_in_canvas = (mouse_pos.x >= canvas_pos.x) && (mouse_pos.x < canvas_pos.x + canvas_size.x) &&
                           (mouse_pos.y >= canvas_pos.y) && (mouse_pos.y < canvas_pos.y + canvas_size.y);

    if (LMB_down)
    {
        psapi::ILayer* layer = canvas->getLayer(0);

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

            double delta = 0.001 * static_cast<double>(radius_);

            double max_point = static_cast<double>(CATMULL_LEN - 2);

            for (double i = 1; i < max_point; i += delta)
            {
                set_point(layer, array_.getInterpolated(i), color_, radius_);
            }
        }
    }
    else
    {
        array_.clear();
    }

    return true;
}

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
