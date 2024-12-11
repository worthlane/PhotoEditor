#include <cassert>
#include <iostream>

#include "../plugins/canvas/canvas.hpp"

#include "api/api_photoshop.hpp"
#include "implementation/canvas/canvas.hpp"

static const char* BACKGROUND_TEXTURE = "assets/textures/light_gray.jpg";
static const char* NORMAL_TEXTURE = "assets/textures/mid_gray.jpg";
static const char* HOVER_TEXTURE = "assets/textures/gray.jpg";
static const char* ACTIVE_TEXTURE = "assets/textures/dark_gray.jpg";

static const psapi::wid_t kCanvasHorizontalScrollBar = 230;
static const psapi::wid_t kCanvasVerticalScrollBar = 231;

bool onLoadPlugin()
{
    std::cout << "canvas loaded\n";

    auto canvas_rect = psapi::getCanvasIntRect();

    auto canvas = std::make_unique<Canvas>(canvas_rect.pos,
                                           canvas_rect.size);

    std::unique_ptr<psapi::sfm::ITexture> back_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> normal_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> active_hor = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> hover_hor = psapi::sfm::ITexture::create();

    back_hor->loadFromFile(BACKGROUND_TEXTURE);
    normal_hor->loadFromFile(NORMAL_TEXTURE);
    active_hor->loadFromFile(ACTIVE_TEXTURE);
    hover_hor->loadFromFile(HOVER_TEXTURE);

    auto hor_scrollbar = std::make_unique<HorizontalScrollBar>(kCanvasHorizontalScrollBar,
                                                                canvas_rect.pos + psapi::sfm::vec2i(0, canvas_rect.size.y),
                                                                psapi::sfm::vec2u(canvas_rect.size.x, 20),
                                                                std::move(back_hor), std::move(normal_hor),
                                                                std::move(hover_hor), std::move(active_hor),
                                                                canvas.get());

    std::unique_ptr<psapi::sfm::ITexture> back_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> normal_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> active_ver = psapi::sfm::ITexture::create();
    std::unique_ptr<psapi::sfm::ITexture> hover_ver = psapi::sfm::ITexture::create();

    back_ver->loadFromFile(BACKGROUND_TEXTURE);
    normal_ver->loadFromFile(NORMAL_TEXTURE);
    active_ver->loadFromFile(ACTIVE_TEXTURE);
    hover_ver->loadFromFile(HOVER_TEXTURE);

    auto ver_scrollbar = std::make_unique<VerticalScrollBar>(kCanvasVerticalScrollBar,
                                                            canvas_rect.pos + psapi::sfm::vec2i(canvas_rect.size.x, 0),
                                                            psapi::sfm::vec2u(20, canvas_rect.size.y),
                                                            std::move(back_ver), std::move(normal_ver),
                                                            std::move(hover_ver), std::move(active_ver),
                                                            canvas.get());

    auto root = psapi::getRootWindow();

    root->addWindow(std::move(canvas));
    root->addWindow(std::move(hor_scrollbar));
    root->addWindow(std::move(ver_scrollbar));

    return true;
}

void onUnloadPlugin()
{
}
