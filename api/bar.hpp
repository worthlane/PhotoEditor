#ifndef BAR_HPP
#define BAR_HPP

#include "standard/api_bar.hpp"

namespace psapi
{

class SimpleBarButton : public IBarButton
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
