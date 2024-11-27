#include <cassert>
#include <iostream>

#include "../plugins/filters/filters.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

void update_point(psapi::ILayer* layer, psapi::ILayer* temp_layer, std::vector<std::vector<bool>>& changed, const psapi::vec2i& pos, const double k, const int radius);
static int apply_contrast(const int color, const double k);

static const size_t CATMULL_LEN = 4;

bool loadPlugin()
{
    std::cout << "filters loaded\n";

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> neg_sprite = psapi::sfm::ISprite::create();
    neg_sprite.get()->setTextureRect(BUTTON_RECT);

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kOptionsBarWindowId));

    auto negative = std::make_unique<PressButton>(kNegativeFilterButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(neg_sprite),
                                               std::make_unique<ContrastAction>(-1, canvas));


    if (tool_bar)
    {
        tool_bar->addWindow(std::move(negative));
    }
}

void unloadPlugin()
{
    delete btn;
}

// ======================================================

ContrastAction::ContrastAction(const double k, psapi::ICanvas* canvas) :
k_(k), canvas_(canvas)
{
}

bool ContrastAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2u canvas_size = canvas_->getSize();

    auto layer_id = canvas_->getActiveLayerIndex();
    auto layer = canvas_->getLayer(layer_id);
    auto temp_layer = canvas_->getTempLayer();
    auto size = canvas_->getSize();

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};
            auto pixel = layer->getPixel(pos);

            pixel.r = apply_contrast(pixel.r, k_);
            pixel.g = apply_contrast(pixel.g, k_);
            pixel.b = apply_contrast(pixel.b, k_);

            temp_layer->setPixel(pos, pixel);

            //update_point(temp_layer, layer, psapi::vec2i(x, y), k_);
        }
    }

    canvas_->cleanTempLayer();

    return true;
}

void update_point(psapi::ILayer* temp_layer, psapi::ILayer* layer, const psapi::vec2i& pos, const double k)
{
    /*int rad2 = radius * radius;

    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            if (pos.x + i < 1 || pos.y + j < 1 ||
                pos.x + i > changed.size() - 1 || pos.y + j > changed[0].size() - 1)
                continue;

            if (i * i + j * j <= rad2 && changed[pos.x + i][pos.y + j] == false)
            {
                psapi::sfm::Color old_pixel = layer->getPixel(pos + psapi::vec2i(i, j));

                int r = 0, g = 0, b = 0;

                for (int x_coverage = -1; x_coverage <= 1; x_coverage++)
                {
                    for (int y_coverage = -1; y_coverage <= 1; y_coverage++)
                    {
                        auto pixel = layer->getPixel(pos + psapi::vec2i(i + x_coverage, j + y_coverage));

                        r += pixel.r;
                        g += pixel.g;
                        b += pixel.b;
                    }
                }

                r /= 9;
                g /= 9;
                b /= 9;

                r = old_pixel.r + (old_pixel.r - r) * 2;
                g = old_pixel.g + (old_pixel.g - g) * 2;
                b = old_pixel.b + (old_pixel.b - b) * 2;

                if (r < 0) r = 0;
                if (g < 0) g = 0;
                if (b < 0) b = 0;

                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                psapi::sfm::Color new_pixel = {r, g, b, old_pixel.a};

                std::cout << static_cast<int>(new_pixel.r) << " " << static_cast<int>(new_pixel.g) << " " << static_cast<int>(new_pixel.b) << " " << static_cast<int>(new_pixel.a) << std::endl;

                temp_layer->setPixel(pos + psapi::vec2i(i, j), new_pixel);

                changed[pos.x + i][pos.y + j] = true;
            }
        }
    }*/
}

static int apply_contrast(const int color, const double k)
{
    int new_color = (color - 128) * k + 128;

    if (new_color < 0)
        new_color = 0;

    else if (new_color > 255)
        new_color = 255;

    return new_color;
}
