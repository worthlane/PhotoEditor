#ifndef CANVAS_PLUGIN_HPP
#define CANVAS_PLUGIN_HPP

#include "standard/api_canvas.hpp"

#include "../plugins/scrollbar.hpp"

extern "C" {

bool   loadPlugin();
void unloadPlugin();

}

class Layer : public psapi::ILayer
{
public:
    Layer(const size_t width, const size_t height);
    Layer(const size_t width, const size_t height, const psapi::sfm::Color& color);
    virtual ~Layer() = default;

    virtual psapi::sfm::Color getPixel(psapi::sfm::vec2i pos) const override;
    virtual void       setPixel(psapi::sfm::vec2i pos, psapi::sfm::Color pixel) override;

private:
    psapi::sfm::IImage* image_;

    void resize(const size_t width, const size_t height);

    friend class Canvas;
};

class Canvas : public psapi::ICanvas, public Scrollable
{
public:
    Canvas(const psapi::sfm::vec2i& pos,
           const psapi::sfm::vec2i& size,
           const psapi::sfm::vec2f& scale = {1, 1});

    virtual psapi::wid_t getId() const override { return psapi::kCanvasWindowId; }

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual bool update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::ILayer*       getLayer(size_t index)       override;
    virtual const psapi::ILayer* getLayer(size_t index) const override;

    virtual psapi::ILayer*       getTempLayer()       override;
    virtual const psapi::ILayer* getTempLayer() const override;

    virtual void cleanTempLayer() override;

    virtual size_t getActiveLayerIndex() const override;
    virtual void   setActiveLayerIndex(size_t index) override;

    virtual bool insertLayer     (size_t index, std::unique_ptr<psapi::ILayer> layer) override;
    virtual bool removeLayer     (size_t index) override;
    virtual bool insertEmptyLayer(size_t index) override;

    virtual size_t getNumLayers() const override { return layers_.size(); };

    virtual void setPos  (psapi::sfm::vec2i pos)   override { pos_ = pos; };
    virtual void setScale(psapi::sfm::vec2f scale) override { Scrollable::setScale(scale); };
    virtual void setSize (psapi::sfm::vec2i size)  override;

    virtual psapi::sfm::vec2i getMousePosition() const override { return mouse_pos_; };
    virtual bool       isPressed()        const override { return pressed_; };

    virtual psapi::sfm::vec2i getPos() const override { return pos_; }
    virtual psapi::sfm::vec2u getSize() const override { return {size_.x, size_.y}; }
    virtual psapi::sfm::vec2u getObjectSize() const override { return {size_.x, size_.y}; }

    virtual void setParent(const psapi::IWindow* parent) override;

    virtual void forceDeactivate() override;
    virtual void forceActivate() override;
    virtual bool isActive() const override;

    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual bool isWindowContainer() const override { return false; }

    virtual bool updateScale(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

protected:

    std::vector<std::unique_ptr<Layer> > layers_;
    std::unique_ptr<Layer> temp_layer_;

    psapi::sfm::vec2i mouse_pos_ = {0, 0};
    bool       pressed_   = false;

    size_t     active_layer_ = 0;
    bool       is_active_ = true;

    psapi::sfm::vec2i pos_;
    psapi::sfm::vec2i size_;
};


#endif // CANVAS_PLUGIN_HPP
