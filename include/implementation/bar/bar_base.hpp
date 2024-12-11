#ifndef BAR_HPP
#define BAR_HPP

#include "api/api_bar.hpp"

class ABar : public psapi::IBar
{
public:
    ABar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual psapi::wid_t getId() const override;
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

    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;
    virtual bool unPressAllButtons() override;

protected:
    psapi::wid_t id_;
    psapi::vec2i pos_;
    psapi::vec2u size_;

    const psapi::IWindow* parent_;

    std::unique_ptr<psapi::sfm::ISprite> background_;

    std::unique_ptr<psapi::sfm::ISprite> hovered_;
    std::unique_ptr<psapi::sfm::ISprite> pressed_;
    std::unique_ptr<psapi::sfm::ISprite> normal_;
    std::unique_ptr<psapi::sfm::ISprite> released_;

    std::vector<std::unique_ptr<psapi::IBarButton>> buttons_;

    bool is_active_ = true;
};

//psapi::sfm::IntRect getNextChildInfo(const size_t curr_child);


#endif // BAR_HPP
