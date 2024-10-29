#ifndef BAR_HPP
#define BAR_HPP

#include "standard/api_bar.hpp"

namespace psapi {

class BarButton : public IBarButton
{
public:
    virtual void  setState(State state) override;
    virtual State getState() const override; // return state
};

class Bar : public IBar
{
public:
    virtual ChildInfo getNextChildInfo() const override;

    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
};

} // namespace psapi

#endif // BAR_HPP
