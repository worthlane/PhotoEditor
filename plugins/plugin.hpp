#ifndef PLUGIN_HPP
#define PLUGIN_HPP

extern "C"
{

bool   loadPlugin();
void unloadPlugin();

}

class Canvas : public psapi::SimpleCanvas
{
public:
    Canvas(psapi::sfm::vec2i pos, psapi::sfm::vec2i size, psapi::sfm::vec2f scale) : SimpleCanvas(pos, size, scale) {}

    virtual psapi::wid_t getId() const override { return kCanvasWindowId; }

};

#endif // PLUGIN_EXAMPLE_HPP
