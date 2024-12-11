#include <iostream>

#include "style/design.hpp"

static void darker_border(psapi::sfm::IImage* img, const psapi::sfm::IntRect& rect, const double dark_coef);
static void round_corner(psapi::sfm::IImage* img,
                         const psapi::sfm::IntRect& corner, const psapi::sfm::vec2i& center_pos,
                         const double dark_coef, const psapi::sfm::vec2u params);
static void darker_pixel(psapi::sfm::IImage* img, psapi::sfm::vec2u pos, const double dark_coef);

void make_styled_sprite(psapi::sfm::ISprite* sprite, psapi::sfm::ITexture* texture,
                         const psapi::sfm::IntRect& rect, const double dark_coef, const psapi::sfm::vec2u params)
{
    size_t radius = params.x;
    size_t width  = params.y;

    std::unique_ptr<psapi::sfm::IImage> img = texture->copyToImage();

    psapi::sfm::IntRect top_left_corner = {rect.pos, {radius, radius}};
    psapi::sfm::vec2i   top_left_center = {top_left_corner.pos.x + radius, top_left_corner.pos.y + radius};

    psapi::sfm::IntRect low_left_corner = {{rect.pos.x, rect.pos.y + rect.size.y - radius}, {radius, radius}};
    psapi::sfm::vec2i   low_left_center = {low_left_corner.pos.x + radius, low_left_corner.pos.y};

    psapi::sfm::IntRect top_right_corner = {{rect.pos.x + rect.size.x - radius, rect.pos.y}, {radius, radius}};
    psapi::sfm::vec2i   top_right_center = {top_right_corner.pos.x, top_right_corner.pos.y + radius};

    psapi::sfm::IntRect low_right_corner = {{rect.pos.x + rect.size.x - radius, rect.pos.y + rect.size.y - radius}, {radius, radius}};
    psapi::sfm::vec2i   low_right_center = {low_right_corner.pos.x, low_right_corner.pos.y};

    round_corner(img.get(), top_left_corner, top_left_center, dark_coef, params);
    round_corner(img.get(), top_right_corner, top_right_center, dark_coef, params);
    round_corner(img.get(), low_left_corner, low_left_center, dark_coef, params);
    round_corner(img.get(), low_right_corner, low_right_center, dark_coef, params);

    psapi::sfm::IntRect upper_border = {{rect.pos.x + radius, rect.pos.y},
                                        {rect.size.x - (2 * radius),  width}};

    psapi::sfm::IntRect lower_border = {{rect.pos.x + radius, rect.pos.y + rect.size.y - width},
                                        {rect.size.x - (2 * radius),  width}};

    psapi::sfm::IntRect left_border  = {{rect.pos.x, rect.pos.y + radius},
                                        {width, rect.size.y - (2 * radius)}};

    psapi::sfm::IntRect right_border = {{rect.pos.x + rect.size.x - width,  rect.pos.y + radius},
                                        {width, rect.size.y - (2 * radius)}};

    darker_border(img.get(), upper_border, dark_coef);
    darker_border(img.get(), lower_border, dark_coef);
    darker_border(img.get(), left_border, dark_coef);
    darker_border(img.get(), right_border, dark_coef);

    texture->update(img.get());

    sprite->setTextureRect(rect);
    sprite->setTexture(texture);
}

static void darker_border(psapi::sfm::IImage* img, const psapi::sfm::IntRect& rect, const double dark_coef)
{
    int y_limit = rect.pos.y + rect.size.y;
    int x_limit = rect.pos.x + rect.size.x;

    for (int y = rect.pos.y; y < y_limit; y++)
    {
        for (int x = rect.pos.x; x < x_limit; x++)
        {
            darker_pixel(img, psapi::sfm::vec2u(x, y), dark_coef);
        }
    }
}

static void round_corner(psapi::sfm::IImage* img,
                         const psapi::sfm::IntRect& corner, const psapi::sfm::vec2i& center_pos,
                         const double dark_coef, const psapi::sfm::vec2u params)
{
    int outer_radius = params.x;
    int inner_radius = params.x - params.y;

    int outer_radius2 = outer_radius * outer_radius;
    int inner_radius2 = inner_radius * inner_radius;

    int y_limit = corner.pos.y + corner.size.y;
    int x_limit = corner.pos.x + corner.size.x;

    for (int y = corner.pos.y; y < y_limit; y++)
    {
        for (int x = corner.pos.x; x < x_limit; x++)
        {
            int x_dist = x - center_pos.x;
            int y_dist = y - center_pos.y;

            int dist2 = x_dist * x_dist + y_dist * y_dist;

            if (dist2 > outer_radius2)
            {
                img->setPixel({x, y}, psapi::sfm::Color(0, 0, 0, 0));
            }
            else if (dist2 >= inner_radius2)
            {
                darker_pixel(img, psapi::sfm::vec2u(x, y), dark_coef);
            }
        }
    }
}

static void darker_pixel(psapi::sfm::IImage* img, psapi::sfm::vec2u pos, const double dark_coef)
{
    psapi::sfm::Color pix = img->getPixel(pos);

    pix.r = dark_coef * static_cast<double>(pix.r);
    pix.g = dark_coef * static_cast<double>(pix.g);
    pix.b = dark_coef * static_cast<double>(pix.b);
    pix.a = dark_coef * static_cast<double>(pix.a);

    img->setPixel(pos, pix);
}
