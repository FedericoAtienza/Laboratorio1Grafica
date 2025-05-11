SRC_DIR = Laboratorio1
BUILD_DIR = debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) 
OBJ_NAME = app
INCLUDE_PATHS = -Iinclude -IDependencias
LIBRARY_PATHS = -Llib/SDL2 -Llib/Freeimage -Llib/assimp
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -Wno-deprecated-declarations
LINKER_FLAGS = -lsdl2 -lfreeimage -lassimp -lSDL2_mixer
FRAMEWORK_PATHS = -framework OpenGL

# Ruta donde estan las libs de assimp
RUNTIME_PATH = -Wl,-rpath,@executable_path/../lib/assimp

# Regla para compilar
all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(FRAMEWORK_PATHS) $(RUNTIME_PATH) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

# Regla para ejecutar el programa
run:
	cd $(BUILD_DIR) && ./$(OBJ_NAME)