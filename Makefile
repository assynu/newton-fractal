PROJECT_NAME = newton_fractal
BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/$(PROJECT_NAME)
CMAKE_FLAGS = -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

.PHONY: all build run clean

all: build

build:
	@echo "> Configuring and Building Project..."
	@mkdir -p $(BUILD_DIR)
	@cmake -S . -B $(BUILD_DIR) $(CMAKE_FLAGS)
	@cmake --build $(BUILD_DIR)

run: build
	@echo "> Running Application..."
	@$(EXECUTABLE)

clean:
	@echo "> Cleaning Build Files..."
	@rm -rf $(BUILD_DIR)
