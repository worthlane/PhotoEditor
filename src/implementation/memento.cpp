#include <cassert>
#include <iostream>

#include "api/api_canvas.hpp"
#include "api/api_sfm.hpp"

#include "implementation/memento.hpp"

ACanvasSnapshot::ACanvasSnapshot(std::unique_ptr<psapi::sfm::IImage> img) : img_(std::move(img))
{}

psapi::sfm::IImage* ACanvasSnapshot::getImage() const
{
    return img_.get();
}


template<>
std::unique_ptr<psapi::ICanvasSnapshot> AMementable<psapi::ICanvasSnapshot>::save()
{
    psapi::ICanvas* canvas = dynamic_cast<psapi::ICanvas*>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
    psapi::ILayer*  layer = canvas->getLayer(canvas->getActiveLayerIndex());

    auto canvas_size = canvas->getSize();

    std::unique_ptr<psapi::sfm::IImage> img = psapi::sfm::IImage::create();
    img->create(canvas_size.x, canvas_size.y);

    for (int x = 0; x < canvas_size.x; x++)
    {
        for (int y = 0; y < canvas_size.y; y++)
        {
            psapi::sfm::Color pixel = layer->getPixel({x, y});
            img->setPixel(x, y, pixel);
        }
    }
    return std::make_unique<ACanvasSnapshot>(std::move(img));
}


template<>
void AMementable<psapi::ICanvasSnapshot>::restore(psapi::ICanvasSnapshot* snapshot)
{
    psapi::ICanvas* canvas = dynamic_cast<psapi::ICanvas *>(psapi::getRootWindow()->getWindowById(psapi::kCanvasWindowId));
    psapi::ILayer* layer = canvas->getLayer(canvas->getActiveLayerIndex());

    ACanvasSnapshot* canvas_snapshot = dynamic_cast<ACanvasSnapshot *>(snapshot);

    psapi::vec2u canvas_size = canvas->getSize();

    for (int x = 0; x < canvas_size.x; x++)
    {
        for (int y = 0; y < canvas_size.y; y++)
        {
            auto pixel = canvas_snapshot->getImage()->getPixel(x, y);

            layer->setPixel({x, y}, pixel);
        }
    }
}
