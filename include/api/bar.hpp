#ifndef BAR_HPP
#define BAR_HPP

#include "standard/api_bar.hpp"

namespace psapi
{

class Bar : public IBar
{
public:
    virtual wid_t getId() const override { return kInvalidWindowId; };

    virtual ChildInfo getNextChildInfo() const override;

    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
};

class BarButton : public IBarButton
{
public:
    virtual wid_t getId() const override { return kInvalidWindowId; };
    
    virtual void  setState(State state) override { state_ = state; };
    virtual State getState() const override      { return state_; };

private:
    State state_ = State::Normal;
};

} // namespace psapi

#endif // BAR_HPP
