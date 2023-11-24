
CXX = g++

CXXFLAGS = -std=c++17 -Wall


SDL_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf


SRC_DIR = ./
BUILD_DIR = ./build
BIN_DIR = ./


SRC = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)


OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))


TARGET = $(BIN_DIR)/gui_manager

all: $(TARGET)


$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SDL_FLAGS)



$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


.PHONY: all clean
