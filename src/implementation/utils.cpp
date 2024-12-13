#include <cassert>

#include "implementation/utils.hpp"

psapi::sfm::vec2i operator/(psapi::sfm::vec2i& self, psapi::sfm::vec2f& other)
{
    return psapi::sfm::vec2i(self.x / other.x, self.y / other.y);
}

using psapi::sfm::Color;

Color Color::getStandardColor(Color::Type type)
{
    switch (type)
    {
        case Color::Type::Black:        return Color(0, 0, 0);
        case Color::Type::White:        return Color(255, 255, 255);
        case Color::Type::Red:          return Color(255, 0, 0);
        case Color::Type::Green:        return Color(0, 255, 0);
        case Color::Type::Blue:         return Color(0, 0, 255);
        case Color::Type::Yellow:       return Color(255, 255, 0);
        case Color::Type::Magenta:      return Color(255, 0, 255);
        case Color::Type::Cyan:         return Color(0, 255, 255);
        case Color::Type::Transparent:  return Color(0, 0, 0, 0);
        default:                        assert(0 && "No color");
    }
}
