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
    void setColor(const Color &color, size_t ind) override;
    Color getColor(size_t ind) const override;

    void setPosition(const vec2i &coord, size_t ind) override;
    void setPosition(const vec2f &coord, size_t ind) override;
    void setPosition(const vec2d &coord, size_t ind) override;
    void setPosition(int x, int y, size_t ind) override;
    void setPosition(float x, float y, size_t ind) override;
    void setPosition(double x, double y, size_t ind) override;

    void draw(IRenderWindow *window) const override;
};

class Image : public IImage
{
public:
    void create(unsigned int width, unsigned int height, const Color &color=Color(0, 0, 0)) override;
    void create(vec2u size, const Color &color=Color(0, 0, 0)) override;

    void create(unsigned int width, unsigned int height, const Color *pixels) override;
    void create(vec2u size, const Color *pixels) override;

    bool loadFromFile(const std::string &filename) override;

    bool saveToFile(const std::string &filename) const;

    void setPixel(unsigned int x, unsigned int y, const Color &color) override;
    void setPixel(vec2u pos, const Color &color) override;

    Color getPixel(unsigned int x, unsigned int y) const override;
    Color getPixel(vec2u pos) const override;

    vec2u getSize() const override;

    vec2i getPos() const override;
    void setPos(const vec2i &pos) override;

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
    bool create(unsigned int width, unsigned int height)                                     override;
    bool loadFromFile  (const std::string& filename,        const IntRect& area = IntRect()) override;
    bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect()) override;
    vec2u getSize() const                                                                    override;
    std::unique_ptr<IImage> copyToImage() const                                              override;
    void update(const IImage *image)                                                         override;
    void update(const Color *pixels)                                                         override;
    void update(const Color *pixels, unsigned int width, unsigned int height,
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
    void setTexture(const ITexture *texture, bool reset_rect = false) override;
    void setTextureRect(const IntRect &rectangle) override;

    void setPosition(float x, float y) override;
    void setPosition(const vec2f &pos) override;

    void setScale(float factorX, float factorY) override;
    vec2u getSize() const override;

    void setColor(const Color &color) override;
    Color getColor() const override;

    void setRotation(float angle) override;

    const vec2f getPosition() const override;
    IntRect getGlobalBounds() const override;

    void draw(IRenderWindow *renderWindow) const override;

private:
    sf::Sprite sprite_;
};

class Font : public IFont
{
public:
    bool loadFromFile(const std::string& filename) override;
private:
    sf::Font font_;

    friend class Text;
};

class Text : public IText
{
public:
    void setString(const std::string& string) override;
    void setFont(const IFont* font)           override;
    void setCharacterSize(unsigned int size)  override;
    void setStyle(uint32_t style)             override;
    void setFillColor(const Color* color)     override;
    void setOutlineColor(const Color* color)  override;
    void setOutlineThickness(float thickness) override;

    void draw(IRenderWindow *window) const override;

    void setPos(const vec2f &pos) override;
    void setSize(const vec2f &size) override;

    IntRect getGlobalBounds() const override;

private:
    sf::Text text_;
};

class RenderWindow : public IRenderWindow
{
public:
    RenderWindow(unsigned int width, unsigned int height, const std::string& name);
    virtual ~RenderWindow() = default;

    bool isOpen() const override;
    void clear()   override;
    void display() override;
    void close()   override;

    vec2u getSize() const override;

    bool pollEvent(Event& event) override;

    void draw(Drawable *target)override;

    void setFps(float fps) override;
    float getFps() const override;

    sf::RenderWindow& getWindow();

private:
    sf::RenderWindow window_;

    friend class Sprite;
    friend class Text;
    friend class Mouse;
};


class Shape : public IShape
{
protected:

    mutable std::unique_ptr<Image> cached_image_;
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

    void setTexture(const ITexture *texture) override;
    void setFillColor(const Color &color) override;

    void setPosition(const vec2i &pos) override;
    void setPosition(const vec2f &pos) override;
    void setPosition(const vec2d &pos) override;
    void setScale(const vec2f &scale) override;
    void setSize(const vec2u &size) override;
    void setRotation(float angle) override;
    void setOutlineColor(const Color &color) override;
    void setOutlineThickness(float thickness) override;

    float getRotation() const override;
    vec2f getScale() const override;
    vec2f getPosition() const override;
    const Color &getFillColor() const override;
    vec2u getSize() const override;
    float getOutlineThickness() const override;
    const Color &getOutlineColor() const override;
    const IImage* getImage() const override;

    void move(const vec2f &offset) override;

    void draw(IRenderWindow *window) const override;

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

    void setTexture(const ITexture *texture) override;
    void setFillColor(const Color &color) override;

    void setPosition(const vec2i &pos) override;
    void setPosition(const vec2f &pos) override;
    void setPosition(const vec2d &pos) override;
    void setScale(const vec2f &scale) override;
    void setSize(const vec2u &size) override;
    void setRotation(float angle) override;
    void setOutlineColor(const Color &color) override;
    void setOutlineThickness(float thickness) override;

    float getRotation() const override;
    vec2f getScale() const override;
    vec2f getPosition() const override;
    const Color &getFillColor() const override;
    vec2u getSize() const override;
    float getOutlineThickness() const override;
    const Color &getOutlineColor() const override;
    const IImage* getImage() const override;

    void move(const vec2f &offset) override;

    void draw(IRenderWindow *window) const override;
private:
    sf::CircleShape shape_;

    mutable bool update_flag_ = false;
};


} // sfm

} // psapi


#endif // SFML_HPP
