#include <string>

#include "../plugins/files/import.hpp"

ImportButton::ImportButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                           std::string& file_name, psapi::sfm::Color color,
                           psapi::ICanvas* canvas) :
                           TextButton(id, bar, pos, size, file_name, color), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> ImportButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<ImportAction>(renderWindow, event, this);
}

ImportAction::ImportAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ImportButton* import) :
                                AAction(render_window, event), import_(import)
{}

bool ImportAction::execute(const Key& key)
{
    auto canvas = import_->canvas_;
    std::string& file_name = import_->name_;

    std::unique_ptr<psapi::sfm::IImage> image = psapi::sfm::IImage::create();
    image->loadFromFile(file_name);

    //psapi::vec2u canvas_size = canvas->getSize();

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto size = image->getSize();
    auto canvas_size = canvas->getSize();

    for (int x = 0; x < size.x && x < canvas_size.x; x++)
    {
        for (int y = 0; y < size.y && x < canvas_size.y; y++)
        {
            psapi::vec2i pos = {x, y};
            auto pixel = image->getPixel(pos.x, pos.y);

            temp_layer->setPixel(pos, pixel);
        }
    }

    canvas->cleanTempLayer();

    return true;
}

bool ImportAction::isUndoable(const Key& key)
{
    return false;
}
