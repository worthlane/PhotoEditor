#ifndef MY_SFML_HPP
#define MY_SFML_HPP

#include <SFML/Graphics.hpp>

#include "api/api_sfm.hpp"

namespace psapi
{

namespace sfm
{

static const Color WHITE = Color(255, 255, 255);
static const Color BLACK = Color(0, 0, 0);

static const Color RED   = Color(255, 0, 0);
static const Color GREEN = Color(0, 255, 0);
static const Color BLUE  = Color(0, 0, 255);

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

    bool saveToFile(const std::string &filename) const;

    virtual void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    virtual void setPixel(vec2u pos, const Color &color) override;

    virtual Color getPixel(unsigned int x, unsigned int y) const override;
    virtual Color getPixel(vec2u pos) const override;

    virtual vec2u getSize() const override;

    virtual vec2i getPos() const override;
    virtual void setPos(const vec2i &pos) override;

private:
    sf::Image image_;

    vec2i pos_ = {0, 0};

    friend class Texture;

    friend class RectangleShape;
    friend class EllipseShape;
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

    friend class RectangleShape;
    friend class EllipseShape;
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
    virtual Color getColor() const override;

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

    virtual void setPos(const vec2f &pos) override;
    virtual void setSize(const vec2f &size) override;

    virtual IntRect getGlobalBounds() const override;

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

    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window_;

    friend class Sprite;
    friend class Text;
    friend class Mouse;
};


class Shape : public IShape
{
public:
    virtual ~Shape() = default;

protected:

    std::unique_ptr<Image> cached_image_;
    /*vec2i pos_;
    vec2u size_;
    vec2f scale_;

    Color color_;
    Color outline_color_;

    std::unique_ptr<IImage> image_;*/
};


class RectangleShape : public IRectangleShape, public Shape
{
public:
    RectangleShape(unsigned int width, unsigned int height);
    virtual ~RectangleShape() = default;

    virtual void setTexture(const ITexture *texture) override;
    virtual void setFillColor(const Color &color) override;

    virtual void setPosition(const vec2i &pos) override;
    virtual void setPosition(const vec2f &pos) override;
    virtual void setPosition(const vec2d &pos) override;
    virtual void setScale(const vec2f &scale) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setRotation(float angle) override;
    virtual void setOutlineColor(const Color &color) override;
    virtual void setOutlineThickness(float thickness) override;

    virtual float getRotation() const override;
    virtual vec2f getScale() const override;
    virtual vec2f getPosition() const override;
    virtual const Color &getFillColor() const override;
    virtual vec2u getSize() const override;
    virtual float getOutlineThickness() const override;
    virtual const Color &getOutlineColor() const override;
    virtual const IImage* getImage() const override;

    virtual void move(const vec2f &offset) override;

    virtual void draw(IRenderWindow *window) const override;

private:
    sf::RectangleShape shape_;

    mutable bool update_flag_ = false;

    psapi::sfm::Color color_ = psapi::sfm::Color(255, 255, 255);
};


class EllipseShape : public IEllipseShape, public Shape
{
public:
    EllipseShape(unsigned int width, unsigned int height);
    virtual ~EllipseShape() = default;

    virtual void setTexture(const ITexture *texture) override;
    virtual void setFillColor(const Color &color) override;

    virtual void setPosition(const vec2i &pos) override;
    virtual void setPosition(const vec2f &pos) override;
    virtual void setPosition(const vec2d &pos) override;
    virtual void setScale(const vec2f &scale) override;
    virtual void setSize(const vec2u &size) override;
    virtual void setRotation(float angle) override;
    virtual void setOutlineColor(const Color &color) override;
    virtual void setOutlineThickness(float thickness) override;

    virtual float getRotation() const override;
    virtual vec2f getScale() const override;
    virtual vec2f getPosition() const override;
    virtual const Color &getFillColor() const override;
    virtual vec2u getSize() const override;
    virtual float getOutlineThickness() const override;
    virtual const Color &getOutlineColor() const override;
    virtual const IImage* getImage() const override;

    virtual void move(const vec2f &offset) override;

    virtual void draw(IRenderWindow *window) const override;
private:
    sf::CircleShape shape_;
};


} // sfm

} // psapi


#endif // SFML_HPP
