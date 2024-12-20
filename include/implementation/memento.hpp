#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "api/api_memento.hpp"
#include "api/api_sfm.hpp"

template<typename SnapshotType>
class AMementable : public psapi::IMementable<SnapshotType>
{
public:
    virtual std::unique_ptr<SnapshotType> save() override;
    virtual void restore(SnapshotType* snapshot) override;
};

class ALayerSnapshot : public psapi::ILayerSnapshot
{
};

class ACanvasSnapshot : public psapi::ICanvasSnapshot
{
public:
    ACanvasSnapshot(std::unique_ptr<psapi::sfm::IImage> img);

    psapi::sfm::IImage* getImage() const;

private:
    std::unique_ptr<psapi::sfm::IImage> img_;

};


#endif // MEMENTO_HPP
