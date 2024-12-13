#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "api/api_bar.hpp"
#include "api/api_sfm.hpp"

#include "implementation/actions.hpp"

class ColorPalette : public psapi::IColorPalette
{
public:
    ColorPalette();

    virtual psapi::sfm::Color getColor() const override;
    virtual void setColor(const psapi::sfm::Color &color) override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::wid_t getId() const override;
    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;
    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;
    virtual void setParent(const psapi::IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

protected:
    psapi::wid_t id_ = psapi::kColorPaletteId;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_ = {0, 0};
    psapi::vec2u size_ = {0, 0};

    psapi::sfm::Color color_ = psapi::sfm::Color(255, 255, 255, 255);

    std::unique_ptr<psapi::sfm::ITexture> texture_;
    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    friend class ColorPaletteAction;
};

class ColorPaletteAction : public AAction
{
public:
    ColorPaletteAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ColorPalette* palette);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    ColorPalette* palette_;
};

#endif // OPTIONS_HPP
