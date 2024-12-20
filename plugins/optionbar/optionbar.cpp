#include <cassert>
#include <iostream>

#include "../plugins/optionbar/optionbar.hpp"

#include "implementation/bar/bar_button.hpp"
#include "style/design.hpp"

#include "api/api_bar.hpp"

static psapi::sfm::ITexture* back    = nullptr;

static const psapi::sfm::IntRect BUTTON_RECT = {{0, 0}, {60, 60}};

static const char* BACKGROUND_TEXTURE = "assets/textures/white.jpg";

bool onLoadPlugin()
{
    std::cout << "option loaded\n";

    back = psapi::sfm::ITexture::create().release();
    back->loadFromFile(BACKGROUND_TEXTURE);

    static const psapi::sfm::IntRect back_rect = psapi::getOptionsBarIntRect();

    std::unique_ptr<psapi::sfm::ISprite> back_sprite = psapi::sfm::ISprite::create();
    make_styled_sprite(back_sprite.get(), back, back_rect, 1, {0, 0});
    back_sprite->setColor(psapi::sfm::Color(41, 44, 49));

    auto bar = std::make_unique<OptionBar>(psapi::kOptionsBarWindowId,
                                           back_rect.pos,
                                           back_rect.size,
                                           std::move(back_sprite));

    std::unique_ptr<psapi::IColorPalette> color_palette = psapi::IColorPalette::create();
    color_palette->setPos(back_rect.pos + psapi::sfm::vec2i(15, 6));

    std::unique_ptr<psapi::IOpacityOption> opacity_opt = psapi::IOpacityOption::create();
    opacity_opt->setPos(back_rect.pos + psapi::sfm::vec2i(300, 2));

    auto root = psapi::getRootWindow();

    root->addWindow(std::move(bar));
    root->addWindow(std::move(color_palette));
    root->addWindow(std::move(opacity_opt));
}

void onUnloadPlugin()
{
    delete back;
}

// ********************* OPTIONBAR *********************/

OptionBar::OptionBar(const psapi::wid_t id, const psapi::vec2i& pos, const psapi::vec2u& size,
            std::unique_ptr<psapi::sfm::ISprite> background) :
    AOptionsBar(id, pos, size, std::move(background))
{
}


void OptionBar::draw(psapi::sfm::IRenderWindow* renderWindow)
{
    background_.get()->setPosition(pos_.x, pos_.y);
    background_.get()->draw(renderWindow);

    for (auto& option : options_)
    {
        option.get()->draw(renderWindow);
    }

}
std::unique_ptr<psapi::IAction> OptionBar::createAction(const psapi::IRenderWindow* renderWindow, const psapi::Event& event)
{
    return std::make_unique<OptionBarAction>(renderWindow, event, this);
}

// ******************** OPTION BAR ACTION ***********************

OptionBarAction::OptionBarAction(const psapi::IRenderWindow* render_window, const psapi::Event& event, OptionBar* option_bar) :
AAction(render_window, event), option_bar_(option_bar)
{
}

bool OptionBarAction::execute(const Key& key)
{
    if (!option_bar_->is_active_)
        return false;

    bool flag = false;

    auto controller = psapi::getActionController();

    for (size_t i = 0; i < option_bar_->options_.size(); ++i)
    {
        auto& option = option_bar_->options_[i];

        flag |= controller->execute(option.get()->createAction(render_window_, event_));
    }

    return flag;
}

bool OptionBarAction::isUndoable(const Key& key)
{
    return false;
}

