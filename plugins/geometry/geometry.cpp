#include <iostream>

#include "../plugins/geometry/geometry.hpp"

static const psapi::sfm::vec2i NO_CATCH = {-1, -1};

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {60, 60}};

bool onLoadPlugin()
{
    std::cout << "geometry loaded\n";

    std::unique_ptr<psapi::sfm::ISprite> rec_sprite = psapi::sfm::ISprite::create();
    rec_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::IRectangleShape> rect = psapi::sfm::IRectangleShape::create(0, 0);
    rect.get()->setFillColor(psapi::sfm::Color(0, 255, 0, 255));

    auto root = psapi::getRootWindow();
    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));
    auto toolbar_pos = tool_bar->getPos();

    auto rectangle = std::make_unique<GeometryButton>(kRectangleButtonId, tool_bar,
                                                psapi::vec2i(3 * 18 + 2 * BUTTON_RECT.size.x, 18),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(rec_sprite),
                                               std::move(rect));


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
                 std::unique_ptr<psapi::sfm::IShape> shape) :
                 SwitchButton(id, bar, pos, size, std::move(sprite)), shape_(std::move(shape)), catch_pos_(NO_CATCH)
{
    auto root = psapi::getRootWindow();
    canvas_ = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));
    options_bar_ = static_cast<psapi::IOptionsBar*>(root->getWindowById(psapi::kOptionsBarWindowId));
}

std::unique_ptr<psapi::IAction> GeometryButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
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

    return std::make_unique<GeometryAction>(renderWindow, event, this);
}

void GeometryButton::replaceOptions()
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

void GeometryButton::createOptions()
{
    auto palette = psapi::IColorPalette::create();

    psapi::sfm::Color col = shape_->getFillColor();

    palette_ = palette.get();
    palette_->setColor(col);

    options_.push_back(std::move(palette));
}

GeometryAction::GeometryAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, GeometryButton* button) :
                                AAction(render_window, event), button_(button)
{}

bool GeometryAction::execute(const Key& key)
{
    auto canvas = button_->canvas_;
    auto palette = button_->palette_;
    auto shape   = (button_->shape_).get();
    auto catch_pos = button_->catch_pos_;

    psapi::vec2i mouse_pos = canvas->getMousePosition();
    bool LMB_down = canvas->isPressedLeftMouseButton();

    psapi::vec2u canvas_size = canvas->getSize();

    psapi::ILayer* layer = canvas->getTempLayer();

    shape->setFillColor(palette->getColor());

    if (event_.type == psapi::sfm::Event::MouseMoved && LMB_down)
    {
        psapi::sfm::vec2i shape_pos = {shape->getPosition().x, shape->getPosition().y};
        auto shape_size = shape->getSize();

        for (int x = 0; x < shape_size.x; x++)
        {
            for (int y = 0; y < shape_size.y; y++)
            {
                layer->setPixel(psapi::sfm::vec2i(x, y) + shape_pos, {0, 0, 0, 0});
            }
        }

        shape_pos = {std::min(catch_pos.x, mouse_pos.x), std::min(catch_pos.y, mouse_pos.y)};
        shape_size = {abs(mouse_pos.x - catch_pos.x), abs(mouse_pos.y - catch_pos.y)};

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

    return true;
}

bool GeometryAction::isUndoable(const Key& key)
{
    return false;
}
