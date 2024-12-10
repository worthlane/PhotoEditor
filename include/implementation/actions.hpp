#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <deque>

#include "api/api_actions.hpp"
#include "api/api_photoshop.hpp"

class AAction : public psapi::IAction
{
public:
    AAction(const psapi::IRenderWindow* render_window, const psapi::Event& event);

protected:
    const psapi::IRenderWindow* render_window_;
    const psapi::Event& event_;
};

class AUndoableAction : public psapi::IUndoableAction
{
public:
    AUndoableAction(const psapi::IRenderWindow* render_window, const psapi::Event& event);

protected:
    const psapi::IRenderWindow* render_window_;
    const psapi::Event& event_;
};

class ActionController : public psapi::AActionController
{
public:
    ActionController();

    virtual bool execute(std::unique_ptr<psapi::IAction> action) override;

    virtual bool undo() override;
    virtual bool redo() override;
private:
    std::deque<std::unique_ptr<psapi::IUndoableAction>> undo_deque_;
    std::deque<std::unique_ptr<psapi::IUndoableAction>> redo_deque_;
};


#endif // ACTIONS_HPP
