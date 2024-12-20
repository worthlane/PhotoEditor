#include "brightness.hpp"

#include "pluginLib/windows/windows.hpp"
#include "pluginLib/bars/ps_bar.hpp"
#include "pluginLib/actions/actions.hpp"
#include "pluginLib/filters/filters.hpp"
#include "pluginLib/filters/filterWindows.hpp"
#include "pluginLib/timer/timer.hpp"
#include "pluginLib/canvas/canvas.hpp"
#include "pluginLib/filters/slider.hpp"
#include "catmullRom.hpp"

#include <cassert>
#include <algorithm>
#include <string>

using namespace ps;
using namespace psapi;
using namespace psapi::sfm;
using namespace cr;

namespace
{

static const wid_t kGraphId = 88819302;

class InteractivePoint : public ABarButton
{
public:
    InteractivePoint(vec2i pos, vec2u size,
                     vec2i boundaryTopLeft, vec2u boundarySizes);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    bool update(const IRenderWindow* renderWindow, const Event& event);

    void draw(IRenderWindow* renderWindow) override;

    void setPos(const vec2i& pos) override;
    void setSize(const vec2u& size) override;

    void setSprite(SpriteInfo spriteInfo, State state);
private:
    static const size_t nStates = 4;
    SpriteInfo sprites_[nStates];

    vec2i boundaryTopLeft_;
    vec2u boundarySizes_;
};

static const vec2i deltaFromGraphSpritePos = {63, 4};
static const vec2u deltaFromGraphSpriteSize = {80, 56};
static const int kInteractivePointRadius = 7;

class Graph : public AWindow
{
public:
    Graph(vec2i pos, SpriteInfo graphSprite, vec2f graphSteps_);

    void draw(IRenderWindow* renderWindow) override;

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;

    void addPoint(vec2i point);
    void removePoint(vec2i point);
    void movePoint(vec2i point, vec2i newPos);

    vec2f getGraphSteps() const;

    void setPos(const vec2i& pos) override;
    void setSize(const vec2u& size) override;

    const CatmullRom& getInterpolator() const & { return catmullRom_; }
    vec2f recalculateInterpolatorPointToData(vec2f point) const;

private:
    CatmullRom catmullRom_;
    SpriteInfo graphSprite_;
    vec2f graphSteps_;
};

class SceneController : public IWindow
{
public:
    SceneController(Graph&& graph);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow, const Event& event) override;
    bool update(const IRenderWindow* renderWindow, const Event& event);

    void draw(IRenderWindow* renderWindow) override;

    wid_t getId() const override;

    IWindow* getWindowById(wid_t id) override;
    const IWindow* getWindowById(wid_t id) const override;

    vec2i getPos() const override;
    vec2u getSize() const override;

    void setPos(const vec2i& pos) override;
    void setSize(const vec2u& size) override;

    void forceActivate() override;
    void forceDeactivate() override;

    bool isActive() const override;
    bool isWindowContainer() const override;

    void setParent(const IWindow* parent) override;

private:
    wid_t id_ = kInvalidWindowId;

    bool isActive_ = true;

    std::vector<InteractivePoint> points_;
    Graph graph_;
};

// Graph implementation

void drawPoint(IRenderWindow* renderWindow, vec2i point, int radius = 4)
{
    auto circle = IEllipseShape::create(radius, radius);
    circle->setFillColor(Color::getStandardColor(Color::Type::White));
    circle->setPosition(point - vec2i{radius, radius});

    renderWindow->draw(circle.get());
}

Graph::Graph(vec2i pos, SpriteInfo graphSprite, vec2f graphSteps)
    : AWindow(pos, graphSprite.sprite->getSize(), kInvalidWindowId),
      graphSprite_(std::move(graphSprite)), graphSteps_(graphSteps),
      catmullRom_(std::vector<vec2f>())
{
    id_ = kGraphId;

    vec2f infPoint = vec2f{0.f, pos_.y};
    catmullRom_.addPoint(infPoint);

    graphSprite_.sprite->setPosition(pos_.x, pos_.y);
}

void Graph::draw(IRenderWindow* renderWindow)
{
    graphSprite_.sprite->draw(renderWindow);

    static const float infX = 900.f;
    vec2f infPoint = vec2f{infX, 0.f};
    catmullRom_.addPoint(infPoint);

    const double step = 0.001;
    //fprintf(stderr, "GRAPH SIZE: %u\n", catmullRom_.getSize());
    for (double a = 1; a < (double)catmullRom_.getSize() - 2.0 - step; a += step)
    {
        drawPoint(renderWindow, vec2i{static_cast<int>(catmullRom_[a].x),
                                      static_cast<int>(catmullRom_[a].y)});
    }

    catmullRom_.removePoint(infPoint);
}


std::unique_ptr<IAction> Graph::createAction(const IRenderWindow* renderWindow, const Event& event)
{
    return nullptr;
}

void Graph::addPoint(vec2i point) { catmullRom_.addPoint(vec2f{(float)point.x, (float)point.y}); }
void Graph::removePoint(vec2i point) { catmullRom_.removePoint(vec2f{(float)point.x, (float)point.y}); };
void Graph::movePoint(vec2i point, vec2i newPos) { catmullRom_.setPoint(vec2f{(float)point.x, (float)point.y},
                                                                        vec2f{(float)newPos.x, (float)newPos.y}); };

vec2f Graph::getGraphSteps() const { return graphSteps_; }

void Graph::setPos(const vec2i& pos)
{
    AWindow::setPos(pos);

    graphSprite_.sprite->setPosition(pos.x, pos.y);
}

void Graph::setSize(const vec2u& size)
{
    AWindow::setSize(size);

    assert(0);
}

vec2f Graph::recalculateInterpolatorPointToData(vec2f point) const
{
    return vec2f{((point.x - (float)pos_.x - (float)deltaFromGraphSpritePos.x - kInteractivePointRadius) * graphSteps_.x),
                 ((float)pos_.y + (float)size_.y - point.y - (float)deltaFromGraphSpritePos.y) * graphSteps_.y};
}
// Interactive point implementation

InteractivePoint::InteractivePoint(vec2i pos, vec2u size,
                                   vec2i boundaryTopLeft, vec2u boundarySizes)
    : boundaryTopLeft_(boundaryTopLeft), boundarySizes_(boundarySizes)
{
    pos_ = pos;
    size_ = size;
}

void InteractivePoint::setSprite(SpriteInfo spriteInfo, State state)
{
    sprites_[static_cast<size_t>(state)] = std::move(spriteInfo);
    sprites_[static_cast<size_t>(state)].sprite->setPosition(pos_.x, pos_.y);
}

std::unique_ptr<IAction> InteractivePoint::createAction(const IRenderWindow* renderWindow,
                                                        const Event& event)
{
    return std::make_unique<UpdateCallbackAction<InteractivePoint>>(*this, renderWindow, event);
}

bool InteractivePoint::update(const IRenderWindow* renderWindow, const Event& event)
{
    if (event.type == Event::MouseButtonPressed &&
        checkIsHovered(Mouse::getPosition(renderWindow), pos_, size_))
        state_ = State::Press;

    bool updateStateResult = updateState(renderWindow, event);

    if (state_ != State::Press)
        return updateStateResult;

    vec2i mousePos = Mouse::getPosition(renderWindow);

    mousePos.x = std::clamp(mousePos.x, boundaryTopLeft_.x, boundaryTopLeft_.x + (int)boundarySizes_.x);
    mousePos.y = std::clamp(mousePos.y, boundaryTopLeft_.y, boundaryTopLeft_.y + (int)boundarySizes_.y);

    setPos(mousePos);

    return true;
}

void InteractivePoint::draw(IRenderWindow* renderWindow)
{
    renderWindow->draw(sprites_[static_cast<size_t>(state_)].sprite.get());
}

void InteractivePoint::setPos(const vec2i& pos)
{
    ABarButton::setPos(pos);

    for (auto& sprite : sprites_)
        sprite.sprite->setPosition(pos.x - size_.x / 2, pos.y - size_.y / 2);
}

void InteractivePoint::setSize(const vec2u& size)
{
    ABarButton::setSize(size);

    fprintf(stderr, "CAN'T SET SIZE TO POINT\n");
}

// Scene controller implementation

vec2i calculateMiddle(vec2i pos, vec2u size)
{
    return {pos.x + size.x / 2, pos.y + size.y / 2};
}

SceneController::SceneController(Graph&& graph) : graph_(std::move(graph))
{
    vec2u size = {13, 13};

    vec2i pos = graph_.getPos();
    vec2i positions[] = {pos + vec2i{63, 4}, pos + vec2i{150, 100}, pos + vec2i{300, 400}, pos + vec2i{600, 200}};

    for (auto& point : positions)
    {
        // TODO: magic shifts that depend on texture
        InteractivePoint interactivePoint{point, size, graph_.getPos() + deltaFromGraphSpritePos,
                                                       graph_.getSize() - deltaFromGraphSpriteSize};

        interactivePoint.setSprite(createSprite("assets/textures/circleNormal.png"),  IBarButton::State::Normal);
        interactivePoint.setSprite(createSprite("assets/textures/circleHover.png"),   IBarButton::State::Hover);
        interactivePoint.setSprite(createSprite("assets/textures/circlePress.png"),   IBarButton::State::Press);
        interactivePoint.setSprite(createSprite("assets/textures/circleRelease.png"), IBarButton::State::Released);

        graph_.addPoint(calculateMiddle(point, size));
        points_.push_back(std::move(interactivePoint));
    }

    //fprintf(stderr, "GRAPH SIZE - %zu\n", graph_.catmullRom_.getSize());
}


std::unique_ptr<IAction> SceneController::createAction(const IRenderWindow* renderWindow,
                                                       const Event& event)
{
    return std::make_unique<UpdateCallbackAction<SceneController>>(*this, renderWindow, event);
}

void unPressAllPoints(std::vector<InteractivePoint>& points)
{
    for (auto& point : points)
        point.setState(IBarButton::State::Normal);
}

bool SceneController::update(const IRenderWindow* renderWindow, const Event& event)
{
    AActionController* controller = getActionController();
    bool executedSomeone = false;

    InteractivePoint* pressedPoint = nullptr;
    for (auto& point : points_)
    {
        vec2i middle = calculateMiddle(point.getPos(), point.getSize());
        bool movedPoint = controller->execute(point.createAction(renderWindow, event));

        if (movedPoint)
            graph_.movePoint(middle, calculateMiddle(point.getPos(), point.getSize()));

        if (point.getState() == IBarButton::State::Press)
            pressedPoint = &point;

        executedSomeone |= movedPoint;
    }

    if (pressedPoint)
    {
        unPressAllPoints(points_);
        pressedPoint->setState(IBarButton::State::Press);
    }

    controller->execute(graph_.createAction(renderWindow, event));

    return executedSomeone;
}

void SceneController::draw(IRenderWindow* renderWindow)
{
    graph_.draw(renderWindow);

    for (auto& point : points_)
        point.draw(renderWindow);
}

wid_t SceneController::getId() const
{
    return id_;
}

IWindow* SceneController::getWindowById(wid_t id)
{
    if (id == id_)
        return this;

    IWindow* searchResult = graph_.getWindowById(id);
    if (searchResult)
        return searchResult;

    for (auto& point : points_)
    {
        searchResult = point.getWindowById(id);
        if (searchResult)
            return searchResult;
    }

    return nullptr;
}

const IWindow* SceneController::getWindowById(wid_t id) const
{
    return const_cast<SceneController*>(this)->getWindowById(id);
}

vec2i SceneController::getPos() const
{
    assert(0);
}

vec2u SceneController::getSize() const
{
    assert(0);
}

void SceneController::setPos(const vec2i& /* pos */) { assert(0); }
void SceneController::setSize(const vec2u& /* size */) { assert(0); }

void SceneController::forceActivate()
{
    isActive_ = true;
}

void SceneController::forceDeactivate()
{
    isActive_ = false;
}

bool SceneController::isActive() const
{
    return isActive_;
}

bool SceneController::isWindowContainer() const
{
    return false;
}

void SceneController::setParent(const IWindow* /* parent */)
{
    assert(0);
}

// Brightness button

class BrightnessFilter : public ANamedBarButton
{
public:
    BrightnessFilter(std::unique_ptr<IText> name, std::unique_ptr<IFont> font);

    std::unique_ptr<IAction> createAction(const IRenderWindow* renderWindow,
                                          const Event& event) override;

    bool update(const IRenderWindow* renderWindow, const Event& event);
    void draw(IRenderWindow* renderWindow) override;

private:
    std::vector<std::vector<Color>> beginLayer_;

    std::unique_ptr<FilterWindow> filterWindow_;
};

std::unique_ptr<FilterWindow> createFilterWindow(const char* name)
{
    auto filterWindow = std::make_unique<FilterWindow>(kInvalidWindowId, name);

    vec2i graphTopLeft = {0, 0};

    ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
    SpriteInfo grid = createSprite("assets/textures/grid_plot.png");
    vec2f graphSteps = vec2f{(float)canvas->getSize().x /
                             ((float)grid.sprite->getSize().x - deltaFromGraphSpriteSize.x), 0.004f};

    Graph graph{graphTopLeft, std::move(grid), graphSteps};

    auto emptyWindow = std::make_unique<EmptyWindow>(createSprite("assets/textures/renderWindowColor.png"));
    emptyWindow->setSize(filterWindow->getRenderWindowSize());

    auto sceneController = std::make_unique<SceneController>(std::move(graph));

    filterWindow->addWindow(std::move(emptyWindow)); // order is important
    filterWindow->addWindow(std::move(sceneController));

    addApplyButtons(filterWindow.get());

    return filterWindow;
}

BrightnessFilter::BrightnessFilter(std::unique_ptr<IText> name, std::unique_ptr<IFont> font)
{
    name_ = std::move(name);
    font_ = std::move(font);
}

std::unique_ptr<IAction> BrightnessFilter::createAction(const IRenderWindow* renderWindow,
                                                        const Event& event)
{
    return std::make_unique<UpdateCallbackAction<BrightnessFilter>>(*this, renderWindow, event);
}

void setBrightness(std::vector<std::vector<Color>>& pixels, unsigned fromX, unsigned toX, float brightness)
{
    for (unsigned y = 0; y < pixels.size(); ++y)
    {
        for (unsigned x = fromX; x < toX; ++x)
        {
            if (x >= pixels[y].size())
                break;

            pixels[y][x].r = std::clamp(static_cast<int>(pixels[y][x].r * brightness), 0, 255);
            pixels[y][x].g = std::clamp(static_cast<int>(pixels[y][x].g * brightness), 0, 255);
            pixels[y][x].b = std::clamp(static_cast<int>(pixels[y][x].b * brightness), 0, 255);
        }
    }
}

std::vector<std::vector<Color>> applyBrightness(const std::vector<std::vector<Color>>& pixels,
                                                const Graph* graph)
{
    const CatmullRom& interpolator = graph->getInterpolator();

    std::vector<std::vector<Color>> result = pixels;

    double step = 0.01;
    vec2f prevPoint = interpolator[1.0];
    for (double a = 1; a < (double)interpolator.getSize() - 2.0 - step; a += step)
    {
        vec2f point = interpolator[a];

        unsigned fromX = static_cast<unsigned>(graph->recalculateInterpolatorPointToData(prevPoint).x);
        unsigned toX = static_cast<unsigned>(graph->recalculateInterpolatorPointToData(point).x);
        #if 0
        fprintf(stderr, "POINT - %u %u\n", static_cast<unsigned>(point.x), static_cast<unsigned>(point.y));
        fprintf(stderr, "FROM %u TO %u\n", fromX, toX);
        #endif

        setBrightness(result, fromX, toX, graph->recalculateInterpolatorPointToData(point).y);
        prevPoint = point;
    }

    return result;
}

bool BrightnessFilter::update(const IRenderWindow* renderWindow, const Event& event)
{
    bool updateStateRes = updateState(renderWindow, event);

    if (state_ != State::Released)
    {
        if (filterWindow_)
        {
            filterWindow_->close();
            filterWindow_.reset();
        }

        return updateStateRes;
    }

    ICanvas* canvas = static_cast<ICanvas*>(getRootWindow()->getWindowById(kCanvasWindowId));
    assert(canvas);

    size_t activeLayerIndex = canvas->getActiveLayerIndex();
    ILayer* activeLayer = canvas->getLayer(activeLayerIndex);

    vec2u canvasSize = canvas->getSize();

    if (updateStateRes)
    {
        beginLayer_ = getLayerScreenIn2D(activeLayer, canvasSize);
        filterWindow_ = createFilterWindow("Brightness Filter");
    }

    assert(filterWindow_);

    AActionController* actionController = getActionController();

    if (!actionController->execute(filterWindow_->createAction(renderWindow, event)))
    {
        state_ = State::Normal;
        return false;
    }

    Graph* graph = dynamic_cast<Graph*>(filterWindow_->getWindowById(kGraphId));

    if (!graph)
        return false;

    copyPixelsToLayer(activeLayer, applyBrightness(beginLayer_, graph));

    return true;
}

void BrightnessFilter::draw(IRenderWindow* renderWindow)
{
    ANamedBarButton::draw(renderWindow);

    if (filterWindow_)
        filterWindow_->draw(renderWindow);
}


} // namespace anonymous

bool onLoadPlugin()
{
    std::unique_ptr<IText> text = IText::create();
    std::unique_ptr<IFont> font = IFont::create();
    font->loadFromFile("assets/fonts/arial.ttf");
    text->setFont(font.get());
    text->setString("Brightness Filter");

    auto button = std::make_unique<BrightnessFilter>(std::move(text), std::move(font));

    IWindowContainer* rootWindow = getRootWindow();
    assert(rootWindow);
    auto filterMenu = dynamic_cast<IMenuButton*>(rootWindow->getWindowById(kMenuFilterId));
    assert(filterMenu);

    filterMenu->addMenuItem(std::move(button));

    return true;
}

void onUnloadPlugin()
{

}
