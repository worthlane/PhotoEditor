#include "implementation/utils.hpp"

psapi::sfm::vec2i operator/(psapi::sfm::vec2i& self, psapi::sfm::vec2f& other)
{
    return psapi::sfm::vec2i(self.x / other.x, self.y / other.y);
}
