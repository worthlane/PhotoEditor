#include "graphics/my_sfml.hpp"

namespace psapi
{

namespace sfm
{

static const vec2u SCREEN = {1280, 960};

// *************************************************************************
//                              SPRITE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void Sprite::setTexture(const ITexture *texture, bool reset_rect)
{
    const Texture* tex = static_cast<const Texture*>(texture);
    sprite_.setTexture(tex->texture_, reset_rect);
}

void Sprite::setTextureRect(const IntRect &rectangle)
{
    sprite_.setTextureRect(sf::IntRect(rectangle.top_x, rectangle.top_y, rectangle.width, rectangle.height));
}

void Sprite::setPosition(float x, float y)
{
    sprite_.setPosition(x, y);
}

void Sprite::setPosition(const vec2f &pos)
{
    sprite_.setPosition(pos.x, pos.y);
}

void Sprite::setScale(float factorX, float factorY)
{
    sprite_.setScale(factorX, factorY);
}

vec2u Sprite::getSize() const
{
    // TODO implement
}

void Sprite::setColor(const Color &color)
{
    sprite_.setColor(sf::Color(color.r, color.g, color.b, color.a));
}

void Sprite::setRotation(float angle)
{
    sprite_.setRotation(angle);
}

const vec2f Sprite::getPosition() const
{
    sf::Vector2f vec = sprite_.getPosition();

    return {vec.x, vec.y};
}

IntRect Sprite::getGlobalBounds() const
{
    sf::FloatRect rect = sprite_.getGlobalBounds();

    return { static_cast<int>(rect.left),  static_cast<int>(rect.top),
             static_cast<int>(rect.width), static_cast<int>(rect.height) };
}

void Sprite::draw(IRenderWindow *window) const
{
    RenderWindow* desktop = static_cast<RenderWindow*>(window);
    desktop->window_.draw(sprite_);
}

std::unique_ptr<ISprite> ISprite::create()
{
    return std::make_unique<Sprite>();
}

// *************************************************************************
//                              RENDERWINDOW
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

RenderWindow::RenderWindow(unsigned int length, unsigned int width, const std::string& title) :
      window_(sf::VideoMode(length, width), title)
{}

bool RenderWindow::isOpen() const
{
    return window_.isOpen();
}

void RenderWindow::clear()
{
    window_.clear();
}

void RenderWindow::display()
{
    window_.display();
}

void RenderWindow::close()
{
    window_.close();
}

vec2u RenderWindow::getSize() const
{
    return {static_cast<unsigned int>(window_.getSize().x), static_cast<unsigned int>(window_.getSize().y)};
}

bool RenderWindow::pollEvent(Event& event)
{
    sf::Event sfml_event;

    if (!window_.pollEvent(sfml_event))
        return false;

    switch (sfml_event.type)
    {
        case sf::Event::Closed:
            event.type = Event::Closed;
            break;

        case sf::Event::Resized:
            event.type = Event::Resized;
            event.size.width  = sfml_event.size.width;
            event.size.height = sfml_event.size.height;
            break;

        case sf::Event::LostFocus:
            event.type = Event::LostFocus;
            break;

        case sf::Event::GainedFocus:
            event.type = Event::GainedFocus;
            break;

        case sf::Event::TextEntered:
            event.type         = Event::TextEntered;
            event.text.unicode = sfml_event.text.unicode;
            break;

        case sf::Event::KeyPressed:
            event.type        = Event::KeyPressed;
            event.key.code    = static_cast<Keyboard::Key>(sfml_event.key.code);
            event.key.alt     = sfml_event.key.alt;
            event.key.control = sfml_event.key.control;
            event.key.shift   = sfml_event.key.shift;
            event.key.system  = sfml_event.key.system;
            break;

        case sf::Event::KeyReleased:
            event.type        = Event::KeyReleased;
            event.key.code    = static_cast<Keyboard::Key>(sfml_event.key.code);
            event.key.alt     = sfml_event.key.alt;
            event.key.control = sfml_event.key.control;
            event.key.shift   = sfml_event.key.shift;
            event.key.system  = sfml_event.key.system;
            break;

        case sf::Event::MouseWheelScrolled:
            event.type             = Event::MouseWheelScrolled;
            event.mouseWheel.delta = sfml_event.mouseWheelScroll.delta;
            event.mouseWheel.x     = sfml_event.mouseWheelScroll.x;
            event.mouseWheel.y     = sfml_event.mouseWheelScroll.y;
            event.mouseWheel.wheel = static_cast<Mouse::Wheel>(sfml_event.mouseWheelScroll.wheel);
            break;

        case sf::Event::MouseButtonPressed:
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = static_cast<Mouse::Button>(sfml_event.mouseButton.button);
            event.mouseButton.x      = sfml_event.mouseButton.x;
            event.mouseButton.y      = sfml_event.mouseButton.y;
            break;

        case sf::Event::MouseButtonReleased:
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = static_cast<Mouse::Button>(sfml_event.mouseButton.button);
            event.mouseButton.x      = sfml_event.mouseButton.x;
            event.mouseButton.y      = sfml_event.mouseButton.y;
            break;

        case sf::Event::MouseMoved:
            event.type        = Event::MouseMoved;
            event.mouseMove.x = sfml_event.mouseMove.x;
            event.mouseMove.y = sfml_event.mouseMove.y;
            break;

        case sf::Event::MouseEntered:
            event.type = Event::MouseEntered;
            break;

        case sf::Event::MouseLeft:
            event.type = Event::MouseLeft;
            break;

        default:
            event.type = Event::Unknown;
            break;
    }

    return true;
}

void RenderWindow::draw(Drawable *target)
{
    if (target)
    {
        target->draw(this);
    }
}

void RenderWindow::setFps(float fps)
{
    // TODO implement
}

float RenderWindow::getFps() const
{
    // TODO implement
}

std::unique_ptr<IRenderWindow> IRenderWindow::create(unsigned int width, unsigned int height, const std::string& name)
{
    return std::make_unique<RenderWindow>(width, height, name);
}

// *************************************************************************
//                              TEXT
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void Text::setString(const std::string& string)
{
    text_.setString(string);
}

void Text::setFont(const IFont* font)
{
    const Font* my_font = static_cast<const Font*>(font);

    text_.setFont(my_font->font_);
}

void Text::setCharacterSize(unsigned int size)
{
    text_.setCharacterSize(size);
}

void Text::setStyle(uint32_t style)
{
    text_.setStyle(style);
}

void Text::setFillColor(const Color* color)
{
    text_.setFillColor(sf::Color(color->r, color->g, color->b, color->a));
}

void Text::setOutlineColor(const Color* color)
{
    text_.setOutlineColor(sf::Color(color->r, color->g, color->b, color->a));
}

void Text::setOutlineThickness(float thickness)
{
    text_.setOutlineThickness(thickness);
}

void Text::draw(IRenderWindow *window) const
{
    RenderWindow* desktop = static_cast<RenderWindow*>(window);
    desktop->window_.draw(text_);
}

std::unique_ptr<IText> IText::create()
{
    return std::make_unique<Text>();
}

// *************************************************************************
//                              FONT
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

bool Font::loadFromFile(const std::string& filename)
{
    return font_.loadFromFile(filename);
}

std::unique_ptr<IFont> IFont::create()
{
    return std::make_unique<Font>();
}

// *************************************************************************
//                          PIXELS ARRAY
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void PixelsArray::setColor(const Color &color, size_t ind)
{
    // TODO implement
}

Color PixelsArray::getColor(size_t ind) const
{
    // TODO implement
}

void PixelsArray::setPosition(const vec2i &coord, size_t ind)
{
    // TODO implement
}

void PixelsArray::setPosition(const vec2f &coord, size_t ind)
{
    // TODO implement
}

void PixelsArray::setPosition(const vec2d &coord, size_t ind)
{
    // TODO implement
}

void PixelsArray::setPosition(int x, int y, size_t ind)
{
    // TODO implement
}

void PixelsArray::setPosition(float x, float y, size_t ind)
{
    // TODO implement
}

void PixelsArray::setPosition(double x, double y, size_t ind)
{
    // TODO implement
}

void PixelsArray::draw(IRenderWindow *window) const
{
    // TODO implement
}

// *************************************************************************
//                          IMAGE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void Image::create(unsigned int width, unsigned int height, const Color &color)
{
    image_.create(width, height, sf::Color(color.r, color.g, color.b, color.a));
}
void Image::create(vec2u size, const Color &color)
{
    image_.create(size.x, size.y, sf::Color(color.r, color.g, color.b, color.a));
}

void Image::create(unsigned int width, unsigned int height, const Color *pixels)
{
    image_.create(width, height, sf::Color(0, 0, 0, 0));

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            image_.setPixel(x, y, sf::Color(pixels[y * width + x].r, pixels[y * width + x].g,
                                            pixels[y * width + x].b, pixels[y * width + x].a));
        }
    }
}
void Image::create(vec2u size, const Color *pixels)
{
    create(size.x, size.y, pixels);
}

bool Image::loadFromFile(const std::string &filename)
{
    return image_.loadFromFile(filename);
}

vec2u Image::getSize() const
{
    sf::Vector2u size = image_.getSize();

    return {size.x, size.y};
}
void Image::setPixel(unsigned int x, unsigned int y, const Color &color)
{
    image_.setPixel(x, y, sf::Color(color.r, color.g, color.b, color.a));
}

void Image::setPixel(vec2u pos, const Color &color)
{
    setPixel(pos.x, pos.y, color);
}

Color Image::getPixel(unsigned int x, unsigned int y) const
{
    sf::Color color = image_.getPixel(x, y);

    return {color.r, color.g, color.b, color.a};
}
Color Image::getPixel(vec2u pos) const
{
    return getPixel(pos.x, pos.y);
}

std::unique_ptr<IImage> IImage::create()
{
    return std::make_unique<Image>();
}

// *************************************************************************
//                          TEXTURE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

bool Texture::create(unsigned int width, unsigned int height)
{
    return texture_.create(width, height);
}
bool Texture::loadFromFile(const std::string& filename, const IntRect& area)
{
    return texture_.loadFromFile(filename);
}
bool Texture::loadFromMemory(const void* data, size_t size, const IntRect& area)
{
    return texture_.loadFromMemory(data, size);
}
vec2u Texture::getSize() const
{
    sf::Vector2u size = texture_.getSize();

    return {size.x, size.y};
}
std::unique_ptr<IImage> Texture::copyToImage() const
{
    sf::Image sf_image = texture_.copyToImage();

    auto unique_img = std::make_unique<Image>();
    unique_img->image_ = std::move(sf_image);

    return unique_img;
}

void Texture::update(const IImage *image)
{
    const Image* img = static_cast<const Image*>(image);

    texture_.loadFromImage(img->image_);
}
void Texture::update(const Color *pixels)
{
    vec2u size = getSize();

    Image img;
    img.create(size, pixels);
    update(&img);
}
void Texture::update(const Color *pixels, unsigned int width, unsigned int height,
                                          unsigned int x,     unsigned int y)
{
    // TODO implement
}

std::unique_ptr<ITexture> ITexture::create()
{
    return std::make_unique<Texture>();
}

// *************************************************************************
//                          MOUSE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

bool Mouse::isButtonPressed(Button button)
{
    return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button));
}

vec2i Mouse::getPosition()
{
    sf::Vector2i position = sf::Mouse::getPosition();

    return vec2i(position.x, position.y);
}

vec2i Mouse::getPosition(const IRenderWindow* relative_to)
{
    auto window = dynamic_cast<const RenderWindow*>(relative_to);

    if (window)
    {
        sf::Vector2i position = sf::Mouse::getPosition(window->window_);
        return vec2i(position.x, position.y);
    }

    return vec2i(0, 0);
}

void Mouse::setPosition(const vec2i& position)
{
    sf::Mouse::setPosition(sf::Vector2i(position.x, position.y));
}

void Mouse::setPosition(const vec2i& position, const IRenderWindow* relative_to)
{
    auto window = dynamic_cast<const RenderWindow*>(relative_to);

    if (window)
    {
        sf::Mouse::setPosition(sf::Vector2i(position.x, position.y), window->window_);
    }
}

// *************************************************************************
//                          RECTANGLE SHAPE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void RectangleShape::setTexture(const ITexture *texture)
{
    const Texture* tex = static_cast<const Texture*>(texture);

    shape_.setTexture(&(tex->texture_));
}

void RectangleShape::setFillColor(const Color &color)
{
    shape_.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
}

void RectangleShape::setPosition(const vec2i &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void RectangleShape::setPosition(const vec2f &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void RectangleShape::setPosition(const vec2d &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void RectangleShape::setScale(const vec2f &scale)
{
    shape_.setScale(scale.x, scale.y);
}

void RectangleShape::setSize(const vec2u &size)
{
    shape_.setSize(sf::Vector2f(size.x, size.y));
}
void RectangleShape::setRotation(float angle)
{
    shape_.setRotation(angle);
}
void RectangleShape::setOutlineColor(const Color &color)
{
    shape_.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
}
void RectangleShape::setOutlineThickness(float thickness)
{
    shape_.setOutlineThickness(thickness);
}

float RectangleShape::getRotation() const
{
    return shape_.getRotation();
}

vec2f RectangleShape::getScale() const
{
    return {shape_.getScale().x, shape_.getScale().y};
}

vec2f RectangleShape::getPosition() const
{
    return {shape_.getPosition().x, shape_.getPosition().y};
}
const Color& RectangleShape::getFillColor() const
{
    sf::Color color = shape_.getFillColor();

    return {color.r, color.g, color.b, color.a};
}
vec2u RectangleShape::getSize() const
{
    sf::Vector2f size = shape_.getSize();

    return {size.x, size.y};
}

float RectangleShape::getOutlineThickness() const
{
    return shape_.getOutlineThickness();
}

const Color& RectangleShape::getOutlineColor() const
{
    sf::Color color = shape_.getOutlineColor();

    return {color.r, color.g, color.b, color.a};
}

const IImage* RectangleShape::getImage() const
{
    vec2u size = getSize();

    if (size.x <= 0 || size.y <= 0)
        return cached_image_.get();

    sf::RenderTexture render_texture;
    if (!render_texture.create(SCREEN.x, SCREEN.y))
        return nullptr;

    render_texture.clear(sf::Color::Transparent);
    render_texture.draw(shape_);
    render_texture.display();

    sf::Image image = render_texture.getTexture().copyToImage();

    cached_image_->create(image.getSize().x, image.getSize().y,
                        reinterpret_cast<const Color*>(image.getPixelsPtr()));

    return cached_image_.get();
}

void RectangleShape::draw(IRenderWindow *window) const
{
    // TODO implement
}

std::unique_ptr<IRectangleShape> IRectangleShape::create(unsigned int width, unsigned int height)
{
    return std::make_unique<RectangleShape>(width, height);
}
std::unique_ptr<IRectangleShape> IRectangleShape::create(const vec2u& size)
{
    return std::make_unique<RectangleShape>(size.x, size.y);
}

RectangleShape::RectangleShape(unsigned int width, unsigned int height)
{
    shape_.setSize(sf::Vector2f(width, height));
    cached_image_ = std::make_unique<Image>();
}

void RectangleShape::move(const vec2f &offset)
{
    // TODO
}

// *************************************************************************
//                          ELLIPSE SHAPE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

EllipseShape::EllipseShape(unsigned int width, unsigned int height)
{
    // TODO
    shape_.setRadius(width);

    psapi::sfm::vec2f scale = psapi::sfm::vec2f(1, static_cast<float>(height) / static_cast<float>(width));

    shape_.setScale(sf::Vector2f(scale.x, scale.y));
    cached_image_ = std::make_unique<Image>();
}


std::unique_ptr<IEllipseShape> IEllipseShape::create(unsigned int width, unsigned int height)
{
    return std::make_unique<EllipseShape>(width, height);
}

std::unique_ptr<IEllipseShape> IEllipseShape::create(const vec2u &size)
{
    return std::make_unique<EllipseShape>(size.x, size.y);
}
std::unique_ptr<IEllipseShape> IEllipseShape::create(unsigned int radius)
{
    return std::make_unique<EllipseShape>(radius, radius);
}

void EllipseShape::setTexture(const ITexture *texture)
{
    const Texture* tex = static_cast<const Texture*>(texture);

    shape_.setTexture(&(tex->texture_));
}

void EllipseShape::setFillColor(const Color &color)
{
    shape_.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
}

void EllipseShape::setPosition(const vec2i &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void EllipseShape::setPosition(const vec2f &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void EllipseShape::setPosition(const vec2d &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void EllipseShape::setScale(const vec2f &scale)
{
    shape_.setScale(scale.x, scale.y);
}

void EllipseShape::setSize(const vec2u &size)
{
    // TODO
    shape_.setRadius(size.x);

    psapi::sfm::vec2f scale = getScale();

    scale = psapi::sfm::vec2f(1, static_cast<float>(size.y) / static_cast<float>(size.x)) * scale;

    shape_.setScale(sf::Vector2f(scale.x, scale.y));
}
void EllipseShape::setRotation(float angle)
{
    shape_.setRotation(angle);
}
void EllipseShape::setOutlineColor(const Color &color)
{
    shape_.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
}
void EllipseShape::setOutlineThickness(float thickness)
{
    shape_.setOutlineThickness(thickness);
}

float EllipseShape::getRotation() const
{
    return shape_.getRotation();
}

vec2f EllipseShape::getScale() const
{
    return {shape_.getScale().x, shape_.getScale().y};
}

vec2f EllipseShape::getPosition() const
{
    return {shape_.getPosition().x, shape_.getPosition().y};
}
const Color& EllipseShape::getFillColor() const
{
    sf::Color color = shape_.getFillColor();

    return {color.r, color.g, color.b, color.a};
}
vec2u EllipseShape::getSize() const
{
    // TODO
    psapi::sfm::vec2f scale = getScale();

    return {static_cast<unsigned int>(shape_.getRadius() * scale.x), static_cast<unsigned int>(shape_.getRadius() * scale.y)};
}

float EllipseShape::getOutlineThickness() const
{
    return shape_.getOutlineThickness();
}

const Color& EllipseShape::getOutlineColor() const
{
    sf::Color color = shape_.getOutlineColor();

    return {color.r, color.g, color.b, color.a};
}

const IImage* EllipseShape::getImage() const
{
    vec2u size = getSize();

    if (size.x <= 0 || size.y <= 0)
        return cached_image_.get();

    sf::RenderTexture render_texture;
    if (!render_texture.create(SCREEN.x, SCREEN.y))
        return nullptr;

    render_texture.clear(sf::Color::Transparent);
    render_texture.draw(shape_);
    render_texture.display();

    sf::Image image = render_texture.getTexture().copyToImage();

    cached_image_->create(image.getSize().x, image.getSize().y,
                        reinterpret_cast<const Color*>(image.getPixelsPtr()));

    return cached_image_.get();
}

void EllipseShape::draw(IRenderWindow *window) const
{
    // TODO implement
}

void EllipseShape::move(const vec2f &offset)
{
    // TODO
}

} // sfm

} // psapi
