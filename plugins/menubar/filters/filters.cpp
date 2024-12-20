#include <cassert>
#include <iostream>

#include "../plugins/menubar/filters/filters.hpp"

void update_point(psapi::ILayer* layer, psapi::ILayer* temp_layer, std::vector<std::vector<bool>>& changed, const psapi::vec2i& pos, const double k, const int radius);
static int apply_contrast(const int color, const double k);

static const size_t CATMULL_LEN = 4;

static psapi::sfm::Color calculate_gauss_blur(psapi::ILayer* layer, const psapi::vec2u& size, const psapi::vec2i pos, const std::vector<std::vector<double>>& gauss_matrix);

// ======================================================

NegativeButton::NegativeButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                                std::string& name, psapi::sfm::Color color,
                                const double k, psapi::ICanvas* canvas) :
                 TextButton(id, bar, pos, size, name, color), k_(k), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> NegativeButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<NegativeAction>(renderWindow, event, this);
}

NegativeAction::NegativeAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, NegativeButton* filter) :
                                AUndoableAction(render_window, event), filter_(filter)
{}

bool NegativeAction::execute(const Key& key)
{
    auto canvas = filter_->canvas_;
    auto k      = filter_->k_;

    psapi::vec2u canvas_size = canvas->getSize();

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto size = canvas->getSize();

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};
            auto pixel = layer->getPixel(pos);

            pixel.r = apply_contrast(pixel.r, k);
            pixel.g = apply_contrast(pixel.g, k);
            pixel.b = apply_contrast(pixel.b, k);

            temp_layer->setPixel(pos, pixel);
        }
    }

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));
    canvas->cleanTempLayer();

    return true;
}

bool NegativeAction::isUndoable(const Key& key)
{
    return true;
}

bool NegativeAction::undo(const Key &key)
{
    if (filter_->snapshots_.empty())
        return false;

    filter_->future_snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->snapshots_.back().release();

    filter_->snapshots_.pop_back();

    filter_->canvas_->restore(snapshot);

    return true;
}

bool NegativeAction::redo(const Key &key)
{
    if (filter_->future_snapshots_.empty())
        return false;

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->future_snapshots_.back().release();
    filter_->future_snapshots_.pop_back();
    filter_->canvas_->restore(snapshot);

    return true;
}

BasReliefButton::BasReliefButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 const double k, psapi::ICanvas* canvas) :
                 TextButton(id, bar, pos, size, name, color), k_(k), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> BasReliefButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<BasReliefAction>(renderWindow, event, this);
}

BasReliefAction::BasReliefAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BasReliefButton* filter) :
                                AUndoableAction(render_window, event), filter_(filter)
{}

bool BasReliefAction::execute(const Key& key)
{
    auto canvas = filter_->canvas_;
    auto k = filter_->k_;

    psapi::vec2u canvas_size = canvas->getSize();

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto size = canvas->getSize();

    const int offset = 4;

    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.y; y++)
        {
            psapi::vec2i pos = {x, y};
            auto pixel = layer->getPixel(pos);

            pixel.r = apply_contrast(pixel.r, k);
            pixel.g = apply_contrast(pixel.g, k);
            pixel.b = apply_contrast(pixel.b, k);

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

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));
    canvas->cleanTempLayer();

    return true;
}

bool BasReliefAction::isUndoable(const Key& key)
{
    return true;
}

bool BasReliefAction::undo(const Key &key)
{
    if (filter_->snapshots_.empty())
        return false;

    filter_->future_snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->snapshots_.back().release();

    filter_->snapshots_.pop_back();

    filter_->canvas_->restore(snapshot);

    return true;
}

bool BasReliefAction::redo(const Key &key)
{
    if (filter_->future_snapshots_.empty())
        return false;

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->future_snapshots_.back().release();
    filter_->future_snapshots_.pop_back();
    filter_->canvas_->restore(snapshot);

    return true;
}

BlurButton::BlurButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas) :
                 TextButton(id, bar, pos, size, name, color), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> BlurButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<BlurAction>(renderWindow, event, this);
}

BlurAction::BlurAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BlurButton* filter) :
                                AUndoableAction(render_window, event), filter_(filter)
{}

bool BlurAction::execute(const Key& key)
{
    auto canvas = filter_->canvas_;

    psapi::vec2u canvas_size = canvas->getSize();

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto size = canvas->getSize();

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

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));
    canvas->cleanTempLayer();

    return true;
}

bool BlurAction::isUndoable(const Key& key)
{
    return true;
}

bool BlurAction::undo(const Key &key)
{
    if (filter_->snapshots_.empty())
        return false;

    filter_->future_snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->snapshots_.back().release();

    filter_->snapshots_.pop_back();

    filter_->canvas_->restore(snapshot);

    return true;
}

bool BlurAction::redo(const Key &key)
{
    if (filter_->future_snapshots_.empty())
        return false;

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->future_snapshots_.back().release();
    filter_->future_snapshots_.pop_back();
    filter_->canvas_->restore(snapshot);

    return true;
}

UnSharpMaskButton::UnSharpMaskButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas) :
                 TextButton(id, bar, pos, size, name, color), canvas_(canvas)
{}

std::unique_ptr<psapi::IAction> UnSharpMaskButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != PressButton::State::Released)
        return std::make_unique<IdleAction>(renderWindow, event);

    return std::make_unique<UnSharpMaskAction>(renderWindow, event, this);
}

UnSharpMaskAction::UnSharpMaskAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, UnSharpMaskButton* filter) :
                                AUndoableAction(render_window, event), filter_(filter)
{}

bool UnSharpMaskAction::execute(const Key& key)
{
    auto canvas = filter_->canvas_;

    psapi::vec2u canvas_size = canvas->getSize();

    auto layer_id = canvas->getActiveLayerIndex();
    auto layer = canvas->getLayer(layer_id);
    auto temp_layer = canvas->getTempLayer();
    auto size = canvas->getSize();

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

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));
    canvas->cleanTempLayer();

    return true;
}

bool UnSharpMaskAction::isUndoable(const Key& key)
{
    return true;
}

bool UnSharpMaskAction::undo(const Key &key)
{
    if (filter_->snapshots_.empty())
        return false;

    filter_->future_snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->snapshots_.back().release();

    filter_->snapshots_.pop_back();

    filter_->canvas_->restore(snapshot);

    return true;
}

bool UnSharpMaskAction::redo(const Key &key)
{
    if (filter_->future_snapshots_.empty())
        return false;

    filter_->snapshots_.push_back(std::move(filter_->canvas_->save()));

    psapi::ICanvasSnapshot* snapshot = filter_->future_snapshots_.back().release();
    filter_->future_snapshots_.pop_back();
    filter_->canvas_->restore(snapshot);

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
