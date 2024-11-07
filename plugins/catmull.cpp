#include <cassert>
#include <iostream>
#include <cmath>

#include "../plugins/catmull.hpp"

static psapi::sfm::vec2i mul(const psapi::sfm::vec2i& x, const double cf)
{
    return psapi::sfm::vec2i(static_cast<int>(static_cast<double>(x.x) * cf),
                             static_cast<int>(static_cast<double>(x.y) * cf));
}

static double hermite00(double t) { return  2*t*t*t - 3*t*t + 1; }
static double hermite10(double t) { return    t*t*t - 2*t*t + t; }
static double hermite01(double t) { return -2*t*t*t + 3*t*t;     }
static double hermite11(double t) { return    t*t*t -   t*t;     }

static psapi::sfm::vec2i catmull_rom(const std::vector<psapi::sfm::vec2i>& coord, const int i, double remain);

//--------------------------------------------------------------------

static psapi::sfm::vec2i catmull_rom(const std::vector<psapi::sfm::vec2i>& coord, const int i, double remain)
{
    psapi::sfm::vec2i m0 = mul((coord[i + 1] - coord[i - 1]), 0.5);
    psapi::sfm::vec2i m1 = mul((coord[i + 2] - coord[i]), 0.5);

    return mul(coord[i], hermite00(remain))     + mul(m0, hermite10(remain)) +
           mul(coord[i + 1], hermite01(remain)) + mul(m1, hermite11(remain));
}


//--------------------------------------------------------------------

InterpolationArray::InterpolationArray(std::vector<psapi::sfm::vec2i>& array) : points_(array)
{
}

//--------------------------------------------------------------------

InterpolationArray::InterpolationArray() : points_()
{
}

//--------------------------------------------------------------------

InterpolationArray::~InterpolationArray()
{
    points_.clear();
}

//--------------------------------------------------------------------

psapi::sfm::vec2i InterpolationArray::getInterpolated(const double num) const
{
    int integrer = floor(num);
    double remain = num - integrer;

    psapi::sfm::vec2i calc_coords = catmull_rom(points_, integrer, remain);

    return calc_coords;
}

//--------------------------------------------------------------------

void InterpolationArray::queue_push(const psapi::sfm::vec2i& point)
{
    for (int i = 0; i < points_.size() - 1; i++)
    {
        points_[i] = points_[i + 1];
    }

    points_[points_.size() - 1] = point;
}

//--------------------------------------------------------------------

psapi::sfm::vec2i& InterpolationArray::operator[](const int num)
{
    return this->points_[num];
}



