#include <cassert>
#include <iostream>

#include "../plugins/colors.hpp"
#include "../plugins/canvas.hpp"

static psapi::sfm::vec2i operator/(const psapi::sfm::vec2i& self, const psapi::sfm::vec2f& other)
{
    return {static_cast<int>(static_cast<double>(self.x) / other.x),
            static_cast<int>(static_cast<double>(self.y) / other.y)};
}

static psapi::sfm::vec2i operator*(const psapi::sfm::vec2i& self, const psapi::sfm::vec2f& other)
{
    return {static_cast<int>(static_cast<double>(self.x) * other.x),
            static_cast<int>(static_cast<double>(self.y) * other.y)};
}


bool loadPlugin()
{
    std::cout << "canvas loaded\n";

    auto canvas = std::make_unique<Canvas>(psapi::sfm::vec2i(128, 20),
                                           psapi::sfm::vec2i(1072, 780),
                                           psapi::sfm::vec2f(1, 1));

    auto root = psapi::getRootWindow();

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
    image_->create(width, height, sfm::WHITE);
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
    mouse_pos_(psapi::sfm::vec2i(0, 0)), pressed_(false), coord_start_(0, 0)
{
    pos_ = {pos.x, pos.y};
    size_ = {size.x, size.y};
    scale_ = {scale.x, scale.y};

    layers_.push_back(std::make_unique<Layer>(size_.x, size_.y, sfm::WHITE));
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

    psapi::sfm::IImage* final_image = psapi::sfm::IImage::create().release();
    psapi::sfm::ITexture* texture = psapi::sfm::ITexture::create().release();
    psapi::sfm::ISprite* sprite = psapi::sfm::ISprite::create().release();

    final_image->create(size_.x, size_.y, sfm::WHITE);

    sprite->setPosition(pos_.x, pos_.y);
    //sprite->setScale(scale_.x, scale_.y);

    for (int i = layers_amount - 1; i >= 0; i--)
    {
        Layer* layer = layers_[i].get();

        for (size_t y = 0; y < size_.y; y++)
        {
            for (size_t x = 0; x < size_.x; x++)
            {
                //psapi::sfm::vec2i coord = {x / scale_.x, y / scale_.y};
                final_image->setPixel({x, y}, layer->getPixel(coord_start_ + psapi::sfm::vec2i(x, y) / scale_));
            }
        }
    }

    texture->update(final_image);
    sprite->setTexture(texture);

    sprite->draw(renderWindow);

    delete texture;
    delete sprite;
    delete final_image;
}

bool Canvas::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow) - pos_;

    if (event.type == psapi::sfm::Event::MouseWheelScrolled)
    {
        /*if (event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Vertical)
        {
            std::cout << "----------------------------------\n";
        }

        if (event.mouseWheel.wheel == psapi::sfm::Mouse::Wheel::Horizontal)
        {
            std::cout << "************************************\n";
        }*/

        psapi::sfm::vec2i old_mouse_pos = mouse_pos / scale_;

        scale_ += psapi::sfm::vec2f(0.1f * scale_.x, 0.1f * scale_.y) * event.mouseWheel.delta;

        if (scale_.x < 1)   scale_.x = 1;
        if (scale_.y < 1)   scale_.y = 1;

        psapi::sfm::vec2i new_mouse_pos = mouse_pos / scale_;

        coord_start_ += old_mouse_pos - new_mouse_pos;

        psapi::sfm::vec2i right_lower_corner = coord_start_ + psapi::sfm::vec2i(size_.x, size_.y) / scale_;

        if (right_lower_corner.x > size_.x - 1) coord_start_.x = size_.x - (size_.x / scale_.x);
        if (right_lower_corner.y > size_.y - 1) coord_start_.y = size_.y - (size_.y / scale_.y);
        if (coord_start_.x < 0) coord_start_.x = 0;
        if (coord_start_.y < 0) coord_start_.y = 0;
    }

    mouse_pos_ = coord_start_ + mouse_pos / scale_;

    return true;
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
            temp_layer_->setPixel({x, y}, sfm::WHITE);
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
