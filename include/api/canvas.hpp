#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "graphics/my_sfml.hpp"
#include "standard/api_canvas.hpp"

namespace psapi
{

class Layer : public ILayer
{
public:
    Layer(const size_t width, const size_t height);
    Layer(const size_t width, const size_t height, const sfm::Color& color);
    virtual ~Layer() = default;

    virtual sfm::Color getPixel(sfm::vec2i pos) const override;
    virtual void       setPixel(sfm::vec2i pos, sfm::Color pixel) override;

private:
    sfm::Image image_;

    void resize(const size_t width, const size_t height);

    friend class Canvas;
};

class Canvas : public ICanvas
{
public:
    Canvas(sfm::vec2i pos,sfm::vec2i size, sfm::vec2f scale);

    virtual void draw(IRenderWindow* renderWindow) override;
    virtual bool update(const IRenderWindow* renderWindow, const Event& event) override;

    virtual ILayer*       getLayer(size_t index)       override;
    virtual const ILayer* getLayer(size_t index) const override;

    virtual ILayer*       getTempLayer()       override;
    virtual const ILayer* getTempLayer() const override;

    virtual void cleanTempLayer() override;

    virtual size_t getActiveLayerIndex() const override;
    virtual void   setActiveLayerIndex(size_t index) override;

    virtual bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer) override;
    virtual bool removeLayer     (size_t index) override;
    virtual bool insertEmptyLayer(size_t index) override;

    virtual size_t getNumLayers() const override { return layers_.size(); };

    virtual void setPos  (sfm::vec2i pos)   override { pos_ = pos; };
    virtual void setScale(sfm::vec2f scale) override { scale_ = scale; };
    virtual void setSize (sfm::vec2i size)  override;

    virtual sfm::vec2i getMousePosition() const override { return mouse_pos_; };
    virtual bool       isPressed()        const override { return pressed_; };

private:
    sfm::vec2i pos_;
    sfm::vec2i size_;
    sfm::vec2f scale_;

    std::vector<std::unique_ptr<Layer> > layers_;
    std::unique_ptr<Layer> temp_layer_;

    sfm::vec2i mouse_pos_ = {0, 0};
    bool       pressed_   = false;

    size_t      active_ = 0;
};

} // psapi

#endif // CANVAS_HPP
