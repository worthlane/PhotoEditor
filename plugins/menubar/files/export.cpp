#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "../plugins/menubar/files/export.hpp"

#include "implementation/sfm.hpp"

ExportButton::ExportButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                           std::string& file_name, psapi::sfm::Color color,
                           psapi::ICanvas* canvas) :
                           TextButton(id, bar, pos, size, file_name, color), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> ExportButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<ExportAction>(renderWindow, event, this);
}

ExportAction::ExportAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ExportButton* export_but) :
                                AAction(render_window, event), export_but_(export_but)
{}

bool ExportAction::execute(const Key& key)
{
    auto canvas = export_but_->canvas_;
    std::string& file_name = export_but_->name_;

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto canvas_size = canvas->getSize();

    sf::Image img;
    img.create(canvas_size.x, canvas_size.y);

    for (int x = 0; x < canvas_size.x; x++)
    {
        for (int y = 0; y < canvas_size.y; y++)
        {
            psapi::vec2u pos = {x, y};
            psapi::sfm::Color pixel = layer->getPixel(pos);

            img.setPixel(pos.x, pos.y, sf::Color(pixel.r, pixel.g, pixel.b, pixel.a));
        }
    }

    img.saveToFile(file_name);

    return true;
}

bool ExportAction::isUndoable(const Key& key)
{
    return false;
}
