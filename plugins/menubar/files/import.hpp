#ifndef FILEIMPORT_PLUGIN_HPP
#define FILEIMPORT_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

class ImportButton : public TextButton
{
public:
    ImportButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& file_name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class ImportAction;
};

class ImportAction : public AAction
{
public:
    ImportAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ImportButton* import);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

private:
    ImportButton* import_ = nullptr;
};

#endif // FILEIMPORT_PLUGIN_HPP
