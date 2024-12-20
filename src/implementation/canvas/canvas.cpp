#include <cassert>
#include <string>
#include <iostream>

#include "implementation/canvas/canvas.hpp"
#include "implementation/utils.hpp"

#include "implementation/memento.hpp"

static const char* GRID = "assets/textures/psgrid.png";

static void dumpOnImage(psapi::ILayer* layer, psapi::sfm::IImage* image, const psapi::sfm::vec2i& coord_start, const psapi::sfm::vec2f& scale, const psapi::sfm::vec2i& size);

// ********* LAYER ***********************

Layer::Layer(const size_t width, const size_t height, const psapi::sfm::Color& color) : size_(width, height)
{
    image_ = psapi::sfm::IImage::create().release();
    image_->create(width, height, color);
}

psapi::sfm::Color Layer::getPixel(psapi::sfm::vec2i pos) const
{
    return image_->getPixel(pos.x, pos.y);
}
void Layer::setPixel(psapi::sfm::vec2i pos, psapi::sfm::Color pixel)
{
    if (pos.x < 0 || pos.y < 0 || pos.x >= image_->getSize().x || pos.y >= image_->getSize().y)
        return;

    image_->setPixel(pos.x, pos.y, pixel);
}

psapi::drawable_id_t Layer::addDrawable(std::unique_ptr<psapi::sfm::Drawable> object)
{
    assert(0);
}

void Layer::removeDrawable(psapi::drawable_id_t id)
{
    assert(0);
}
void Layer::removeAllDrawables()
{
    assert(0);
}

psapi::sfm::vec2u Layer::getSize() const
{
    return size_;
}

std::unique_ptr<psapi::ILayerSnapshot> Layer::save()
{
    assert(0);
}

void Layer::restore(psapi::ILayerSnapshot* snapshot)
{
    assert(0);
}

static void dumpOnImage(psapi::ILayer* layer, psapi::sfm::IImage* image, const psapi::sfm::vec2i& coord_start, const psapi::sfm::vec2f& scale, const psapi::sfm::vec2i& size)
{
    for (size_t y = 0; y < size.y; y++)
    {
        for (size_t x = 0; x < size.x; x++)
        {
            psapi::sfm::Color pixel = layer->getPixel(coord_start + psapi::sfm::vec2i(x / scale.x, y / scale.y));
            psapi::sfm::Color under = image->getPixel({x, y});

            if (pixel.a == 0) continue;

            float at = pixel.a / 255.0f;
            float au = under.a / 255.0f;

            float a0 = at + au * (1 - at);

            int r = (pixel.r * at + under.r * au * (1 - at)) / a0;
            int g = (pixel.g * at + under.g * au * (1 - at)) / a0;
            int b = (pixel.b * at + under.b * au * (1 - at)) / a0;

            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            pixel = psapi::sfm::Color(r, g, b, a0 * 255);

            image->setPixel({x, y}, pixel);
        }
    }
}

// ************* CANVAS ******************

Canvas::Canvas(const psapi::sfm::vec2i& pos, const psapi::sfm::vec2u& size,
               const psapi::sfm::Color& base_color) : pos_(pos), size_(size), base_color_(base_color)
{
    id_ = psapi::kCanvasWindowId;

    temp_layer_ = std::make_unique<Layer>(size_.x, size_.y, psapi::sfm::Color(0, 0, 0, 0));

    layers_.push_back(std::make_unique<Layer>(size_.x, size_.y, base_color_));

    background_sprite_ = psapi::sfm::ISprite::create();
    background_texture_ = psapi::sfm::ITexture::create();

    background_texture_->loadFromFile(GRID);
    background_sprite_->setTexture(background_texture_.get());
    background_sprite_->setTextureRect({{0, 0}, size_});
    background_sprite_->setPosition(pos.x, pos.y);
}

psapi::wid_t Canvas::getId() const
{
    return id_;
}

psapi::IWindow* Canvas::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* Canvas::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i Canvas::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u Canvas::getSize() const
{
    return size_;
}

void Canvas::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;
}

void Canvas::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;
}

void Canvas::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void Canvas::forceActivate()
{
    is_active_ = true;
}

void Canvas::forceDeactivate()
{
    is_active_ = false;
}

bool Canvas::isActive() const
{
    return is_active_;
}

bool Canvas::isWindowContainer() const
{
    return false;
}

psapi::ILayer* Canvas::getLayer(size_t index)
{
    return layers_[index].get();
}
const psapi::ILayer* Canvas::getLayer(size_t index) const
{
    return layers_[index].get();
}
psapi::ILayer* Canvas::getTempLayer()
{
    return temp_layer_.get();
}
const psapi::ILayer* Canvas::getTempLayer() const
{
    return temp_layer_.get();
}

void Canvas::cleanTempLayer()
{
    psapi::ILayer* active_layer = getLayer(getActiveLayerIndex());
    psapi::ILayer* temp_layer = getTempLayer();

    auto size = temp_layer->getSize();

    for (size_t y = 0; y < size.y; y++)
    {
        for (size_t x = 0; x < size.x; x++)
        {
            psapi::sfm::Color temp_pixel = temp_layer->getPixel({x, y});
            psapi::sfm::Color under_pixel = active_layer->getPixel({x, y});

            if (temp_pixel.a == 0)
                continue;

            float at = temp_pixel.a / 255.0f;
            float au = under_pixel.a / 255.0f;

            float a0 = at + au * (1 - at);

            int r = (temp_pixel.r * at + under_pixel.r * au * (1 - at)) / a0;
            int g = (temp_pixel.g * at + under_pixel.g * au * (1 - at)) / a0;
            int b = (temp_pixel.b * at + under_pixel.b * au * (1 - at)) / a0;

            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            temp_pixel = psapi::sfm::Color(r, g, b, a0 * 255);

            active_layer->setPixel({x, y}, temp_pixel);

            temp_layer->setPixel({x, y}, psapi::sfm::Color(0, 0, 0, 0));
        }
    }
}

size_t Canvas::getNumLayers() const
{
    return layers_.size();
}
size_t Canvas::getActiveLayerIndex() const
{
    return active_layer_;
}
void Canvas::setActiveLayerIndex(size_t index)
{
    active_layer_ = index;
}

bool Canvas::insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer)
{
    layers_.insert(layers_.begin() + index, std::move(layer));
    return true;
}
bool Canvas::removeLayer(size_t index)
{
    layers_.erase(layers_.begin() + index);
    return true;
}

bool Canvas::insertEmptyLayer(size_t index)
{
    assert(0);
}

void Canvas::setZoom(psapi::sfm::vec2f zoom)
{
    Scrollable::setScale(zoom);
}
psapi::sfm::vec2i Canvas::getMousePosition() const
{
    return mouse_pos_;
}
bool Canvas::isPressedRightMouseButton() const
{
    return is_pressed_RMB_;
}
bool Canvas::isPressedLeftMouseButton()  const
{
    return is_pressed_LMB_;
}
bool Canvas::isPressedScrollButton() const
{
    return is_pressed_scroll_;
}

psapi::sfm::Color Canvas::getCanvasBaseColor() const
{
    return base_color_;
}

std::unique_ptr<psapi::ICanvasSnapshot> Canvas::save()
{
    psapi::ILayer*  layer = getLayer(getActiveLayerIndex());

    auto canvas_size = getSize();

    std::unique_ptr<psapi::sfm::IImage> img = psapi::sfm::IImage::create();
    img->create(canvas_size.x, canvas_size.y);

    for (int x = 0; x < canvas_size.x; x++)
    {
        for (int y = 0; y < canvas_size.y; y++)
        {
            psapi::sfm::Color pixel = layer->getPixel({x, y});
            img->setPixel(x, y, pixel);
        }
    }
    return std::make_unique<ACanvasSnapshot>(std::move(img));;
}
void Canvas::restore(psapi::ICanvasSnapshot* snapshot)
{
    psapi::ILayer* layer = getLayer(getActiveLayerIndex());

    ACanvasSnapshot* canvas_snapshot = dynamic_cast<ACanvasSnapshot *>(snapshot);

    psapi::vec2u canvas_size = getSize();

    for (int x = 0; x < canvas_size.x; x++)
    {
        for (int y = 0; y < canvas_size.y; y++)
        {
            auto pixel = canvas_snapshot->getImage()->getPixel(x, y);

            layer->setPixel({x, y}, pixel);
        }
    }
}

void Canvas::draw(psapi::IRenderWindow* renderWindow)
{
    size_t layers_amount = layers_.size();

    psapi::sfm::IImage* final_image = psapi::sfm::IImage::create().release();
    psapi::sfm::ITexture* texture = psapi::sfm::ITexture::create().release();
    psapi::sfm::ISprite* sprite = psapi::sfm::ISprite::create().release();

    final_image->create(size_.x, size_.y, psapi::sfm::Color(0, 0, 0, 0));

    sprite->setPosition(pos_.x, pos_.y);

    psapi::sfm::vec2f scale = getScale();
    psapi::sfm::vec2i coord_start = getCoordStart();

    for (int i = layers_amount - 1; i >= 0; i--)
    {
        auto layer = getLayer(i);
        dumpOnImage(layer, final_image, coord_start, scale, size_);
    }

    auto temp_layer = getTempLayer();
    dumpOnImage(temp_layer, final_image, coord_start, scale, size_);

    texture->update(final_image);
    sprite->setTexture(texture);

    background_sprite_->setTextureRect({{0, 0}, size_});
    background_sprite_->setPosition(pos_.x, pos_.y);

    background_sprite_->draw(renderWindow);
    sprite->draw(renderWindow);

    delete texture;
    delete sprite;
    delete final_image;
}

std::unique_ptr<psapi::IAction> Canvas::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<CanvasAction>(renderWindow, event, this);
}

psapi::sfm::vec2u Canvas::getObjectSize() const
{
    return size_;
}

bool Canvas::updateScale(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    auto scale = getScale();
    auto coord_start = getCoordStart();
    auto mouse_pos = getMousePosition();

    if (event.type == psapi::sfm::Event::MouseWheelScrolled &&
        event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Vertical)
    {
        psapi::sfm::vec2i old_mouse_pos = mouse_pos / scale;

        scale += psapi::sfm::vec2f(0.03f * scale.x, 0.03f * scale.y) * event.mouseWheel.delta;

        setScale(scale);
        scale = getScale();

        psapi::sfm::vec2i new_mouse_pos = mouse_pos / scale;

        coord_start += old_mouse_pos - new_mouse_pos;

        setCoordStart(coord_start);
        coord_start = getCoordStart();
    }

    mouse_pos_ = coord_start + mouse_pos / scale;

    return true;
}

// ********* CANVAS ACTION *******

CanvasAction::CanvasAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, Canvas* canvas) :
    AAction(render_window, event), canvas_(canvas)
{}

bool CanvasAction::execute(const Key& key)
{
    canvas_->mouse_pos_ = psapi::sfm::Mouse::getPosition(render_window_) - canvas_->pos_;
    canvas_->is_pressed_LMB_ = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);
    canvas_->is_pressed_RMB_ = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Right);
    //canvas_->is_pressed_scroll_ = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Scroll);

    return canvas_->updateScale(render_window_, event_);
}

bool CanvasAction::isUndoable(const Key& key)
{
    return false;
}



