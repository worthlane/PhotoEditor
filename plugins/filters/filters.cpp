#include <cassert>
#include <iostream>

#include "../plugins/filters/filters.hpp"
#include "../plugins/colors.hpp"

static psapi::sfm::ITexture* btn = nullptr;

void update_point(psapi::ILayer* layer, psapi::ILayer* temp_layer, std::vector<std::vector<bool>>& changed, const psapi::vec2i& pos, const double k, const int radius);
static int apply_contrast(const int color, const double k);

static const size_t CATMULL_LEN = 4;

static psapi::sfm::Color calculate_gauss_blur(psapi::ILayer* layer, const psapi::vec2u& size, const psapi::vec2i pos, const std::vector<std::vector<double>>& gauss_matrix);

bool loadPlugin()
{
    std::cout << "filters loaded\n";

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> neg_sprite = psapi::sfm::ISprite::create();
    neg_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> bar_sprite = psapi::sfm::ISprite::create();
    bar_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> sharp_sprite = psapi::sfm::ISprite::create();
    sharp_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> blur_sprite = psapi::sfm::ISprite::create();
    blur_sprite.get()->setTextureRect(BUTTON_RECT);

    auto root = psapi::getRootWindow();

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto tool_bar = static_cast<psapi::IBar*>(root->getWindowById(psapi::kOptionsBarWindowId));

    auto negative = std::make_unique<PressButton>(kNegativeFilterButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(neg_sprite),
                                               std::make_unique<ContrastAction>(-1, canvas));

    auto barel = std::make_unique<PressButton>(kBareliefFilterButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(bar_sprite),
                                               std::make_unique<BareliefAction>(-1, canvas));

    auto blur = std::make_unique<PressButton>(kBlurFilterButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(blur_sprite),
                                               std::make_unique<BlurAction>(canvas));

    auto sharp = std::make_unique<PressButton>(kSharpFilterButtonId, tool_bar,
                                               psapi::vec2u(BUTTON_RECT.width, BUTTON_RECT.height),
                                               std::move(sharp_sprite),
                                               std::make_unique<SharpAction>(canvas));


    if (tool_bar)
    {
        tool_bar->addWindow(std::move(negative));
        tool_bar->addWindow(std::move(barel));
        tool_bar->addWindow(std::move(blur));
        tool_bar->addWindow(std::move(sharp));
    }
}

void unloadPlugin()
{
    delete btn;
}

// ======================================================

SharpAction::SharpAction(psapi::ICanvas* canvas) :
canvas_(canvas)
{
}

bool SharpAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2u canvas_size = canvas_->getSize();

    auto layer_id = canvas_->getActiveLayerIndex();
    auto layer = canvas_->getLayer(layer_id);
    auto temp_layer = canvas_->getTempLayer();
    auto size = canvas_->getSize();

    static const std::vector<std::vector<double>> GAUSS_MATRIX = {{0.025, 0.1, 0.025},
                                                                  {  0.1, 0.5,   0.1},
                                                                  {0.025, 0.1, 0.025}};

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};

            psapi::sfm::Color blur = calculate_gauss_blur(layer, canvas_size, pos, GAUSS_MATRIX);
            auto pixel = layer->getPixel(pos);

            float r = static_cast<float>(pixel.r) + static_cast<float>(pixel.r - blur.r) * 2;
            float g = static_cast<float>(pixel.g) + static_cast<float>(pixel.g - blur.g) * 2;
            float b = static_cast<float>(pixel.b) + static_cast<float>(pixel.b - blur.b) * 2;

            r = std::min(std::max(r, 0.0f), 255.0f);
            g = std::min(std::max(g, 0.0f), 255.0f);
            b = std::min(std::max(b, 0.0f), 255.0f);

            temp_layer->setPixel({x, y}, {static_cast<uint8_t>(r),
                                          static_cast<uint8_t>(g),
                                          static_cast<uint8_t>(b),
                                          blur.a});
        }
    }

    canvas_->cleanTempLayer();

    return true;
}

BlurAction::BlurAction(psapi::ICanvas* canvas) :
canvas_(canvas)
{
}

bool BlurAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2u canvas_size = canvas_->getSize();

    auto layer_id = canvas_->getActiveLayerIndex();
    auto layer = canvas_->getLayer(layer_id);
    auto temp_layer = canvas_->getTempLayer();
    auto size = canvas_->getSize();

    static const std::vector<std::vector<double>> GAUSS_MATRIX = {{0.025, 0.1, 0.025},
                                                                  {  0.1, 0.5,   0.1},
                                                                  {0.025, 0.1, 0.025}};


    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};

            psapi::sfm::Color blur = calculate_gauss_blur(layer, canvas_size, pos, GAUSS_MATRIX);

            temp_layer->setPixel({x, y}, blur);
        }
    }

    canvas_->cleanTempLayer();

    return true;
}

BareliefAction::BareliefAction(const double k, psapi::ICanvas* canvas) :
k_(k), canvas_(canvas)
{
}

bool BareliefAction::operator()(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    psapi::vec2u canvas_size = canvas_->getSize();

    auto layer_id = canvas_->getActiveLayerIndex();
    auto layer = canvas_->getLayer(layer_id);
    auto temp_layer = canvas_->getTempLayer();
    auto size = canvas_->getSize();

    const int offset = 1;

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};
            auto pixel = layer->getPixel(pos);

            pixel.r = apply_contrast(pixel.r, k_);
            pixel.g = apply_contrast(pixel.g, k_);
            pixel.b = apply_contrast(pixel.b, k_);

            auto offset_pixel = layer->getPixel({x, y});
            if (x + offset < size.x && y + offset < size.y)
                offset_pixel = layer->getPixel({x + offset, y + offset});
            else if (x + offset < size.x)
                offset_pixel = layer->getPixel({x + offset, y});
            else if (y + offset < size.y)
                offset_pixel = layer->getPixel({x, y + offset});

            psapi::sfm::Color result = {(pixel.r + offset_pixel.r) / 2,
                                        (pixel.g + offset_pixel.g) / 2,
                                        (pixel.b + offset_pixel.b) / 2,
                                        pixel.a};

            temp_layer->setPixel(pos, result);
        }
    }

    canvas_->cleanTempLayer();

    return true;
}

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
        }
    }

    canvas_->cleanTempLayer();

    return true;
}

static psapi::sfm::Color calculate_gauss_blur(psapi::ILayer* layer, const psapi::vec2u& size, const psapi::vec2i pos, const std::vector<std::vector<double>>& gauss_matrix)
{
    double r = 0;
    double g = 0;
    double b = 0;

    static const psapi::vec2u gauss_size = {gauss_matrix.size(), gauss_matrix[0].size()};
    static const psapi::vec2i gauss_center = {gauss_size.x / 2, gauss_size.y / 2};

    for (int x = 0; x < gauss_size.x; x++)
    {
        for (int y = 0; y < gauss_size.y; y++)
        {
            psapi::vec2i gauss_pos = psapi::vec2i(x + pos.x - gauss_center.x, y + pos.y - gauss_center.y);

            psapi::sfm::Color pixel = {255, 255, 255, 255};

            if (gauss_pos.x > 0 && gauss_pos.x < size.x && gauss_pos.y > 0 && gauss_pos.y < size.y)
                pixel = layer->getPixel(gauss_pos);

            r += pixel.r * gauss_matrix[x][y];
            g += pixel.g * gauss_matrix[x][y];
            b += pixel.b * gauss_matrix[x][y];
        }
    }

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    return {static_cast<uint8_t>(r),
            static_cast<uint8_t>(g),
            static_cast<uint8_t>(b),
            layer->getPixel(pos).a};
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
