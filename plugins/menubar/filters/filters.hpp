#ifndef FILTER_PLUGIN_HPP
#define FILTER_PLUGIN_HPP

#include "api/api_bar.hpp"
#include "api/api_canvas.hpp"

#include "implementation/bar/bar_button.hpp"
#include "implementation/actions.hpp"

class NegativeButton : public TextButton
{
public:
    NegativeButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 const double k, psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;
    psapi::ICanvas* canvas_ = nullptr;

    friend class NegativeAction;

    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> snapshots_;
    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> future_snapshots_;
};

class NegativeAction : public AUndoableAction
{
public:
    NegativeAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, NegativeButton* filter);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

    bool undo(const Key &key);
    bool redo(const Key &key);

private:
    NegativeButton* filter_ = nullptr;
};

class BlurButton : public TextButton
{
public:
    BlurButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class BlurAction;

    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> snapshots_;
    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> future_snapshots_;
};

class BlurAction : public AUndoableAction
{
public:
    BlurAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BlurButton* filter);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

    bool undo(const Key &key);
    bool redo(const Key &key);

private:
    BlurButton* filter_ = nullptr;
};

class UnSharpMaskButton : public TextButton
{
public:
    UnSharpMaskButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    psapi::ICanvas* canvas_ = nullptr;

    friend class UnSharpMaskAction;

    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> snapshots_;
    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> future_snapshots_;
};

class UnSharpMaskAction : public AUndoableAction
{
public:
    UnSharpMaskAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, UnSharpMaskButton* filter);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

    bool undo(const Key &key);
    bool redo(const Key &key);

private:
    UnSharpMaskButton* filter_ = nullptr;
};

class BasReliefButton : public TextButton
{
public:
    BasReliefButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::string& name, psapi::sfm::Color color,
                 const double k, psapi::ICanvas* canvas);

    std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

private:
    double k_;
    psapi::ICanvas* canvas_ = nullptr;

    friend class BasReliefAction;

    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> snapshots_;
    std::vector<std::unique_ptr<psapi::ICanvasSnapshot>> future_snapshots_;
};

class BasReliefAction : public AUndoableAction
{
public:
    BasReliefAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, BasReliefButton* filter);

    bool execute   (const Key& key) override;
    bool isUndoable(const Key& key) override;

    bool undo(const Key &key);
    bool redo(const Key &key);

private:
    BasReliefButton* filter_ = nullptr;
};

static const psapi::wid_t kNegativeFilterButtonId = 233;
static const psapi::wid_t kBasReliefFilterButtonId = 234;
static const psapi::wid_t kBlurFilterButtonId = 235;
static const psapi::wid_t kUnSharpMaskFilterButtonId = 236;

#endif // BRUSH_PLUGIN_HPP
