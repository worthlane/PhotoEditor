#include <iostream>

#include "../plugins/geometry.hpp"
#include "../plugins/colors.hpp"

static const psapi::sfm::vec2i NO_CATCH = {-1, -1};

static const psapi::sfm::IntRect BUTTON_RECT = {0, 0, 90, 90};

bool loadPlugin()
{
    std::cout << "geometry loaded\n";

    auto root = psapi::getRootWindow();
    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    std::unique_ptr<psapi::sfm::ISprite> rec_sprite = psapi::sfm::ISprite::create();
    rec_sprite.get()->setTextureRect(BUTTON_RECT);

    auto rectangle = std::make_unique<SwitchButton>(kRectangleButtonId,
                                                psapi::vec2i(19, 237),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(rec_sprite),
                                               std::make_unique<GeometryAction>(canvas, nullptr));


    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

    if (tool_bar)
    {
        tool_bar->addWindow(std::move(rectangle));
    }
}

void unloadPlugin()
{

}

// ===================== GEOMETRY ACTION =======================

GeometryAction::GeometryAction(psapi::ICanvas* canvas, psapi::sfm::IShape* shape) :
    shape_(shape), canvas_(canvas), catch_pos_(NO_CATCH)
{}

bool GeometryAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    psapi::vec2u canvas_size = canvas_->getSize();

    psapi::ILayer* layer = canvas_->getTempLayer();
    if (!layer)
            return false;

    bool drawn_shape = false;

    if (LMB_down)
    {
        if (catch_pos_.x == NO_CATCH.x && catch_pos_.y == NO_CATCH.y)
        {
            catch_pos_ = mouse_pos;
        }

        psapi::vec2i pos = mouse_pos;

        for (int x = 0; x < shape_rect_.width; x++)
        {
            for (int y = 0; y < shape_rect_.height; y++)
            {
                layer->setPixel({x + shape_rect_.top_x, y + shape_rect_.top_y}, {0, 0, 0, 0});
            }
        }

        shape_rect_ = {std::min(catch_pos_.x, mouse_pos.x),
                       std::min(catch_pos_.y, mouse_pos.y),
                       abs(mouse_pos.x - catch_pos_.x),
                       abs(mouse_pos.y - catch_pos_.y)};

        for (int x = 0; x < shape_rect_.width; x++)
        {
            for (int y = 0; y < shape_rect_.height; y++)
            {
                layer->setPixel({x + shape_rect_.top_x, y + shape_rect_.top_y}, sfm::BLUE);
            }
        }

        drawn_shape = true;
    }
    else
    {
        catch_pos_ = NO_CATCH;
        if (drawn_shape)
        {
            canvas_->cleanTempLayer();
            drawn_shape = false;
        }
    }

    return true;
}
