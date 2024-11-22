#ifndef DESIGN_PLUGIN_HPP
#define DESIGN_PLUGIN_HPP

#include "standard/api_sfm.hpp"

static const size_t STD_GUI_RADIUS = 10;

void set_round_texture(psapi::sfm::ISprite* sprite, psapi::sfm::ITexture* texture,
                        const psapi::sfm::IntRect& rect, const size_t radius);

#endif // DESIGN_PLUGIN_HPP
