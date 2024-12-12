#include <iostream>

#include "../plugins/geometry/geometry.hpp"

static const psapi::sfm::vec2i NO_CATCH = {-1, -1};

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {60, 60}};

bool onLoadPlugin()
{
    std::cout << "geometry loaded\n";

    auto root = psapi::getRootWindow();
    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    std::unique_ptr<psapi::sfm::ISprite> rec_sprite = psapi::sfm::ISprite::create();
    rec_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::IRectangleShape> rect = psapi::sfm::IRectangleShape::create(0, 0);
    rect.get()->setFillColor(psapi::sfm::Color(0, 255, 0, 255));

    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));
    auto toolbar_pos = tool_bar->getPos();

    auto rectangle = std::make_unique<GeometryButton>(kRectangleButtonId, tool_bar,
                                                psapi::vec2i(toolbar_pos + psapi::vec2i(18, 3 * 18 + 2 * BUTTON_RECT.size.y)),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(rec_sprite),
                                               canvas, std::move(rect));


    if (tool_bar)
    {
        tool_bar->addWindow(std::move(rectangle));
    }
}

void onUnloadPlugin()
{

}


GeometryButton::GeometryButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 psapi::ICanvas* canvas, std::unique_ptr<psapi::sfm::IShape> shape) :
                 SwitchButton(id, bar, pos, size, std::move(sprite)), canvas_(canvas), shape_(std::move(shape)), catch_pos_(NO_CATCH)
{}

std::unique_ptr<psapi::IAction> GeometryButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = canvas_->isPressedLeftMouseButton();

    if (LMB_down && catch_pos_.x == NO_CATCH.x && catch_pos_.y == NO_CATCH.y)
    {
        catch_pos_ = mouse_pos;
    }

    if (!LMB_down && (catch_pos_.x != NO_CATCH.x || catch_pos_.y != NO_CATCH.y))
    {
        catch_pos_ = NO_CATCH;
        canvas_->cleanTempLayer();
    }

    return std::make_unique<GeometryAction>(renderWindow, event, shape_.get(), canvas_, catch_pos_);
}

GeometryAction::GeometryAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, psapi::sfm::IShape* shape, psapi::ICanvas* canvas, psapi::sfm::vec2i catch_pos) :
                                AAction(render_window, event), shape_(shape), canvas_(canvas), catch_pos_(catch_pos)
{}

bool GeometryAction::execute(const Key& key)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = canvas_->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas_->getSize();

    psapi::ILayer* layer = canvas_->getTempLayer();

    if (event_.type == psapi::sfm::Event::MouseMoved && LMB_down)
    {
        psapi::sfm::vec2i shape_pos = {shape_->getPosition().x, shape_->getPosition().y};
        auto shape_size = shape_->getSize();

        for (int x = 0; x < shape_size.x; x++)
        {
            for (int y = 0; y < shape_size.y; y++)
            {
                layer->setPixel(psapi::sfm::vec2i(x, y) + shape_pos, {0, 0, 0, 0});
            }
        }

        shape_pos = {std::min(catch_pos_.x, mouse_pos.x), std::min(catch_pos_.y, mouse_pos.y)};
        shape_size = {abs(mouse_pos.x - catch_pos_.x), abs(mouse_pos.y - catch_pos_.y)};

        shape_->setPosition(shape_pos);
        shape_->setSize(shape_size);

        const psapi::sfm::IImage* shape_img = shape_->getImage();

        for (int x = 0; x < shape_size.x; x++)
        {
            for (int y = 0; y < shape_size.y; y++)
            {
                psapi::sfm::Color pixel = shape_img->getPixel(x + shape_pos.x, y + shape_pos.y);
                layer->setPixel(psapi::sfm::vec2i(x, y) + shape_pos, pixel);
            }
        }
    }

    return true;
}

bool GeometryAction::isUndoable(const Key& key)
{
    return false;
}
