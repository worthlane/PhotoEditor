#include <cassert>
#include <iostream>

#include "../plugins/filters/filtersbar.hpp"
#include "../plugins/filters/filters.hpp"

#include "style/design.hpp"

#include "implementation/bar/bar_base.hpp"

static psapi::sfm::ITexture* btn = nullptr;

static psapi::sfm::ITexture* back    = nullptr;
static psapi::sfm::ITexture* release = nullptr;
static psapi::sfm::ITexture* hover   = nullptr;
static psapi::sfm::ITexture* press   = nullptr;
static psapi::sfm::ITexture* normal  = nullptr;

static const psapi::sfm::IntRect BACKGROUND_RECT = {{0, 30}, {90, 300}};
static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {88, 28}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";
static const char* HOVER_TEXTURE      = "assets/textures/white.jpg";
static const char* RELEASE_TEXTURE    = "assets/textures/white.jpg";
static const char* PRESS_TEXTURE      = "assets/textures/white.jpg";
static const char* NORMAL_TEXTURE     = "assets/textures/white.jpg";

bool onLoadPlugin()
{
    std::cout << "filtersbar loaded\n";

    back = psapi::sfm::ITexture::create().release();
    back->loadFromFile(BACKGROUND_TEXTURE);

    hover = psapi::sfm::ITexture::create().release();
    hover->loadFromFile(HOVER_TEXTURE);

    press = psapi::sfm::ITexture::create().release();
    press->loadFromFile(PRESS_TEXTURE);

    normal = psapi::sfm::ITexture::create().release();
    normal->loadFromFile(NORMAL_TEXTURE);

    release = psapi::sfm::ITexture::create().release();
    release->loadFromFile(RELEASE_TEXTURE);

    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, BACKGROUND_RECT, 1, {0, 0});
    back_sprite->setColor(psapi::sfm::Color(210, 231, 252));

    std::unique_ptr<psapi::sfm::ISprite> release_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(release_sprite.get(), release, BUTTON_RECT, 1, {6, 0});
    release_sprite->setColor(psapi::sfm::Color(121, 232, 150));

    std::unique_ptr<psapi::sfm::ISprite> hover_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(hover_sprite.get(), hover, BUTTON_RECT, 1, {6, 0});
    hover_sprite->setColor(psapi::sfm::Color(150, 150, 150));

    std::unique_ptr<psapi::sfm::ISprite> press_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(press_sprite.get(), press, BUTTON_RECT, 1, {6, 0});
    press_sprite->setColor(psapi::sfm::Color(100, 100, 100));

    std::unique_ptr<psapi::sfm::ISprite> normal_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(normal_sprite.get(), normal, BUTTON_RECT, 1, {6, 0});
    normal_sprite->setColor(psapi::sfm::Color(168, 208, 247));

    auto filtersbar = std::make_unique<FiltersBar>(kFiltersBarWindowId,
                                         psapi::vec2i(0, 30),
                                         psapi::vec2u(BACKGROUND_RECT.size.x, BACKGROUND_RECT.size.y),
                                         std::move(back_sprite),
                                         std::move(normal_sprite),
                                         std::move(hover_sprite),
                                         std::move(press_sprite),
                                         std::move(release_sprite));


    auto root = psapi::getRootWindow();
    auto filters = root->getWindowById(kFiltersBarWindowId);

    btn = psapi::sfm::ITexture::create().release();

    std::unique_ptr<psapi::sfm::ISprite> filters_sprite = psapi::sfm::ISprite::create();
    filters_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> neg_sprite = psapi::sfm::ISprite::create();
    neg_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> bar_sprite = psapi::sfm::ISprite::create();
    bar_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> contrast_sprite = psapi::sfm::ISprite::create();
    contrast_sprite.get()->setTextureRect(BUTTON_RECT);

    std::unique_ptr<psapi::sfm::ISprite> blur_sprite = psapi::sfm::ISprite::create();
    blur_sprite.get()->setTextureRect(BUTTON_RECT);

    auto canvas = static_cast<psapi::ICanvas*>(root->getWindowById(psapi::kCanvasWindowId));

    auto menu = static_cast<psapi::IBar*>(root->getWindowById(psapi::kMenuBarWindowId));
    auto menu_pos = menu->getPos();

    auto negative = std::make_unique<NegativeButton>(kNegativeFilterButtonId, filtersbar.get(),
                                                    psapi::vec2i(1, 1),
                                                    psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                                    std::move(neg_sprite),
                                                    -1, canvas);

    auto barel = std::make_unique<BareliefButton>(kBareliefFilterButtonId, filtersbar.get(),
                                                  psapi::vec2i(1, 2 + BUTTON_RECT.size.y),
                                                  psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                                  std::move(bar_sprite),
                                                  -1, canvas);

    auto blur = std::make_unique<BlurButton>(kBlurFilterButtonId, filtersbar.get(),
                                             psapi::vec2i(1, 3 + 2 * BUTTON_RECT.size.y),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(blur_sprite),
                                               canvas);

    auto contrast = std::make_unique<ContrastButton>(kContrastFilterButtonId, filtersbar.get(),
                                               psapi::vec2i(1, 4 + 3 * BUTTON_RECT.size.y),
                                               psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                               std::move(contrast_sprite),
                                               canvas);

    auto filterbutton = std::make_unique<FiltersButton>(psapi::kMenuFilterId, menu,
                                                        psapi::vec2i(1, 1),
                                                        psapi::vec2u(BUTTON_RECT.size.x, BUTTON_RECT.size.y),
                                                        std::move(filters_sprite), std::move(filtersbar));



    filterbutton->addMenuItem(std::move(negative));
    filterbutton->addMenuItem(std::move(barel));
    filterbutton->addMenuItem(std::move(blur));
    filterbutton->addMenuItem(std::move(contrast));
    menu->addWindow(std::move(filterbutton));

        /*bar->addWindow(std::move(negative));
        bar->addWindow(std::move(barel));
        bar->addWindow(std::move(blur));
        bar->addWindow(std::move(contrast));*/

    //root->addWindow(std::move(bar));
}

void onUnloadPlugin()
{
    delete back;
    delete release;
    delete hover;
    delete press;
    delete normal;

    delete btn;
}


// ********************* MENUBAR *********************/

FiltersBar::FiltersBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background,
            std::unique_ptr<psapi::sfm::ISprite> normal,
            std::unique_ptr<psapi::sfm::ISprite> hovered,
            std::unique_ptr<psapi::sfm::ISprite> pressed,
            std::unique_ptr<psapi::sfm::ISprite> released) :
    ABar(id, pos, size, std::move(background),
                    std::move(normal),
                    std::move(hovered),
                    std::move(pressed),
                    std::move(released))
{
}


void FiltersBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    if (!isActive())
        return;

    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

    for (auto& button : buttons_)
    {
        finishButtonDraw(renderWindow, button.get());
        button.get()->draw(renderWindow);
    }

}
std::unique_ptr<psapi::IAction> FiltersBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<FiltersBarAction>(renderWindow, event, this);
}

// ******************** TOOLBAR ACTION ***********************

FiltersBarAction::FiltersBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FiltersBar* filters_bar) :
AAction(render_window, event), filters_bar_(filters_bar)
{
}

bool FiltersBarAction::execute(const Key& key)
{
    if (!filters_bar_->is_active_)
        return false;

    bool flag = false;

    auto controller = psapi::getActionController();

    for (size_t i = 0; i < filters_bar_->buttons_.size(); ++i)
    {
        auto& button = filters_bar_->buttons_[i];

        psapi::IBarButton::State prev_state = button.get()->getState();

        flag |= controller->execute(button.get()->createAction(render_window_, event_));

        psapi::IBarButton::State cur_state = button.get()->getState();

        if (cur_state == psapi::IBarButton::State::Released &&
            prev_state != psapi::IBarButton::State::Released)
        {
            for (size_t j = 0; j < filters_bar_->buttons_.size(); ++j)
                if (j != i) filters_bar_->buttons_[j].get()->setState(psapi::IBarButton::State::Normal);
        }
    }

    return flag;
}

bool FiltersBarAction::isUndoable(const Key& key)
{
    return false;
}

// ******** FILTERS BUTTON **********

FiltersButton::FiltersButton(const psapi::wid_t id, psapi::IBar* bar, const psapi::vec2i& pos, const psapi::vec2u& size,
                 std::unique_ptr<psapi::sfm::ISprite> sprite, std::unique_ptr<psapi::IBar> menu) :
                 MenuSwitchButton(id, bar, pos, size, std::move(sprite), std::move(menu))
{}

std::unique_ptr<psapi::IAction> FiltersButton::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    if (!isActive())
        return std::make_unique<IdleAction>(renderWindow, event);

    updateState(renderWindow, event);

    if (state_ != SwitchButton::State::Released)
    {
        if (menu_->isActive())
            deactivateMenu();

        return std::make_unique<IdleAction>(renderWindow, event);
    }

    if (!menu_->isActive())
        activateMenu();

    return std::make_unique<FiltersAction>(renderWindow, event, this);
}

FiltersAction::FiltersAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, FiltersButton* filters_button) :
                                AAction(render_window, event), filters_button_(filters_button)
{}

bool FiltersAction::execute(const Key& key)
{
    auto controller = psapi::getActionController();
    controller->execute(filters_button_->menu_->createAction(render_window_, event_));

    return true;
}

bool FiltersAction::isUndoable(const Key& key)
{
    return false;
}


