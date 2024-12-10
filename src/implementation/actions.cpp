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
        psapi::IUndoableAction* act = dynamic_cast<psapi::IUndoableAction*>(action.get());
        if (!act) return result;

        undo_deque_.push_back(std::unique_ptr<psapi::IUndoableAction>(act));

        if (undo_deque_.size() > MAX_DEQUE_SIZE)
            undo_deque_.pop_front();
    }

    return result;
}

bool ActionController::undo()
{
    if (undo_deque_.empty())
        return false;

    psapi::IUndoableAction* act = undo_deque_.back().get();
    if (!act) return false;

    bool result = actionUndo(act);
    undo_deque_.pop_back();

    if (!isUndoableAction(act))
    {
        redo_deque_.push_back(std::unique_ptr<psapi::IUndoableAction>(act));

        if (redo_deque_.size() > MAX_DEQUE_SIZE)
            redo_deque_.pop_front();
    }

    return result;
}

bool ActionController::redo()
{
    if (redo_deque_.empty())
        return false;

    psapi::IUndoableAction* act = redo_deque_.back().get();
    if (!act) return false;

    bool result = actionRedo(act);
    redo_deque_.pop_back();

    if (!isUndoableAction(act))
    {
        undo_deque_.push_back(std::unique_ptr<psapi::IUndoableAction>(act));

        if (undo_deque_.size() > MAX_DEQUE_SIZE)
            undo_deque_.pop_front();
    }

    return result;
}
