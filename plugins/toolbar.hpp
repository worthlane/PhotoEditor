#ifndef TOOLBAR_PLUGIN_HPP
#define TOOLBAR_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "graphics/my_sfml.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class ToolBar : public psapi::IBar
{
public:
    ToolBar(const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> normal);

    virtual psapi::wid_t getId() const override { return psapi::kToolBarWindowId; };

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual bool update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::ChildInfo getNextChildInfo() const override;
    virtual void finishButtonDraw(psapi::IRenderWindow* renderWindow, const psapi::IBarButton* button) const override;

    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;

    virtual void setParent(const psapi::IWindow* parent) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;

    virtual void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id) override;

    virtual bool isWindowContainer() const override { return true; };
private:
    psapi::vec2i pos_;
    psapi::vec2u size_;

    const psapi::IWindow* parent_;

    std::unique_ptr<psapi::sfm::ISprite> background_;

    std::unique_ptr<psapi::sfm::ISprite> hovered_;
    std::unique_ptr<psapi::sfm::ISprite> pressed_;
    std::unique_ptr<psapi::sfm::ISprite> normal_;

    std::vector<std::unique_ptr<psapi::IBarButton> > buttons_;

    bool is_active_ = true;
    size_t next_child_ = 0;

};

#endif // TOOLBAR_PLUGIN_HPP
