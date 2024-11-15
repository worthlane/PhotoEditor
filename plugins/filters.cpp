#include <cassert>
#include <iostream>

#include "../plugins/filters.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static const psapi::sfm::IntRect BUTTON_RECT = {0, 0, 90, 90};

void update_point(psapi::ILayer* layer, psapi::ILayer* temp_layer, std::vector<std::vector<bool>>& changed, const psapi::vec2i& pos, const double k, const int radius);
static int apply_filter(const int color, const double k);
static void clear_bool_array(std::vector<std::vector<bool>>& array);

static const size_t CATMULL_LEN = 4;

bool loadPlugin()
{
    std::cout << "filters loaded\n";

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> neg_sprite = psapi::sfm::ISprite::create();
    neg_sprite.get()->setTextureRect(BUTTON_RECT);

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto negative = std::make_unique<SwitchButton>(kNegativeFilterButtonId,
                                                psapi::vec2i(19, 346),
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(neg_sprite),
                                               std::make_unique<NegativeAction>(-1, 40, canvas));


    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kToolBarWindowId));

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

NegativeAction::NegativeAction(const double k, const size_t radius, psapi::ICanvas* canvas, const bool scale_related) :
k_(k), radius_(radius), array_(), scale_related_(scale_related), canvas_(canvas)
{
    auto size = canvas_->getSize();
    changed_.resize(size.x, std::vector<bool>(size.y));

    for (size_t i = 0; i < size.x; i++)
    {
        for (size_t j = 0; j < size.y; j++)
        {
            changed_[i][j] = false;
        }
    }
}

bool NegativeAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2i mouse_pos = canvas_->getMousePosition();
    bool LMB_down = psapi::sfm::Mouse::isButtonPressed(psapi::sfm::Mouse::Button::Left);

    psapi::vec2u canvas_size = canvas_->getSize();

    auto layer_id = canvas_->getActiveLayerIndex();
    auto layer = canvas_->getLayer(layer_id);
    auto temp_layer = canvas_->getTempLayer();

    if (LMB_down)
    {
        psapi::vec2i pos = mouse_pos;

        if (array_.size() < CATMULL_LEN)
        {
            array_.push_back(pos);
        }
        else
        {
            array_.queue_push(pos);

            double delta = 0.001 * static_cast<double>(radius_);
            double max_point = static_cast<double>(CATMULL_LEN - 2);

            //int radius = static_cast<int>(scale_related_ ? static_cast<double>(radius_) / scale.x : radius_);

            for (double i = 1; i < max_point; i += delta)
            {
                update_point(layer, temp_layer, changed_, array_.getInterpolated(i), k_, radius_);
            }
        }
    }
    else
    {
        array_.clear();
        if (array_.size())
            canvas_->cleanTempLayer();

        clear_bool_array(changed_);
    }

    return true;
}

void update_point(psapi::ILayer* layer, psapi::ILayer* temp_layer, std::vector<std::vector<bool>>& changed, const psapi::vec2i& pos,
                      const double k, const int radius)
{
    int rad2 = radius * radius;

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
    }
}

static int apply_filter(const int color, const double k)
{
    int new_color = (color - 128) * k + 128;

    if (new_color < 0)
        new_color = 0;

    else if (new_color > 255)
        new_color = 255;

    return new_color;
}

static void clear_bool_array(std::vector<std::vector<bool>>& array)
{
    for (size_t i = 0; i < array.size(); i++)
    {
        for (size_t j = 0; j < array[i].size(); j++)
        {
            array[i][j] = false;
        }
    }
}
