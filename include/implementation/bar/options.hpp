#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "api/api_bar.hpp"
#include "api/api_sfm.hpp"

#include "implementation/actions.hpp"

class ColorPalette : public psapi::IColorPalette
{
public:
    ColorPalette();

    virtual psapi::sfm::Color getColor() const override;
    virtual void setColor(const psapi::sfm::Color &color) override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::wid_t getId() const override;
    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;
    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;
    virtual void setParent(const psapi::IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

protected:
    psapi::wid_t id_ = psapi::kColorPaletteId;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    psapi::vec2i pos_ = {0, 0};
    psapi::vec2u size_ = {0, 0};

    psapi::sfm::Color color_ = psapi::sfm::Color(255, 255, 255);

    std::unique_ptr<psapi::sfm::IRectangleShape> current_;

    std::vector<std::unique_ptr<psapi::sfm::IRectangleShape>> variants_;

    void placeColors(const psapi::sfm::vec2i pos, const psapi::sfm::vec2u size);
    bool hoverRectangle(const psapi::sfm::vec2i pos, const psapi::sfm::IRectangleShape* sprite);

    friend class ColorPaletteAction;
};

class ColorPaletteAction : public AAction
{
public:
    ColorPaletteAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ColorPalette* palette);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    ColorPalette* palette_;
};

class OpacityOption : public psapi::IOpacityOption
{
public:
    enum class State
    {
        Normal,
        Hover,
        Active,
    };

    OpacityOption();

    virtual float getOpacity() const override;
    virtual void setOpacity(float opacity) override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::wid_t getId() const override;
    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;
    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;
    virtual void setParent(const psapi::IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    static std::unique_ptr<IOpacityOption> create();

protected:
    psapi::wid_t id_ = psapi::kOpacityBarId;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    float opacity_ = 1;

    psapi::vec2i pos_ = {0, 0};
    psapi::vec2u size_ = {0, 0};

    psapi::vec2i rail_pos_ = {0, 0};
    psapi::vec2i slider_pos_ = {0, 0};

    psapi::vec2i catch_pos_ = {0, 0};

    std::unique_ptr<psapi::sfm::ITexture> background_;

    std::unique_ptr<psapi::sfm::ITexture> normal_rail_;
    std::unique_ptr<psapi::sfm::ITexture> active_rail_;

    std::unique_ptr<psapi::sfm::ITexture> normal_;
    std::unique_ptr<psapi::sfm::ITexture> hover_;
    std::unique_ptr<psapi::sfm::ITexture> active_;

    State state_ = State::Normal;

    void loadStateTexture(psapi::sfm::ISprite* sprite) const;
    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);

    void updateSlider();
    void setSliderPos(psapi::sfm::vec2i pos);

    friend class OpacityOptionAction;
};

class OpacityOptionAction : public AAction
{
public:
    OpacityOptionAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, OpacityOption* option);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    OpacityOption* option_;
};

class ThicknessOption : public psapi::IThicknessOption
{
public:
    enum class State
    {
        Normal,
        Hover,
        Active,
    };

    ThicknessOption();

    virtual float getThickness() const override;
    virtual void setThickness(float thickness) override;

    virtual void draw(psapi::IRenderWindow* renderWindow) override;
    virtual std::unique_ptr<psapi::IAction> createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event) override;

    virtual psapi::wid_t getId() const override;
    virtual psapi::IWindow* getWindowById(psapi::wid_t id) override;
    virtual const psapi::IWindow* getWindowById(psapi::wid_t id) const override;

    virtual psapi::vec2i getPos() const override;
    virtual psapi::vec2u getSize() const override;
    virtual void setSize(const psapi::vec2u& size) override;
    virtual void setPos(const psapi::vec2i& pos) override;
    virtual void setParent(const psapi::IWindow* parent) override;
    virtual void forceActivate() override;
    virtual void forceDeactivate() override;
    virtual bool isActive() const override;
    virtual bool isWindowContainer() const override;

    static std::unique_ptr<IThicknessOption> create();

protected:
    psapi::wid_t id_ = psapi::kThicknessBarId;
    const psapi::IWindow* parent_ = nullptr;
    bool is_active_ = true;

    float thickness_ = 0.2;

    psapi::vec2i pos_ = {0, 0};
    psapi::vec2u size_ = {0, 0};

    psapi::vec2i rail_pos_ = {0, 0};
    psapi::vec2i slider_pos_ = {0, 0};

    psapi::vec2i catch_pos_ = {0, 0};

    std::unique_ptr<psapi::sfm::ITexture> background_;

    std::unique_ptr<psapi::sfm::ITexture> normal_rail_;
    std::unique_ptr<psapi::sfm::ITexture> active_rail_;

    std::unique_ptr<psapi::sfm::ITexture> normal_;
    std::unique_ptr<psapi::sfm::ITexture> hover_;
    std::unique_ptr<psapi::sfm::ITexture> active_;

    State state_ = State::Normal;

    void loadStateTexture(psapi::sfm::ISprite* sprite) const;
    void updateState(const psapi::IRenderWindow* renderWindow, const psapi::Event& event);

    void updateSlider();
    void setSliderPos(psapi::sfm::vec2i pos);

    friend class ThicknessOptionAction;
};

class ThicknessOptionAction : public AAction
{
public:
    ThicknessOptionAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, ThicknessOption* option);

    virtual bool execute   (const Key& key) override;
    virtual bool isUndoable(const Key& key) override;
private:
    ThicknessOption* option_;
};


/*class IThicknessOption : public IWindow
{
public:
    virtual float getThickness() const = 0;
    virtual void setThickness(float thickness) = 0;

    static std::unique_ptr<IThicknessOption> create();
};*/

#endif // OPTIONS_HPP
