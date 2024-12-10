#include <iostream>

#include "implementation/photoshop.hpp"

static const size_t SCREEN_LENGTH = 1200;
static const size_t SCREEN_WIDTH  = 800;

psapi::sfm::IntRect psapi::getCanvasIntRect()
{
    return {{0, 0}, {SCREEN_LENGTH, SCREEN_WIDTH}};
}

psapi::sfm::IntRect psapi::getToolbarIntRect()
{
    return {{0, 0}, {0, 0}};
}

psapi::sfm::IntRect psapi::getOptionsBarIntRect()
{
    return {{0, 0}, {0, 0}};
}

psapi::sfm::IntRect psapi::getInstrumentOptionsIntRect()
{
    return {{0, 0}, {0, 0}};
}

psapi::sfm::vec2u psapi::getScreenSize()
{
    return {SCREEN_LENGTH, SCREEN_WIDTH};
}

bool psapi::IWindowContainer::isWindowContainer() const
{
    return true;
};

psapi::generalFunction psapi::getGeneralFunction(const std::string& name)
{
    std::cout << "getGeneralFunction: " << name << std::endl;
    return nullptr;
}

psapi::IRootWindow* psapi::getRootWindow()
{
    static RootWindow root_window;

    return &root_window;
}

// ******************* AWINDOW ************************

AWindow::AWindow(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size) : id_(id), pos_(pos), size_(size) {}

psapi::wid_t AWindow::getId() const
{
    return id_;
}

psapi::IWindow* AWindow::getWindowById(psapi::wid_t id)
{
    return (id == getId()) ? this : nullptr;
}

const psapi::IWindow* AWindow::getWindowById(psapi::wid_t id) const
{
    return (id == getId()) ? this : nullptr;
}

psapi::sfm::vec2i AWindow::getPos() const
{
    return pos_;
}

psapi::sfm::vec2u AWindow::getSize() const
{
    return size_;
}

void AWindow::setSize(const psapi::sfm::vec2u& size)
{
    size_ = size;
}

void AWindow::setPos(const psapi::sfm::vec2i& pos)
{
    pos_ = pos;
}

void AWindow::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void AWindow::forceActivate()
{
    is_active_ = true;
}

void AWindow::forceDeactivate()
{
    is_active_ = false;
}

bool AWindow::isActive() const
{
    return is_active_;
}

bool AWindow::isWindowContainer() const
{
    return false;
}

// ******************* AWINDOWCONTAINER *************************

AWindowContainer::AWindowContainer(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size) : id_(id), pos_(pos), size_(size) {}

psapi::wid_t AWindowContainer::getId() const
{
    return id_;
}

psapi::IWindow* AWindowContainer::getWindowById(psapi::wid_t id)
{
    if (id == getId())
        return this;

    for (auto& window : windows_)
    {
        psapi::IWindow* result = window->getWindowById(id);

        if (result)
            return result;
    }

    return nullptr;
}

const psapi::IWindow* AWindowContainer::getWindowById(psapi::wid_t id) const
{
    if (id == getId())
        return this;

    for (auto& window : windows_)
    {
        psapi::IWindow* result = window->getWindowById(id);

        if (result)
            return result;
    }

    return nullptr;
}

psapi::vec2i AWindowContainer::getPos() const
{
    return pos_;
}

psapi::vec2u AWindowContainer::getSize() const
{
    return size_;
}

void AWindowContainer::setSize(const psapi::vec2u& size)
{
    size_ = size;
}

void AWindowContainer::setPos(const psapi::vec2i& pos)
{
    pos_ = pos;
}

void AWindowContainer::setParent(const psapi::IWindow* parent)
{
    parent_ = parent;
}

void AWindowContainer::forceActivate()
{
    is_active_ = true;
}

void AWindowContainer::forceDeactivate()
{
    is_active_ = false;
}

bool AWindowContainer::isActive() const
{
    return is_active_;
}

void AWindowContainer::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    windows_.push_back(std::move(window));
}

void AWindowContainer::removeWindow(psapi::wid_t id)
{
    for (size_t i = 0; i < windows_.size(); ++i)
    {
        if (windows_[i]->getId() == id)
        {
            windows_.erase(windows_.begin() + i);
            break;
        }
    }
}

bool AWindowContainer::isWindowContainer() const
{
    return true;
}

// **************** ROOTWINDOW *************************

RootWindow::RootWindow() : id_(psapi::kRootWindowId) {};

void RootWindow::draw(psapi::IRenderWindow* renderWindow)
{
    for (auto& window : windows_)
    {
        window->draw(renderWindow);
    }
}

std::unique_ptr<psapi::IAction> RootWindow::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<RootAction>(renderWindow, event, &windows_);
}

psapi::layer_id_t RootWindow::getUpperLayerId() const
{
    return 0;
}

psapi::layer_id_t RootWindow::increaseLayerId()
{
    return 0;
}

psapi::layer_id_t RootWindow::decreaseLayerId()
{
    return 0;
}

psapi::wid_t RootWindow::getId() const
{
    return id_;
}
psapi::IWindow* RootWindow::getWindowById(psapi::wid_t id)
{
    if (id == getId())
        return this;

    for (auto& window : windows_)
    {
        psapi::IWindow* result = window->getWindowById(id);

        if (result)
            return result;
    }

    return nullptr;
}

const psapi::IWindow* RootWindow::getWindowById(psapi::wid_t id) const
{
    if (id == getId())
        return this;

    for (auto& window : windows_)
    {
        psapi::IWindow* result = window->getWindowById(id);

        if (result)
            return result;
    }

    return nullptr;
}

psapi::vec2i RootWindow::getPos() const
{
    return {0, 0};
}
psapi::vec2u RootWindow::getSize() const
{
    return psapi::getScreenSize();
}
void RootWindow::setSize(const psapi::vec2u& size)
{}
void RootWindow::setPos(const psapi::vec2i& pos)
{}
void RootWindow::setParent(const psapi::IWindow* parent)
{}
void RootWindow::forceActivate()
{}
void RootWindow::forceDeactivate()
{}
bool RootWindow::isActive() const
{
    return true;
}

void RootWindow::addWindow(std::unique_ptr<psapi::IWindow> window)
{
    windows_.push_back(std::move(window));
}
void RootWindow::removeWindow(psapi::wid_t id)
{
    for (size_t i = 0; i < windows_.size(); ++i)
    {
        if (windows_[i]->getId() == id)
        {
            windows_.erase(windows_.begin() + i);
            break;
        }
    }
}

// ******* ROOT ACTION **********

RootAction::RootAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, std::vector<std::unique_ptr<psapi::IWindow>>* windows) :
AAction(render_window, event), windows_(windows)
{}

bool RootAction::execute(const Key& key)
{
    for (auto& window : *windows_)
    {
        psapi::getActionController()->execute(window->createAction(render_window_, event_));
    }
};

bool RootAction::isUndoable(const Key& key)
{
    return false;
};


