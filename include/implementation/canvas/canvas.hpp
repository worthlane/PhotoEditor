#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "api/api_canvas.hpp"
#include "api/api_memento.hpp"

#include "implementation/canvas/scrollbar.hpp"

class Layer : public psapi::ILayer
{
public:
    Layer(const size_t width, const size_t height, const psapi::sfm::Color& color = {255, 255, 255, 255});

    psapi::sfm::Color getPixel(psapi::sfm::vec2i pos) const override;
    void       setPixel(psapi::sfm::vec2i pos, psapi::sfm::Color pixel) override;

    psapi::drawable_id_t addDrawable(std::unique_ptr<psapi::sfm::Drawable> object) override;

    void removeDrawable(psapi::drawable_id_t id) override;
    void removeAllDrawables() override;

    psapi::sfm::vec2u getSize() const override;

    std::unique_ptr<psapi::ILayerSnapshot> save() override;
    void restore(psapi::ILayerSnapshot* snapshot) override;

private:
    psapi::sfm::IImage* image_;
    psapi::sfm::vec2u   size_;

    void dumpOnImage(psapi::sfm::IImage* image, const psapi::sfm::vec2i& coord_start, const psapi::sfm::vec2f& scale, const psapi::sfm::vec2i& size);

    friend class Canvas;
};

class Canvas : public psapi::ICanvas, public Scrollable
{
public:
    Canvas(const psapi::sfm::vec2i& pos,
           const psapi::sfm::vec2u& size,
           const psapi::sfm::Color& base_color = {0, 0, 0, 0});

    psapi::ILayer*       getLayer(size_t index) override;
    const psapi::ILayer* getLayer(size_t index) const override;
    psapi::ILayer*       getTempLayer()       override;
    const psapi::ILayer* getTempLayer() const override;

    void cleanTempLayer() override;

    size_t getNumLayers() const override;
    size_t getActiveLayerIndex() const override;
    void   setActiveLayerIndex(size_t index) override;

    bool insertLayer(size_t index, std::unique_ptr<psapi::ILayer> layer) override;
    bool removeLayer     (size_t index) override;
    bool insertEmptyLayer(size_t index) override;

    void setZoom(psapi::sfm::vec2f zoom) override;
    psapi::sfm::vec2i getMousePosition() const override;
    bool isPressedRightMouseButton() const override;
    bool isPressedLeftMouseButton()  const override;
    bool isPressedScrollButton()     const override;

    psapi::sfm::Color getCanvasBaseColor() const override;

    void draw(psapi::IRenderWindow* renderWindow) override;
    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    std::unique_ptr<psapi::ICanvasSnapshot> save() override;
    void restore(psapi::ICanvasSnapshot* snapshot) override;

    psapi::wid_t getId() const override;
    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::sfm::vec2i getPos() const override;
    psapi::sfm::vec2u getSize() const override;
    void setSize(const psapi::sfm::vec2u& size) override;
    void setPos(const psapi::sfm::vec2i& pos) override;
    void setParent(const psapi::IWindow* parent) override;
    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;
    bool isWindowContainer() const override;

    psapi::sfm::vec2u getObjectSize() const override;
    bool updateScale(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

protected:
    psapi::wid_t id_;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_;
    psapi::vec2u size_;

    std::vector<std::unique_ptr<psapi::ILayer> > layers_;
    std::unique_ptr<psapi::ILayer> temp_layer_;

    std::unique_ptr<psapi::sfm::ISprite> background_sprite_;
    std::unique_ptr<psapi::sfm::ITexture> background_texture_;

    psapi::sfm::vec2i mouse_pos_ = {0, 0};
    bool is_pressed_LMB_ = false;
    bool is_pressed_RMB_ = false;
    bool is_pressed_scroll_ = false;

    size_t     active_layer_ = 0;

    psapi::sfm::Color base_color_ = {255, 255, 255, 255};

    friend class CanvasAction;

};

class CanvasAction : public AAction
{
public:
    CanvasAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, Canvas* canvas);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;
private:
    Canvas* canvas_;
};

#endif // CANVAS_HPP
