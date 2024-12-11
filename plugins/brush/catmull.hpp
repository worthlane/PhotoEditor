#ifndef CATMULL_HPP
#define CATMULL_HPP

#include "api/api_system.hpp"

class InterpolationArray
{
    public:
        InterpolationArray();
        InterpolationArray(std::vector<psapi::sfm::vec2i>& array);
        ~InterpolationArray();

        size_t size() const { return points_.size(); }

        psapi::sfm::vec2i getInterpolated(const double num) const;

        psapi::sfm::vec2i& operator[](const int num);

        void queue_push(const psapi::sfm::vec2i& point);

        void clear()
        {
            points_.clear();
        }

        void push_back(const psapi::sfm::vec2i& point)
        {
            points_.push_back(point);
        }

    private:
        std::vector<psapi::sfm::vec2i> points_;
};

#endif // CATMULL_HPP
