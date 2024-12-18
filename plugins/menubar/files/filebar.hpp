#ifndef FILTERSBAR_PLUGIN_HPP
#define FILTERSBAR_PLUGIN_HPP

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"


extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

class FileBar : public ABar
{
public:
    FileBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    friend class FileBarAction;
};

class FileBarAction : public AAction
{
public:
    FileBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FileBar* file_bar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    FileBar* file_bar_;
};

class FileButton : public TextMenuButton
{
public:
    FileButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
               std::string& name, psapi::sfm::Color color, std::unique_ptr<psapi::IBar> menu);

    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;
private:

    friend class FileAction;
};

class FileAction : public AAction
{
public:
    FileAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FileButton* file_button);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;

private:
    FileButton* file_button_ = nullptr;
};

static const psapi::wid_t kFileBarWindowId = 267;

static const psapi::wid_t kFileImportBarWindowId = 777;
static const psapi::wid_t kFileExportBarWindowId = 778;

#endif // FILTERSBAR_PLUGIN_HPP
