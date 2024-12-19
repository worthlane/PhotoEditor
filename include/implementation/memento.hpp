#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "api/api_memento.hpp"

/*template<typename SnapshotType>
class AMementable : public psapi::IMementable<SnapshotType>
{
public:
    virtual std::unique_ptr<SnapshotType> save() = 0;
    virtual void restore(SnapshotType* snapshot) = 0;
private:
    std::vector<Color> pixels_;
};

class ALayerSnapshot : public psapi::ILayerSnapshot
{
};

class ACanvasSnapshot : public psapi::ICanvasSnapshot
{
};*/

#endif // MEMENTO_HPP
