#ifndef TOOLBAR_BUTTON_PLUGIN_HPP
#define TOOLBAR_BUTTON_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

class Action
{
    public:
        virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) = 0;
};

class ABarButton : public psapi::IBarButton
{
public:
    ABarButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ISprite> sprite,
               std::unique_ptr<Action> action);

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

    void setPos(const psapi::vec2i pos);

protected:
    psapi::wid_t id_;

    State state_ = State::Normal;

    psapi::vec2u size_;
    psapi::vec2i pos_;

    std::unique_ptr<psapi::sfm::ISprite> sprite_;

    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    std::unique_ptr<Action> action_;
};

class SwitchButton : public ABarButton
{
public:
    SwitchButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 std::unique_ptr<Action> action);

    virtual bool update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual void setState(State state) override;

private:
    State prev_state_ = State::Normal;

    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);
};

class PressButton : public ABarButton
{
public:
    PressButton(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite,
                 std::unique_ptr<Action> action);

    virtual bool update(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);
};

#endif // TOOLBAR_BUTTON_PLUGIN_HPP

