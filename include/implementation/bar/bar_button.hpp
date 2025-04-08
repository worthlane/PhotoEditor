#ifndef BAR_BUTTON_HPP
#define BAR_BUTTON_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"
#include "api/api_memento.hpp"

class ABarButton : public psapi::IBarButton
{
public:
    ABarButton(const psapi::wid_t id, psapi::IBar* bar,
               const psapi::vec2i& pos,
               const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ISprite> sprite);

    psapi::wid_t getId() const override;

    void setState(State state) override;
    State getState() const override;

    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    void draw(psapi::IRenderWindow* renderWindow) override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;

    void setParent(const psapi::IWindow* parent) override;

    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    bool isWindowContainer() const override;

    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;

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

    void setState(State state) override;

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

class TextButton : public PressButton
{
public:
    TextButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                std::string& name, psapi::sfm::Color color);

    void draw(psapi::IRenderWindow* renderWindow) override;

protected:
    std::unique_ptr<psapi::sfm::IText> text_;
    std::unique_ptr<psapi::sfm::IFont> font_;

    std::string name_;
    psapi::sfm::Color text_color_;
    psapi::vec2u text_gap_ = {0, 0};
};

class AMenuButton : public psapi::IMenuButton
{
public:

    AMenuButton(const psapi::wid_t id, psapi::IBar* bar,
               const psapi::vec2i& pos,
               const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ISprite> sprite,
               std::unique_ptr<psapi::IBar> menu);

    psapi::wid_t getId() const override;

    void setState(State state) override;
    State getState() const override;

    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    void draw(psapi::IRenderWindow* renderWindow) override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;

    void setParent(const psapi::IWindow* parent) override;

    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    bool isWindowContainer() const override;

    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;

    void addMenuItem(std::unique_ptr<psapi::IWindow> item) override;

    void activateMenu() override;
    void deactivateMenu() override;

    psapi::IBar *getMenu() override;
    const psapi::IBar *getMenu() const override;

    void expandMenu(psapi::IBar* menu, psapi::IWindow* item);

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

class MenuSwitchButton : public AMenuButton
{
public:
    MenuSwitchButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu);

    void setState(State state) override;

protected:
    State prev_state_ = State::Normal;

    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);
};

class TextMenuButton : public MenuSwitchButton
{
public:
    TextMenuButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                std::string& name, psapi::sfm::Color color, std::unique_ptr<psapi::IBar> menu, const bool need_centering = true);

    void draw(psapi::IRenderWindow* renderWindow) override;

protected:
    std::unique_ptr<psapi::sfm::IText> text_;
    std::unique_ptr<psapi::sfm::IFont> font_;

    std::string name_;
    psapi::sfm::Color text_color_;
    psapi::vec2u text_gap_ = {0, 0};
};

#endif // BAR_BUTTON_HPP

