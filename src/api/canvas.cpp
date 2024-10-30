#include "api/canvas.hpp"

#include "graphics/my_sfml.hpp"

namespace psapi
{

// ******************* LAYER ******************

SimpleLayer::SimpleLayer(const size_t width, const size_t height)
{
    image_.create(width, height);
}

SimpleLayer::SimpleLayer(const size_t width, const size_t height, const sfm::Color& color)
{
    image_.create(width, height, color);
}

sfm::Color SimpleLayer::getPixel(sfm::vec2i pos) const
{
    return image_.getPixel(pos.x, pos.y);
}

void SimpleLayer::setPixel(sfm::vec2i pos, sfm::Color pixel)
{
    image_.setPixel(pos.x, pos.y, pixel);
}

void SimpleLayer::resize(const size_t width, const size_t height)
{
    // TODO
}

// ****************** CANVAS *******************

SimpleCanvas::SimpleCanvas(sfm::vec2i pos,sfm::vec2i size, sfm::vec2f scale) :
    pos_(pos), size_(size), scale_(scale),
    layers_(), temp_layer_(std::make_unique<SimpleLayer>(size_.x, size_.y)),
    mouse_pos_(sfm::vec2i(0, 0)), pressed_(false)
{
    layers_.push_back(std::make_unique<SimpleLayer>(size_.x, size_.y, sfm::WHITE));
}

ILayer* SimpleCanvas::getLayer(size_t index)
{
    return layers_[index].get();
}
const ILayer* SimpleCanvas::getLayer(size_t index) const
{
    return layers_[index].get();
}

ILayer* SimpleCanvas::getTempLayer()
{
    return temp_layer_.get();
}
const ILayer* SimpleCanvas::getTempLayer() const
{
    return temp_layer_.get();
}

size_t SimpleCanvas::getActiveLayerIndex() const
{
    return active_;
}

void SimpleCanvas::setActiveLayerIndex(size_t index)
{
    active_ = index;
}

void SimpleCanvas::draw(IRenderWindow* renderWindow)
{
    size_t layers_amount = layers_.size();

    sfm::RenderWindow* desktop = static_cast<sfm::RenderWindow*>(renderWindow);

    sfm::Texture texture;
    sfm::Sprite sprite;

    sprite.setPosition(pos_.x, pos_.y);
    sprite.setScale(scale_.x, scale_.y);

    for (int i = layers_amount - 1; i >= 0; i--)
    {
        SimpleLayer* layer = layers_[i].get();

        texture.update(&(layer->image_));
        sprite.setTexture(&texture);

        sprite.draw(desktop);
    }
}

bool SimpleCanvas::update(const IRenderWindow* renderWindow, const Event& event)
{
    mouse_pos_ = sfm::Mouse::getPosition(renderWindow);

    pressed_ = sfm::Mouse::isButtonPressed(sfm::Mouse::Button::Left);
}

void SimpleCanvas::setSize(sfm::vec2i size)
{
    size_ = size;

    temp_layer_->resize(size.x, size.y);

    for (auto& layer : layers_)
    {
        layer->resize(size.x, size.y);
    }
}

void SimpleCanvas::cleanTempLayer()
{
    for (size_t y = 0; y < size_.y; y++)
    {
        for (size_t x = 0; x < size_.x; x++)
        {
            temp_layer_->setPixel({x, y}, sfm::WHITE);
        }
    }
}

bool SimpleCanvas::insertLayer(size_t index, std::unique_ptr<ILayer> layer)
{
    // TODO
}
bool SimpleCanvas::removeLayer(size_t index)
{
    // TODO
}
bool SimpleCanvas::insertEmptyLayer(size_t index)
{
    /*std::make_unique<SimpleLayer>(size_.x, size_.y);

    return insertLayer(index, std::move(temp_layer_));*/
}

}   // namespace psapi
