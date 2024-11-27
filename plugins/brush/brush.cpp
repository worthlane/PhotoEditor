#include <cassert>
#include <iostream>

#include "../plugins/brush/brush.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

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
    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    auto brush = std::make_unique<SwitchButton>(kBrushButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(btn_sprite),
                                               std::make_unique<PaintAction>(sfm::RED, 3, canvas));

    std::unique_ptr<psapi::sfm::ISprite> ers_sprite = psapi::sfm::ISprite::create();
    ers_sprite.get()->setTextureRect(BUTTON_RECT);

    auto eraser = std::make_unique<SwitchButton>(kEraserButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(ers_sprite),
                                               std::make_unique<PaintAction>(sfm::WHITE, 20, canvas, true));


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

PaintAction::PaintAction(const psapi::sfm::Color& color, const size_t radius, psapi::ICanvas* canvas, const bool scale_related) :
color_(color), radius_(radius), array_(), scale_related_(scale_related), canvas_(canvas)
{}

bool PaintAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

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
