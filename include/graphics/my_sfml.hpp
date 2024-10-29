#ifndef SFML_HPP
#define SFML_HPP

#include <SFML/Graphics.hpp>

#include "standard/api_sfm.hpp"

namespace psapi
{

namespace sfm
{

class PixelsArray : public IPixelsArray
{
public:
    virtual ~PixelsArray() = default;

    virtual void setColor(const Color &color, size_t ind) override;
    virtual Color getColor(size_t ind) const override;

    virtual void setPosition(const vec2i &coord, size_t ind) override;
    virtual void setPosition(const vec2f &coord, size_t ind) override;
    virtual void setPosition(const vec2d &coord, size_t ind) override;
    virtual void setPosition(int x, int y, size_t ind) override;
    virtual void setPosition(float x, float y, size_t ind) override;
    virtual void setPosition(double x, double y, size_t ind) override;

    virtual void draw(IRenderWindow *window) const override;
};


class Image : public IImage
{
public:
    virtual ~Image() = default;

    virtual void create(unsigned int width, unsigned int height, const Color &color=Color(0, 0, 0)) override;
    virtual void create(vec2u size, const Color &color=Color(0, 0, 0)) override;

    virtual void create(unsigned int width, unsigned int height, const Color *pixels) override;
    virtual void create(vec2u size, const Color *pixels) override;

    virtual bool loadFromFile(const std::string &filename) override;

    virtual vec2u getSize() const override;
    virtual void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    virtual void setPixel(vec2u pos, const Color &color) override;

    virtual Color getPixel(unsigned int x, unsigned int y) const override;
    virtual Color getPixel(vec2u pos) const override;
};

class Texture : public ITexture
{
public:
    virtual ~Texture() = default;

    virtual bool create(unsigned int width, unsigned int height)                                     override;
    virtual bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect()) override;
    virtual bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) override;
    virtual vec2u getSize() const                                                                    override;
    virtual std::unique_ptr<IImage> copyToImage() const                                              override;
    virtual void update(const IImage *image)                                                         override;
    virtual void update(const Color *pixels)                                                         override;
    virtual void update(const Color *pixels, unsigned int width, unsigned int height,
                                             unsigned int x,     unsigned int y) override;
private:
    sf::Texture texture_;

    friend class Sprite;
};

class Sprite : public ISprite
{
public:
    virtual ~Sprite() = default;

    virtual void setTexture(const ITexture *texture, bool reset_rect = false) override;
    virtual void setTextureRect(const IntRect &rectangle) override;

    virtual void setPosition(float x, float y) override;
    virtual void setPosition(const vec2f &pos) override;

    virtual void setScale(float factorX, float factorY) override;
    virtual vec2u getSize() const override;

    virtual void setColor(const Color &color) override;

    virtual void setRotation(float angle) override;

    virtual const vec2f getPosition() const override;
    virtual IntRect getGlobalBounds() const override;

    virtual void draw(IRenderWindow *renderWindow) const override;

private:
    sf::Sprite sprite_;
};

class Font : public IFont
{
public:
    virtual ~Font() = default;

    virtual bool loadFromFile(const std::string& filename) override;
private:
    sf::Font font_;

    friend class Text;
};

class Text : public IText
{
public:
    virtual ~Text() = default;

    virtual void setString(const std::string& string) override;
    virtual void setFont(const IFont* font)           override;
    virtual void setCharacterSize(unsigned int size)  override;
    virtual void setStyle(uint32_t style)             override;
    virtual void setFillColor(const Color* color)     override;
    virtual void setOutlineColor(const Color* color)  override;
    virtual void setOutlineThickness(float thickness) override;

    virtual void draw(IRenderWindow *window) const override;

private:
    sf::Text text_;
};

class RenderWindow : public IRenderWindow
{
public:
    RenderWindow(unsigned int width, unsigned int height, const std::string& name);
    virtual ~RenderWindow() = default;

    virtual bool isOpen() const override;
    virtual void clear()   override;
    virtual void display() override;
    virtual void close()   override;

    virtual vec2u getSize() const override;

    virtual bool pollEvent(Event& event) override;

    virtual void draw(Drawable *target)override;

    virtual void setFps(float fps) override;
    virtual float getFps() const override;

private:
    sf::RenderWindow window_;

    friend class Sprite;
    friend class Text;
};


class Shape : public IShape
{
public:
    virtual ~Shape() = default;
};


class RectangleShape : public IRectangleShape
{
public:
    virtual ~RectangleShape() = default;

    virtual void draw(IRenderWindow *window) const override;

private:
    sf::RectangleShape shape_;
};


class EllipseShape : public IEllipseShape
{
public:
    virtual ~EllipseShape() = default;

    virtual void draw(IRenderWindow *window) const override;
private:
    sf::CircleShape shape_;
};


} // sfm

} // psapi


#endif // SFML_HPP
