#include "api/canvas.hpp"

namespace psapi
{

// ******************* LAYER ******************

Layer::Layer(const size_t width, const size_t height)
{
    image_.create(width, height);
}

Layer::Layer(const size_t width, const size_t height, const sfm::Color& color)
{
    image_.create(width, height, color);
}

sfm::Color Layer::getPixel(sfm::vec2i pos) const
{
    return image_.getPixel(pos.x, pos.y);
}

void Layer::setPixel(sfm::vec2i pos, sfm::Color pixel)
{
    image_.setPixel(pos.x, pos.y, pixel);
}

void Layer::resize(const size_t width, const size_t height)
{
    // TODO
}

// ****************** CANVAS *******************

Canvas::Canvas(sfm::vec2i pos,sfm::vec2i size, sfm::vec2f scale) :
    pos_(pos), size_(size), scale_(scale),
    layers_(), temp_layer_(std::make_unique<Layer>(size_.x, size_.y)),
    mouse_pos_(sfm::vec2i(0, 0)), pressed_(false)
{
    layers_.push_back(std::make_unique<Layer>(size_.x, size_.y, sfm::WHITE));
}

ILayer* Canvas::getLayer(size_t index)
{
    return layers_[index].get();
}
const ILayer* Canvas::getLayer(size_t index) const
{
    return layers_[index].get();
}

ILayer* Canvas::getTempLayer()
{
    return temp_layer_.get();
}
const ILayer* Canvas::getTempLayer() const
{
    return temp_layer_.get();
}

size_t Canvas::getActiveLayerIndex() const
{
    return active_;
}

void Canvas::setActiveLayerIndex(size_t index)
{
    active_ = index;
}

void Canvas::draw(IRenderWindow* renderWindow)
{
    // TODO
}

bool Canvas::update(const IRenderWindow* renderWindow, const Event& event)
{
    // TODO
}

void Canvas::setSize(sfm::vec2i size)
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
            temp_layer_->setPixel({x, y}, sfm::WHITE);
        }
    }
}

bool Canvas::insertLayer(size_t index, std::unique_ptr<ILayer> layer)
{
    // TODO
}
bool Canvas::removeLayer(size_t index)
{
    // TODO
}
bool Canvas::insertEmptyLayer(size_t index)
{
    /*std::make_unique<Layer>(size_.x, size_.y);

    return insertLayer(index, std::move(temp_layer_));*/
}

}   // namespace psapi
