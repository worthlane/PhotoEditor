#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include "api/api_photoshop.hpp"

#include "implementation/actions.hpp"

class AWindow : public psapi::IWindow
{
public:
    AWindow(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size);
    psapi::wid_t getId() const override;
    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;
    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;
    void setParent(const psapi::IWindow* parent) override;
    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;
    bool isWindowContainer() const override;
protected:
    psapi::wid_t id_;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_;
    psapi::vec2u size_;
};

class AWindowContainer : public psapi::IWindowContainer
{
public:
    AWindowContainer(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size);

    psapi::wid_t getId() const override;
    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;
    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;
    void setParent(const psapi::IWindow* parent) override;
    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    void removeWindow(psapi::wid_t id) override;

    bool isWindowContainer() const override;
protected:
    psapi::wid_t id_;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_;
    psapi::vec2u size_;

    std::vector<std::unique_ptr<psapi::IWindow>> windows_;
};

class RootWindow : public psapi::IRootWindow
{
public:
    RootWindow();

    void draw(psapi::IRenderWindow* renderWindow) override;
    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    psapi::layer_id_t getUpperLayerId() const override;
    psapi::layer_id_t increaseLayerId() override;
    psapi::layer_id_t decreaseLayerId() override;

    psapi::wid_t getId() const override;
    psapi::IWindow* getWindowById(psapi::wid_t id) override;
    const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    psapi::vec2i getPos() const override;
    psapi::vec2u getSize() const override;
    void setSize(const psapi::vec2u& size) override;
    void setPos(const psapi::vec2i& pos) override;
    void setParent(const psapi::IWindow* parent) override;
    void forceActivate() override;
    void forceDeactivate() override;
    bool isActive() const override;

    void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    void removeWindow(psapi::wid_t id) override;
private:
    psapi::wid_t id_;

    std::vector<std::unique_ptr<psapi::IWindow>> windows_;

    friend class RootAction;
};

class RootAction : public AAction
{
public:
    RootAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, RootWindow* root_window);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;
private:
    RootWindow* root_window_;
};


#endif // PHOTOSHOP_HPP
