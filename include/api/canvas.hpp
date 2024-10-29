#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "standard/api_canvas.hpp"

namespace psapi
{

class Layer : public ILayer
{
public:
    virtual sfm::Color getPixel(sfm::vec2i pos) const override;
    virtual void       setPixel(sfm::vec2i pos, sfm::Color pixel) override;
};

class Canvas : public ICanvas
{
public:
    virtual ILayer*       getLayer(size_t index)       override;
    virtual const ILayer* getLayer(size_t index) const override;

    virtual ILayer*       getTempLayer()       override;
    virtual const ILayer* getTempLayer() const override;

    virtual void cleanTempLayer() override;

    virtual size_t getNumLayers() const override;

    virtual size_t getActiveLayerIndex() const override;
    virtual void   setActiveLayerIndex(size_t index) override;

    virtual bool insertLayer     (size_t index, std::unique_ptr<ILayer> layer) override;
    virtual bool removeLayer     (size_t index) override;
    virtual bool insertEmptyLayer(size_t index) override;

    virtual void setPos  (sfm::vec2i pos)   override;
    virtual void setSize (sfm::vec2i size)  override;
    virtual void setScale(sfm::vec2f scale) override;

    virtual sfm::vec2i getMousePosition() const override;
    virtual bool       isPressed()        const override;
};

} // psapi

#endif // CANVAS_HPP
