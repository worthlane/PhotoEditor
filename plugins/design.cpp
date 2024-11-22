#include <iostream>

#include "../plugins/design.hpp"

static void round_corner(psapi::sfm::IImage* img,
                         const psapi::sfm::IntRect& corner, const psapi::sfm::vec2i& center_pos,
                         const size_t radius);

void set_round_texture(psapi::sfm::ISprite* sprite, psapi::sfm::ITexture* texture,
                        const psapi::sfm::IntRect& rect, const size_t radius)
{
    std::unique_ptr<psapi::sfm::IImage> img = texture->copyToImage();

    psapi::sfm::IntRect top_left_corner = {rect.top_x, rect.top_y, radius, radius};
    psapi::sfm::vec2i   top_left_center = {top_left_corner.top_x + radius, top_left_corner.top_y + radius};

    psapi::sfm::IntRect low_left_corner = {rect.top_x, rect.top_y + rect.height - radius, radius, radius};
    psapi::sfm::vec2i   low_left_center = {low_left_corner.top_x + radius, low_left_corner.top_y};

    psapi::sfm::IntRect top_right_corner = {rect.top_x + rect.width - radius, rect.top_y, radius, radius};
    psapi::sfm::vec2i   top_right_center = {top_right_corner.top_x, top_right_corner.top_y + radius};

    psapi::sfm::IntRect low_right_corner = {rect.top_x + rect.width - radius, rect.top_y + rect.height - radius, radius, radius};
    psapi::sfm::vec2i   low_right_center = {low_right_corner.top_x, low_right_corner.top_y};

    round_corner(img.get(), top_left_corner, top_left_center, radius);
    round_corner(img.get(), top_right_corner, top_right_center, radius);
    round_corner(img.get(), low_left_corner, low_left_center, radius);
    round_corner(img.get(), low_right_corner, low_right_center, radius);

    texture->update(img.get());

    sprite->setTextureRect(rect);
    sprite->setTexture(texture);
}

static void round_corner(psapi::sfm::IImage* img,
                         const psapi::sfm::IntRect& corner, const psapi::sfm::vec2i& center_pos,
                         const size_t radius)
{
    int radius2 = radius * radius;

    int y_limit = corner.top_y + corner.height;
    int x_limit = corner.top_x + corner.width;

    for (int y = corner.top_y; y < y_limit; y++)
    {
        for (int x = corner.top_x; x < x_limit; x++)
        {
            int x_dist = x - center_pos.x;
            int y_dist = y - center_pos.y;

            if (x_dist * x_dist + y_dist * y_dist > radius2)
            {
                img->setPixel({x, y}, psapi::sfm::Color(0, 0, 0, 0));
            }
        }
    }
}
