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

    std::unique_ptr<psapi::sfm::IRectangleShape> rect = psapi::sfm::IRectangleShape::create(0, 0);
    rect.get()->setFillColor(sfm::BLUE);

    auto rectangle = std::make_unique<SwitchButton>(kRectangleButtonId,
                                                psapi::vec2i(19, 237),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(rec_sprite),
                                               std::make_unique<GeometryAction>(canvas, std::move(rect)));


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

GeometryAction::GeometryAction(psapi::ICanvas* canvas, std::unique_ptr<psapi::sfm::IShape> shape) :
    shape_(std::move(shape)), canvas_(canvas), catch_pos_(NO_CATCH)
{}

bool GeometryAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    psapi::vec2u canvas_size = canvas_->getSize();

    psapi::ILayer* layer = canvas_->getTempLayer();

    if (LMB_down && catch_pos_.x == NO_CATCH.x && catch_pos_.y == NO_CATCH.y)
    {
        catch_pos_ = mouse_pos;
    }

    if (event.type == psapi::sfm::Event::MouseMoved && LMB_down)
    {
        psapi::sfm::IShape* shape = shape_.get();

        psapi::sfm::vec2i shape_pos = {shape->getPosition().x, shape->getPosition().y};
        auto shape_size = shape->getSize();

        for (int x = 0; x < shape_size.x; x++)
        {
            for (int y = 0; y < shape_size.y; y++)
            {
                layer->setPixel(psapi::sfm::vec2i(x, y) + shape_pos, {0, 0, 0, 0});
            }
        }

        shape_pos = {std::min(catch_pos_.x, mouse_pos.x), std::min(catch_pos_.y, mouse_pos.y)};
        shape_size = {abs(mouse_pos.x - catch_pos_.x), abs(mouse_pos.y - catch_pos_.y)};

        shape->setPosition(shape_pos);
        shape->setSize(shape_size);

        const psapi::sfm::IImage* shape_img = shape->getImage();

        for (int x = 0; x < shape_size.x; x++)
        {
            for (int y = 0; y < shape_size.y; y++)
            {
                psapi::sfm::Color pixel = shape_img->getPixel(x + shape_pos.x, y + shape_pos.y);
                layer->setPixel(psapi::sfm::vec2i(x, y) + shape_pos, pixel);
            }
        }
    }

    if (!LMB_down && (catch_pos_.x != NO_CATCH.x || catch_pos_.y != NO_CATCH.y))
    {
        catch_pos_ = NO_CATCH;
        canvas_->cleanTempLayer();
    }

    return true;
}
