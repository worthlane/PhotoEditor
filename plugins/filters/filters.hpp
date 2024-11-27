#ifndef FILTER_PLUGIN_HPP
#define FILTER_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/bar_base/bar_button.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class ContrastAction : public Action
{
public:
    ContrastAction(const double k, psapi::ICanvas* canvas);
    ~ContrastAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;

    psapi::ICanvas* canvas_ = nullptr;
};

static const psapi::wid_t kNegativeFilterButtonId = 233;

#endif // BRUSH_PLUGIN_HPP
