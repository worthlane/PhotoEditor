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
MY_BUILD = build/mine

DOXYFILE = Doxyfile
DOXYBUILD = doxygen $(DOXYFILE)

# ==============================================================
# 						 FILE GROUPS
# ==============================================================

SOURCES = main.cpp
SOURCE_DIR = src

OBJECTS = $(SOURCES:%.cpp=$(MY_BUILD)/%.o)

# ==============================================================
#						 API
# ==============================================================

API_TARGET_DLL = $(BUILD_DIR)/libapi_photoshop.dylib

IMPLEMENTATION_SOURCES = sfm.cpp photoshop.cpp actions.cpp utils.cpp memento.cpp
IMPLEMENTATION_DIR = $(SOURCE_DIR)/implementation

API_SOURCES = api_system.cpp
API_DIR = $(SOURCE_DIR)/api

STYLE_SOURCES = design.cpp
STYLE_DIR = $(SOURCE_DIR)/style

CANVAS_SOURCES = canvas.cpp scrollbar.cpp
CANVAS_DIR     = $(IMPLEMENTATION_DIR)/canvas

BAR_SOURCES = bar_button.cpp bar_base.cpp options.cpp
BAR_DIR = $(IMPLEMENTATION_DIR)/bar

IMPLEMENTATION_OBJECTS = $(IMPLEMENTATION_SOURCES:%.cpp=$(MY_BUILD)/%.o)
STYLE_OBJECTS = $(STYLE_SOURCES:%.cpp=$(MY_BUILD)/%.o)
API_OBJECTS = $(API_SOURCES:%.cpp=$(MY_BUILD)/%.o)
BAR_OBJECTS = $(BAR_SOURCES:%.cpp=$(MY_BUILD)/%.o)
CANVAS_OBJECTS = $(CANVAS_SOURCES:%.cpp=$(MY_BUILD)/%.o)


# ==============================================================

.PHONY: all
all: $(API_TARGET_DLL) $(EXECUTABLE)

# -------------------------------------------------------------------------------

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(API_TARGET_DLL)

$(MY_BUILD)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

# -------------------------------------------------------------------------------

$(API_TARGET_DLL): $(API_OBJECTS) $(IMPLEMENTATION_OBJECTS) $(STYLE_OBJECTS) $(CANVAS_OBJECTS) $(BAR_OBJECTS)
	$(CXX) -dynamiclib $^ -o $@ $(CXXFLAGS)

$(MY_BUILD)/%.o : $(API_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(MY_BUILD)/%.o : $(STYLE_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(MY_BUILD)/%.o : $(CANVAS_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(MY_BUILD)/%.o : $(BAR_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(MY_BUILD)/%.o : $(IMPLEMENTATION_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

# -------------------------------------------------------------------------------

.PHONY: clean makedirs plugins

clean:
	rm -rf  $(EXECUTABLE) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.dylib $(BUILD_DIR)/*.so $(BUILD_DIR)/*.dSYM $(MY_BUILD)/*.o $(MY_BUILD)/*.dll

makedirs:
	mkdir -p $(BUILD_DIR)

plugins:
	$(CXX) -dynamiclib plugins/canvas/canvas.cpp -o $(BUILD_DIR)/canvas.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/toolbar/toolbar.cpp -o $(BUILD_DIR)/toolbar.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/menubar.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/menubar.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/brush/brush.cpp plugins/brush/catmull.cpp -o $(BUILD_DIR)/brush.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/filters/filters.cpp plugins/menubar/filters/filtersbar.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/filters.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/geometry/geometry.cpp -o $(BUILD_DIR)/geometry.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/optionbar/optionbar.cpp -o $(BUILD_DIR)/optionbar.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/files/filebar.cpp plugins/menubar/files/import.cpp plugins/menubar/files/export.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/files.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/edit/edit.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/edit.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/layer/layer.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/layer.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
	$(CXX) -dynamiclib plugins/menubar/help/help.cpp plugins/menubar/submenubar.cpp -o $(BUILD_DIR)/help.dylib $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dylib
