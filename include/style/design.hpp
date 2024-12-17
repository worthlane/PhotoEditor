#ifndef DESIGN_PLUGIN_HPP
#define DESIGN_PLUGIN_HPP

#include "api/api_sfm.hpp"

static const size_t STD_GUI_RADIUS = 4;
static const size_t STD_GUI_WIDTH  = 1;
static const double DARK_COEF = 0.7;

void make_styled_sprite(psapi::sfm::ISprite* sprite, psapi::sfm::ITexture* texture,
                        const psapi::sfm::IntRect& rect, const double dark_coef = DARK_COEF, const psapi::sfm::vec2u params = {STD_GUI_RADIUS, STD_GUI_WIDTH});

#endif // DESIGN_PLUGIN_HPP
