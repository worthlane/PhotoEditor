#ifndef FILEBAR_PLUGIN_HPP
#define FILEBAR_PLUGIN_HPP

#include <filesystem>
#include <iostream>

#include "api/api_canvas.hpp"

#include "implementation/actions.hpp"
#include "implementation/bar/bar_base.hpp"
#include "implementation/bar/bar_button.hpp"

#include "../plugins/menubar/submenubar.hpp"

static const psapi::wid_t kFileButtonId = 1000;
static const std::string SEARCH_DIR = "assets";
static const std::vector<std::string> ALLOWED_FILE_TYPES = {".png", ".jpg", ".jpeg"};

extern "C"
{

bool onLoadPlugin();
void onUnloadPlugin();

}

template <typename T>
void createFileButtons(CallSubMenuButton* button, psapi::ICanvas* canvas)
{
    std::string dir = "assets";

    auto bar = button->getMenu();

    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir))
    {
        size_t counter = 0;

        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                std::string name = entry.path().string();

                bool flag = false;
                for (auto& type : ALLOWED_FILE_TYPES)
                    flag |= (name.find(type) != std::string::npos);

                if (!flag)
                    continue;

                std::unique_ptr<T> file_button = std::make_unique<T>(kFileButtonId, bar,
                                                        psapi::vec2i(GAP.x / 2, (counter + 1) * GAP.y + counter * SUBBUTTON_RECT.size.y),
                                                        SUBBUTTON_RECT.size,
                                                        name, psapi::sfm::Color(255, 255, 255), canvas);

                expandMenu(bar, file_button.get());
                button->addMenuItem(std::move(file_button));

                counter++;
            }
        }
    }
    else
    {
        std::cerr << "Can't find \"" << dir << "\" directory\n";
    }
}

static const psapi::wid_t kFileBarWindowId = 267;

static const psapi::wid_t kFileImportBarWindowId = 777;
static const psapi::wid_t kFileExportBarWindowId = 778;

static const psapi::wid_t kFileImportMenuId = 780;
static const psapi::wid_t kFileExportMenuId = 781;

#endif // FILEBAR_PLUGIN_HPP
