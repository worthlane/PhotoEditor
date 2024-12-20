#include <iostream>
#include <cassert>

#include "implementation/actions.hpp"

static const size_t MAX_DEQUE_SIZE = 32;

AAction::AAction(const psapi::IRenderWindow* render_window, const psapi::Event& event) : render_window_(render_window), event_(event)
{}

AUndoableAction::AUndoableAction(const psapi::IRenderWindow* render_window, const psapi::Event& event) : render_window_(render_window), event_(event)
{}

// ************* ACTION CONTROLLER ****************

psapi::AActionController* psapi::getActionController()
{
    static ActionController actionController;

    return &actionController;
}

ActionController::ActionController()
{}

bool ActionController::execute(std::unique_ptr<psapi::IAction> action)
{
    bool result = actionExecute(action.get());

    if (isUndoableAction(action.get()))
    {
        psapi::IUndoableAction* act = dynamic_cast<psapi::IUndoableAction*>(action.release());
        assert(act);

        undo_deque_.push_back(std::unique_ptr<psapi::IUndoableAction>(act));

        if (undo_deque_.size() > MAX_DEQUE_SIZE)
        {
            undo_deque_.front().release();
            undo_deque_.pop_front();
        }

    }

    return result;
}

bool ActionController::undo()
{
    if (undo_deque_.empty())
        return false;

    std::unique_ptr<psapi::IUndoableAction> act = std::move(undo_deque_.back());

    bool result = false;

    result = actionUndo(act.get());

    undo_deque_.pop_back();
    redo_deque_.push_back(std::move(act));

    if (redo_deque_.size() > MAX_DEQUE_SIZE)
    {
        redo_deque_.front().release();
        redo_deque_.pop_front();
    }

    return result;
}

bool ActionController::redo()
{
    if (redo_deque_.empty())
        return false;

    std::unique_ptr<psapi::IUndoableAction> act = std::move(redo_deque_.back());

    bool result = actionRedo(act.get());

    redo_deque_.pop_back();
    undo_deque_.push_back(std::move(act));

    if (undo_deque_.size() > MAX_DEQUE_SIZE)
    {
        undo_deque_.front().release();
        undo_deque_.pop_front();
    }

    return result;
}

IdleAction::IdleAction(const psapi::IRenderWindow* render_window, const psapi::Event& event) : AAction(render_window, event)
{}

bool IdleAction::execute(const Key& key)
{
    return false;
}

bool IdleAction::isUndoable(const Key& key)
{
    return false;
}
