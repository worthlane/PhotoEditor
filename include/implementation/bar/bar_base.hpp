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

    psapi::wid_t getId() const override;
    void finishButtonDraw(psapi::IRenderWindow* renderWindow, const psapi::IBarButton* button) const override;

    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;

    void setParent(const psapi::IWindow* parent) override;

    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    void removeWindow(psapi::wid_t id) override;

    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;
    bool unPressAllButtons() override;

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

class AOptionsBar : public psapi::IOptionsBar
{
public:
    AOptionsBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                std::unique_ptr<psapi::sfm::ISprite> background);

    void removeAllOptions() override;

    psapi::wid_t getId() const override;

    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;

    void setParent(const psapi::IWindow* parent) override;

    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    void removeWindow(psapi::wid_t id) override;

    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;

protected:
    psapi::wid_t id_;
    psapi::vec2i pos_;
    psapi::vec2u size_;

    const psapi::IWindow* parent_;

    std::unique_ptr<psapi::sfm::ISprite> background_;

    std::vector<std::unique_ptr<psapi::IWindow>> options_;

    bool is_active_ = true;
};

//psapi::sfm::IntRect getNextChildInfo(const size_t curr_child);


#endif // BAR_HPP
