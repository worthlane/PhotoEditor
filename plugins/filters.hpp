#ifndef FILTER_PLUGIN_HPP
#define FILTER_PLUGIN_HPP

#include "standard/api_bar.hpp"
#include "standard/api_canvas.hpp"

#include "../plugins/toolbar_button.hpp"
#include "../plugins/catmull.hpp"

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class NegativeAction : public Action
{
public:
    NegativeAction(const double k, const size_t radius, psapi::ICanvas* canvas, const bool scale_related = false);
    ~NegativeAction() = default;

    virtual bool operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    size_t radius_;

    double k_;

    psapi::ICanvas* canvas_ = nullptr;

    std::vector<std::vector<bool>> changed_;

    bool scale_related_ = false;

    InterpolationArray array_;
};

static const psapi::wid_t kNegativeFilterButtonId = 233;

#endif // BRUSH_PLUGIN_HPP
