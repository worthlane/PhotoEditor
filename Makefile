CXX = g++

EXECUTABLE = app

CXXFLAGS  = -D _DEBUG -ggdb3 -std=c++17 -O3 -Wall -Wextra -Weffc++								\
			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts       \
			-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal  \
			-Wformat-nonliteral -Wformat-security -Wformat=2 -Winline          \
			-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked            \
			-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo  \
			-Wstrict-overflow=2                \
			-Wsuggest-override -Wswitch-default      \
			-Wswitch-enum -Wundef -Wunreachable-code -Wunused           \
			-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing   \
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation    \
			-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192         \
			-fPIE -Werror=vla


HOME = $(shell pwd)

INCLUDE_DIR = include

LIB_INCLUDE = -I/opt/homebrew/Cellar/sfml/2.6.1/include -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

LOCATION_FLAG  = -I $(HOME)/$(INCLUDE_DIR)
CXXFLAGS  += $(LIB_INCLUDE)

BUILD_DIR   = build

DOXYFILE = Doxyfile
DOXYBUILD = doxygen $(DOXYFILE)

# ==============================================================
# 						 FILE GROUPS
# ==============================================================

SOURCES = main.cpp
SOURCE_DIR = src

IMPLEMENTATION_SOURCES = actions.cpp bar.cpp canvas.cpp memento.cpp photoshop.cpp sfm.cpp
IMPLEMENTATION_DIR = $(SOURCE_DIR)/implementation

API_SOURCES = api_system.cpp
API_DIR = $(SOURCE_DIR)/api

PLUGINS_SOURCES = canvas_plugin.cpp
PLUGINS_DIR = $(SOURCE_DIR)/plugins

OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
IMPLEMENTATION_OBJECTS = $(IMPLEMENTATION_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
PLUGINS_OBJECTS = $(PLUGINS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
API_OBJECTS = $(API_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# ==============================================================
#						 API
# ==============================================================

API_TARGET_DLL = $(BUILD_DIR)/libapi_photoshop.dll

DLL_API_SOURCES = src/api/api_system.cpp \
				  src/implementation/sfm.cpp src/implementation/photoshop.cpp

# ==============================================================

.PHONY: all
all: $(API_TARGET_DLL) $(EXECUTABLE)

# -------------------------------------------------------------------------------

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(API_TARGET_DLL)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

#$(BUILD_DIR)/%.o : $(GRAPHICS_DIR)/%.cpp
#	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

#$(BUILD_DIR)/%.o : $(STANDARD_DIR)/%.cpp
#	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

#$(BUILD_DIR)/%.o : $(API_DIR)/%.cpp
#	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(API_TARGET_DLL): $(DLL_API_SOURCES)
	$(CXX) -dynamiclib $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

# -------------------------------------------------------------------------------

.PHONY: clean makedirs plugins

clean:
	rm -rf  $(EXECUTABLE) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.dll $(BUILD_DIR)/*.so $(BUILD_DIR)/*.dSYM

makedirs:
	mkdir -p $(BUILD_DIR)

plugins:
	$(CXX) -dynamiclib plugins/canvas/canvas.cpp plugins/canvas/scrollbar.cpp -o $(BUILD_DIR)/canvas.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
#	$(CXX) -dynamiclib plugins/toolbar/tool_bar.cpp plugins/bar_base/bar.cpp plugins/style/design.cpp -o $(BUILD_DIR)/toolbar.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
#	$(CXX) -dynamiclib plugins/brush/brush.cpp plugins/brush/catmull.cpp plugins/bar_base/bar_button.cpp -o $(BUILD_DIR)/brush.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
#	$(CXX) -dynamiclib plugins/filters/filters.cpp plugins/brush/catmull.cpp plugins/bar_base/bar_button.cpp -o $(BUILD_DIR)/filters.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
#	$(CXX) -dynamiclib plugins/geometry/geometry.cpp plugins/bar_base/bar_button.cpp -o $(BUILD_DIR)/geometry.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
#	$(CXX) -dynamiclib plugins/filters/filters_bar.cpp plugins/bar_base/bar_button.cpp plugins/bar_base/bar.cpp plugins/style/design.cpp -o $(BUILD_DIR)/filters_bar.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
