#ifndef PHOTOSHOP_HPP
#define PHOTOSHOP_HPP

#include "api/api_photoshop.hpp"

class AWindow : public psapi::IWindow
{
public:
    AWindow(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size);
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

    virtual void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id) override;

    virtual bool isWindowContainer() const override;
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

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::layer_id_t getUpperLayerId() const override;
    virtual psapi::layer_id_t increaseLayerId() override;
    virtual psapi::layer_id_t decreaseLayerId() override;

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

    virtual void addWindow(std::unique_ptr<psapi::IWindow> window) override;
    virtual void removeWindow(psapi::wid_t id) override;
private:
    psapi::wid_t id_;

    std::vector<std::unique_ptr<psapi::IWindow>> windows_;
};

//virtual layer_id_t getUpperLayerId() const = 0;
//virtual layer_id_t increaseLayerId() = 0;
//virtual layer_id_t decreaseLayerId() = 0;
//virtual void addWindow(std::unique_ptr<IWindow> window) = 0;
//virtual void removeWindow(wid_t id) = 0;
//virtual bool isWindowContainer() const override;
//virtual void draw(IRenderWindow* renderWindow) = 0;
//virtual std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) = 0;
//virtual wid_t getId() const = 0;
//virtual IWindow* getWindowById(wid_t id) = 0;
//virtual const IWindow* getWindowById(wid_t id) const = 0;
//virtual vec2i getPos() const = 0;
//virtual vec2u getSize() const = 0;
//virtual void setSize(const vec2u& size) = 0;
//virtual void setPos(const vec2i& pos) = 0;
//virtual void setParent(const IWindow* parent) = 0;
//virtual void forceActivate() = 0;
//virtual void forceDeactivate() = 0;
//virtual bool isActive() const = 0;
//virtual bool isWindowContainer() const = 0;


#endif // PHOTOSHOP_HPP
