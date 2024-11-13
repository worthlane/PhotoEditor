#include <cassert>
#include <iostream>

#include "../plugins/colors.hpp"
#include "../plugins/canvas.hpp"


static const char* BACKGROUND_TEXTURE = "assets/textures/light_gray.jpg";
static const char* NORMAL_TEXTURE = "assets/textures/mid_gray.jpg";
static const char* HOVER_TEXTURE = "assets/textures/gray.jpg";
static const char* ACTIVE_TEXTURE = "assets/textures/dark_gray.jpg";

static const psapi::wid_t kCanvasHorizontalScrollBar = 230;
static const psapi::wid_t kCanvasVerticalScrollBar = 231;

bool loadPlugin()
{
    std::cout << "canvas loaded\n";

    auto canvas = std::make_unique<Canvas>(psapi::sfm::vec2i(128, 20),
                                           psapi::sfm::vec2i(1052, 760),
                                           psapi::sfm::vec2f(1, 1));

    std::unique_ptr<psapi::sfm::ITexture> back_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> normal_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> active_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> hover_hor = psapi::sfm::ITexture::create();

    back_hor->loadFromFile(BACKGROUND_TEXTURE);
    normal_hor->loadFromFile(NORMAL_TEXTURE);
    active_hor->loadFromFile(ACTIVE_TEXTURE);
    hover_hor->loadFromFile(HOVER_TEXTURE);


    auto hor_scrollbar = std::make_unique<HorizontalScrollBar>(kCanvasHorizontalScrollBar,
                                                                psapi::sfm::vec2i(128, 780),
                                                                psapi::sfm::vec2u(1052, 20),
                                                                std::move(back_hor), std::move(normal_hor),
                                                                std::move(hover_hor), std::move(active_hor),
                                                                canvas.get());

    std::unique_ptr<psapi::sfm::ITexture> back_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> normal_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> active_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> hover_ver = psapi::sfm::ITexture::create();

    back_ver->loadFromFile(BACKGROUND_TEXTURE);
    normal_ver->loadFromFile(NORMAL_TEXTURE);
    active_ver->loadFromFile(ACTIVE_TEXTURE);
    hover_ver->loadFromFile(HOVER_TEXTURE);

    auto ver_scrollbar = std::make_unique<VerticalScrollBar>(kCanvasVerticalScrollBar,
                                                           psapi::sfm::vec2i(1180, 20),
                                                           psapi::sfm::vec2u(20, 760),
                                                           std::move(back_ver), std::move(normal_ver),
                                                           std::move(hover_ver), std::move(active_ver),
                                                           canvas.get());

    auto root = psapi::getRootWindow();

    root->addWindow(std::move(canvas));
    root->addWindow(std::move(hor_scrollbar));
    root->addWindow(std::move(ver_scrollbar));

    return true;
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

void Layer::dumpOnImage(psapi::sfm::IImage* image, const psapi::sfm::vec2i& coord_start, const psapi::sfm::vec2f& scale, const psapi::sfm::vec2i& size)
{
    for (size_t y = 0; y < size.y; y++)
    {
        for (size_t x = 0; x < size.x; x++)
        {
            psapi::sfm::Color pixel = getPixel(coord_start + psapi::sfm::vec2i(x, y) / scale);

            if (pixel.a == 0) continue;

            image->setPixel({x, y}, pixel);
        }
    }
}

// ****************** CANVAS *******************

Canvas::Canvas(const psapi::sfm::vec2i& pos,
               const psapi::sfm::vec2i& size,
               const psapi::sfm::vec2f& scale) :
    layers_(),
    mouse_pos_(psapi::sfm::vec2i(0, 0)), pressed_(false)
{
    pos_ = {pos.x, pos.y};
    size_ = {size.x, size.y};

    setScale({scale.x, scale.y});

    temp_layer_ = std::make_unique<Layer>(size_.x, size_.y, psapi::sfm::Color(0, 0, 0, 0));

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

    psapi::sfm::vec2f scale = getScale();
    psapi::sfm::vec2i coord_start = getCoordStart();

    for (int i = layers_amount - 1; i >= 0; i--)
    {
        Layer* layer = layers_[i].get();
        layer->dumpOnImage(final_image, coord_start, scale, size_);
    }

    Layer* temp_layer = temp_layer_.get();
    temp_layer->dumpOnImage(final_image, coord_start, scale, size_);

    texture->update(final_image);
    sprite->setTexture(texture);

    sprite->draw(renderWindow);

    delete texture;
    delete sprite;
    delete final_image;
}

bool Canvas::update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return updateScale(renderWindow, event);
}

bool Canvas::updateScale(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::sfm::vec2i mouse_pos = psapi::sfm::Mouse::getPosition(renderWindow) - pos_;
    psapi::sfm::vec2f scale = getScale();
    psapi::sfm::vec2i coord_start = getCoordStart();

    if (event.type == psapi::sfm::Event::MouseWheelScrolled)
    {
        psapi::sfm::vec2i old_mouse_pos = mouse_pos / scale;

        scale += psapi::sfm::vec2f(0.05f * scale.x, 0.05f * scale.y) * event.mouseWheel.delta;

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

void Canvas::setSize(psapi::sfm::vec2i size)
{
    size_ = size;
}

void Canvas::cleanTempLayer()
{
    Layer* active_layer = layers_[active_layer_].get();
    Layer* temp_layer = temp_layer_.get();

    for (size_t y = 0; y < size_.y; y++)
    {
        for (size_t x = 0; x < size_.x; x++)
        {
            psapi::sfm::Color temp_pixel = temp_layer->getPixel({x, y});

            if (temp_pixel.a == 0)
                continue;

            active_layer->setPixel({x, y}, temp_pixel);
            temp_layer->setPixel({x, y}, psapi::sfm::Color(0, 0, 0, 0));
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
