#ifndef BAR_BUTTON_HPP
#define BAR_BUTTON_HPP

#include "api/api_bar.hpp"

class ABarButton : public psapi::IBarButton
{
public:
    ABarButton(const psapi::wid_t id, psapi::IBar* bar,
               const psapi::vec2i& pos,
               const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ISprite> sprite);

    virtual psapi::wid_t getId() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;

    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;

    virtual void setParent(const psapi::IWindow* parent) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;

    virtual bool isWindowContainer() const override;

    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;

protected:
    psapi::wid_t id_;

    State state_ = State::Normal;

    psapi::vec2u size_;
    psapi::vec2i pos_;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    const psapi::IBar* parent_ = nullptr;
    bool is_active_ = true;
};

class SwitchButton : public ABarButton
{
public:
    SwitchButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite);

    virtual void setState(State state) override;

protected:
    State prev_state_ = State::Normal;

    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);
};

class PressButton : public ABarButton
{
public:
    PressButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite);

protected:
    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);
};

class AMenuButton : public psapi::IMenuButton
{
public:

    AMenuButton(const psapi::wid_t id, psapi::IBar* bar,
               const psapi::vec2i& pos,
               const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ISprite> sprite,
               std::unique_ptr<psapi::IBar> menu);

    virtual psapi::wid_t getId() const override;

    virtual void setState(State state) override;
    virtual State getState() const override;

    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;

    virtual void setParent(const psapi::IWindow* parent) override;

    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;

    virtual bool isWindowContainer() const override;

    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;

    virtual void addMenuItem(std::unique_ptr<psapi::IWindow> item) override;

    virtual void activateMenu() override;
    virtual void deactivateMenu() override;

    virtual psapi::IBar *getMenu() override;
    virtual const psapi::IBar *getMenu() const override;

protected:
    psapi::wid_t id_;

    State state_ = State::Normal;

    psapi::vec2u size_;
    psapi::vec2i pos_;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    const psapi::IBar* parent_ = nullptr;
    bool is_active_ = true;

    std::unique_ptr<psapi::IBar> menu_;
};

#endif // BAR_BUTTON_HPP

