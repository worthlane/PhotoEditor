#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "api/api_sfm.hpp"
#include "implementation/photoshop.hpp"

class Scrollable
{
public:
    virtual bool updateScale(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) = 0;
    virtual psapi::sfm::vec2u getObjectSize() const = 0;

    psapi::sfm::vec2i getCoordStart() const;
    psapi::sfm::vec2f getScale() const;

    void setCoordStart(psapi::sfm::vec2i coord_start);
    void setScale(psapi::sfm::vec2f scale);

protected:
    psapi::sfm::vec2i coord_start_ = {0, 0};
    psapi::sfm::vec2f scale_       = {1, 1};
};

class AScrollBar : public AWindow
{
public:
    enum class State
    {
        Normal,
        Hover,
        Active,
    };

    AScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
               std::unique_ptr<psapi::sfm::ITexture> background,
               std::unique_ptr<psapi::sfm::ITexture> normal,
               std::unique_ptr<psapi::sfm::ITexture> hover,
               std::unique_ptr<psapi::sfm::ITexture> active,
               Scrollable* object);

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

protected:
    Scrollable* object_;

    psapi::vec2i scroller_pos_;
    psapi::vec2i scroller_size_;
    psapi::vec2i catch_pos_ = {0, 0};

    std::unique_ptr<psapi::sfm::ITexture> background_;

    std::unique_ptr<psapi::sfm::ITexture> normal_;
    std::unique_ptr<psapi::sfm::ITexture> hover_;
    std::unique_ptr<psapi::sfm::ITexture> active_;

    State state_ = State::Normal;

    void loadStateTexture(psapi::sfm::ISprite* sprite) const;
    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);

    virtual void updateScroller() = 0;
    virtual void setScrollerPos(psapi::sfm::vec2i pos) = 0;

    friend class ScrollAction;
};

class HorizontalScrollBar : public AScrollBar
{
public:
    HorizontalScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                        std::unique_ptr<psapi::sfm::ITexture> background,
                        std::unique_ptr<psapi::sfm::ITexture> normal,
                        std::unique_ptr<psapi::sfm::ITexture> hover,
                        std::unique_ptr<psapi::sfm::ITexture> active,
                        Scrollable* object);

protected:
    virtual void updateScroller();
    virtual void setScrollerPos(psapi::sfm::vec2i pos) override;

};

class VerticalScrollBar : public AScrollBar
{
public:
    VerticalScrollBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
                        std::unique_ptr<psapi::sfm::ITexture> background,
                        std::unique_ptr<psapi::sfm::ITexture> normal,
                        std::unique_ptr<psapi::sfm::ITexture> hover,
                        std::unique_ptr<psapi::sfm::ITexture> active,
                        Scrollable* object);

protected:
    virtual void updateScroller();
    virtual void setScrollerPos(psapi::sfm::vec2i pos) override;

};

class ScrollAction : public AAction
{
public:
    ScrollAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, AScrollBar* scrollbar);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    AScrollBar* scrollbar_;
};


#endif // SCROLLBAR_HPP
