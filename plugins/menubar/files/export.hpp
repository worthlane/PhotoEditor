#ifndef FILEEXPORT_PLUGIN_HPP
#define FILEEXPORT_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

class ExportButton : public TextButton
{
public:
    ExportButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& file_name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class ExportAction;
};

class ExportAction : public AAction
{
public:
    ExportAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ExportButton* export_but);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

private:
    ExportButton* export_but_ = nullptr;
};

#endif // FILEEXPORT_PLUGIN_HPP
