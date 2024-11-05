#ifndef BRUSH_PLUGIN_HPP
#define BRUSH_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "graphics/my_sfml.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

static const psapi::wid_t kBrushButtonId = 228;

class BrushButton : public psapi::IBarButton
{
public:
    BrushButton(const psapi::vec2i& pos,
                const psapi::vec2u& size,
                std::unique_ptr<psapi::sfm::ISprite> sprite);

    virtual psapi::wid_t getId() const override { return kBrushButtonId; };

    virtual void setState(State state) override;
    virtual State getState() const override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual bool update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;

    void setPos(const psapi::vec2i pos);

    virtual void setParent(const psapi::IWindow* parent) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;

    virtual bool isWindowContainer() const override;

private:
    State state_      = State::Normal;
    State prev_state_ = State::Normal;

    psapi::vec2u size_;
    psapi::vec2i pos_;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    

    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event,
                     const psapi::vec2i& mouse_pos, const bool LMB_down);
};

#endif // BRUSH_PLUGIN_HPP
