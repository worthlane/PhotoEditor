#ifndef BAR_HPP
#define BAR_HPP

#include "standard/api_bar.hpp"

namespace psapi {

class ABarButton : public IBarButton
{
public:
    virtual void  setState(State state) override { state_ = state; };
    virtual State getState() const override      { return state_; };

private:
    State state_ = State::Normal;
};

class ABar : public IBar
{
public:
    virtual ChildInfo getNextChildInfo() const override;

    virtual void finishButtonDraw(IRenderWindow* renderWindow, const IBarButton* button) const override;
};

} // namespace psapi

#endif // BAR_HPP
