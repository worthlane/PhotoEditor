#include "graphics/my_sfml.hpp"

namespace psapi
{

namespace sfm
{

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
bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area)
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

/*void setTexture(const ITexture *texture)
{
    shape_.setTexture(static_cast<const Texture*>(texture)->texture_);
}

void setFillColor(const Color &color)
{
    shape_.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
}

void setPosition(const vec2i &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setPosition(const vec2f &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setPosition(const vec2d &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setScale(const vec2f &scale)
{
    shape_.setScale(scale.x, scale.y);
}

void setSize(const vec2u &size)
{
    shape_.setSize(sf::Vector2f(size.x, size.y));
}
void setRotation(float angle)
{
    shape_.setRotation(angle);
}
void setOutlineColor(const Color &color)
{
    shape_.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
}
void setOutlineThickness(float thickness)
{
    shape_.setOutlineThickness(thickness);
}

float getRotation() const
{
    return shape_.getRotation();
}

vec2f getScale() const
{
    return {shape_.getScale().x, shape_.getScale().y};
}

vec2f getPosition() const
{
    return {shape_.getPosition().x, shape_.getPosition().y};
}
const Color &getFillColor() const
{
    sf::Color color = shape_.getFillColor();

    return {color.r, color.g, color.b, color.a};
}
vec2u getSize() const
{
    sf::Vector2f size = shape_.getSize();

    return {size.x, size.y};
}

float getOutlineThickness() const
{
    return shape_.getOutlineThickness();
}

const Color &getOutlineColor() const
{
    sf::Color color = shape_.getOutlineColor();

    return {color.r, color.g, color.b, color.a};
}

const IImage* getImage() const
{
    // TODO
}

void draw(IRenderWindow *window) const
{
    // TODO implement
}

// *************************************************************************
//                          ELLIPSE SHAPE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

void setTexture(const ITexture *texture)
{
    shape_.setTexture(static_cast<const Texture*>(texture)->texture_);
}

void setFillColor(const Color &color)
{
    shape_.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
}

void setPosition(const vec2i &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setPosition(const vec2f &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setPosition(const vec2d &pos)
{
    shape_.setPosition(pos.x, pos.y);
}

void setScale(const vec2f &scale)
{
    shape_.setScale(scale.x, scale.y);
}

void setSize(const vec2u &size)
{
    shape_.setSize(sf::Vector2f(size.x, size.y));
}
void setRotation(float angle)
{
    shape_.setRotation(angle);
}
void setOutlineColor(const Color &color)
{
    shape_.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
}
void setOutlineThickness(float thickness)
{
    shape_.setOutlineThickness(thickness);
}

float getRotation() const
{
    return shape_.getRotation();
}

vec2f getScale() const
{
    return {shape_.getScale().x, shape_.getScale().y};
}

vec2f getPosition() const
{
    return {shape_.getPosition().x, shape_.getPosition().y};
}
const Color &getFillColor() const
{
    sf::Color color = shape_.getFillColor();

    return {color.r, color.g, color.b, color.a};
}
vec2u getSize() const
{
    sf::Vector2f size = shape_.getSize();

    return {size.x, size.y};
}

float getOutlineThickness() const
{
    return shape_.getOutlineThickness();
}

const Color &getOutlineColor() const
{
    sf::Color color = shape_.getOutlineColor();

    return {color.r, color.g, color.b, color.a};
}

const IImage* getImage() const
{
    // TODO
}

void draw(IRenderWindow *window) const
{
    // TODO implement
}*/

} // sfm

} // psapi
