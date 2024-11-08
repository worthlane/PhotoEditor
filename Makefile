CXX = g++

EXECUTABLE = app

CXXFLAGS  = -D _DEBUG -ggdb3 -std=c++17 -O3 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts       \
			-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal  \
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline          \
			-Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked            \
			-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo  \
			-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                \
			-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default      \
			-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast           \
			-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing   \
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation    \
			-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192         \
			-Wstack-usage=8192 -fPIE -Werror=vla


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

GRAPHICS_SOURCES = my_sfml.cpp
GRAPHICS_DIR = $(SOURCE_DIR)/graphics

STANDARD_SOURCES = api_system.cpp api_windows.cpp
STANDARD_DIR = $(SOURCE_DIR)/standard

API_SOURCES = utils.cpp window_vector.cpp root_window.cpp
API_DIR = $(SOURCE_DIR)/api

PLUGINS_SOURCES = canvas_plugin.cpp
PLUGINS_DIR = $(SOURCE_DIR)/plugins

OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
GRAPHICS_OBJECTS = $(GRAPHICS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
STANDARD_OBJECTS = $(STANDARD_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
PLUGINS_OBJECTS = $(PLUGINS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
API_OBJECTS = $(API_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# ==============================================================
#						 API
# ==============================================================

API_TARGET_DLL = $(BUILD_DIR)/libapi_photoshop.dll

DLL_API_SOURCES = src/api/utils.cpp src/api/window_vector.cpp src/api/root_window.cpp \
				  src/standard/api_system.cpp src/standard/api_windows.cpp            \
				  src/graphics/my_sfml.cpp

# ==============================================================

.PHONY: all
all: $(API_TARGET_DLL) $(EXECUTABLE)

# -------------------------------------------------------------------------------

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(API_TARGET_DLL)

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(BUILD_DIR)/%.o : $(GRAPHICS_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(BUILD_DIR)/%.o : $(STANDARD_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(BUILD_DIR)/%.o : $(API_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

$(API_TARGET_DLL): $(DLL_API_SOURCES)
	$(CXX) -dynamiclib $^ -o $@ $(CXXFLAGS) $(LOCATION_FLAG)

# -------------------------------------------------------------------------------

.PHONY: clean makedirs plugins

clean:
	rm -rf  $(EXECUTABLE) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.dll $(BUILD_DIR)/*.so $(BUILD_DIR)/*.dSYM

makedirs:
	mkdir -p $(BUILD_DIR)

plugins:
	$(CXX) -dynamiclib plugins/canvas.cpp -o $(BUILD_DIR)/canvas.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
	$(CXX) -dynamiclib plugins/toolbar.cpp -o $(BUILD_DIR)/toolbar.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll
	$(CXX) -dynamiclib plugins/brush.cpp plugins/catmull.cpp plugins/toolbar_button.cpp -o $(BUILD_DIR)/brush.dll $(CXXFLAGS) $(LOCATION_FLAG) $(BUILD_DIR)/libapi_photoshop.dll




