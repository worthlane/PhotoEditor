#include <cassert>
#include <iostream>

//#include "../plugins/colors.hpp"
#include "../plugins/canvas.hpp"

bool loadPlugin()
{
    std::cout << "canvas loaded\n";

    auto canvas = std::make_unique<Canvas>(psapi::sfm::vec2i(128, 20),
                                           psapi::sfm::vec2i(1072, 780),
                                           psapi::sfm::vec2f(1, 1));

    psapi::RootWindow* root = static_cast<psapi::RootWindow*>(psapi::getRootWindow());

    root->addWindow(std::move(canvas));
}

void unloadPlugin()
{
}


// ======================================================

// ******************* LAYER ******************

Layer::Layer(const size_t width, const size_t height)
{
    image_ = psapi::sfm::IImage::create().release();
    image_->create(width, height, psapi::sfm::WHITE);
}

Layer::Layer(const size_t width, const size_t height, const psapi::sfm::Color& color)
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

void Layer::resize(const size_t width, const size_t height)
{
    // TODO
}

// ****************** CANVAS *******************

Canvas::Canvas(const psapi::sfm::vec2i& pos,
               const psapi::sfm::vec2i& size,
               const psapi::sfm::vec2f& scale) :
    layers_(), temp_layer_(std::make_unique<Layer>(size_.x, size_.y)),
    mouse_pos_(psapi::sfm::vec2i(0, 0)), pressed_(false)
{
    pos_ = {pos.x, pos.y};
    size_ = {size.x, size.y};
    scale_ = {scale.x, scale.y};

    layers_.push_back(std::make_unique<Layer>(size_.x, size_.y, psapi::sfm::WHITE));
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

size_t Canvas::getActiveLayerIndex() const
{
    return active_layer_;
}

void Canvas::setActiveLayerIndex(size_t index)
{
    active_layer_ = index;
}

void Canvas::draw(psapi::IRenderWindow* renderWindow)
{
    size_t layers_amount = layers_.size();

    psapi::sfm::RenderWindow* desktop = static_cast<psapi::sfm::RenderWindow*>(renderWindow);

    psapi::sfm::Texture texture;
    psapi::sfm::Sprite sprite;

    sprite.setPosition(pos_.x, pos_.y);
    sprite.setScale(scale_.x, scale_.y);

    for (int i = layers_amount - 1; i >= 0; i--)
    {
        Layer* layer = layers_[i].get();

        texture.update(layer->image_);
        sprite.setTexture(&texture);

        sprite.draw(desktop);
    }
}

bool Canvas::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    mouse_pos_ = psapi::sfm::Mouse::getPosition(renderWindow);

    pressed_ = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);
}

void Canvas::setSize(psapi::sfm::vec2i size)
{
    size_ = size;

    temp_layer_->resize(size.x, size.y);

    for (auto& layer : layers_)
    {
        layer->resize(size.x, size.y);
    }
}

void Canvas::cleanTempLayer()
{
    for (size_t y = 0; y < size_.y; y++)
    {
        for (size_t x = 0; x < size_.x; x++)
        {
            temp_layer_->setPixel({x, y}, psapi::sfm::WHITE);
        }
    }
}

bool Canvas::insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer)
{
    // TODO
}
bool Canvas::removeLayer(size_t index)
{
    // TODO
}
bool Canvas::insertEmptyLayer(size_t index)
{
    //std::make_unique<Layer>(size_.x, size_.y);

    //return insertLayer(index, std::move(temp_layer_));
}


void Canvas::setParent(const psapi::IWindow* parent)
{
    return;
}

void Canvas::forceDeactivate()
{
    is_active_ = false;
}

void Canvas::forceActivate()
{
    is_active_ = true;
}

bool Canvas::isActive() const
{
    return is_active_;
}

psapi::IWindow* Canvas::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? static_cast<psapi::IWindow*>(this) : nullptr;
}
const psapi::IWindow* Canvas::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? static_cast<const psapi::IWindow*>(this) : nullptr;
}
