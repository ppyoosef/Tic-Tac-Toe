# Makefile for Tic Tac Toe SDL3 App

# Compiler
CXX = g++

# Paths
INCLUDES = -Iinclude
LIBS = -Llib -lSDL3

# Files
SRC = main.cpp
OUT_DIR = build
TARGET = $(OUT_DIR)/tic-tac-toe.exe
DLL_SRC = lib/SDL3.dll
DLL_DEST = $(OUT_DIR)/SDL3.dll

# Default target
all: $(TARGET) copy-dll

# Build Rule
$(TARGET): $(SRC)
#	@mkdir -p $(OUT_DIR) #for linux
	mkdir $(OUT_DIR)
	$(CXX) $(SRC) -o $(TARGET) $(INCLUDES) $(LIBS)


# Copy DLL after build
copy-dll:
	@cp $(DLL_SRC) $(DLL_DEST)
	

# Clean Rule
clean:
#	@rm -rf $(OUT_DIR)
	del /Q $(OUT_DIR)\*

# Default Target
all: $(TARGET)
