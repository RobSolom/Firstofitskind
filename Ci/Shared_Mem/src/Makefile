CXX      := -gcc
CXXFLAGS := -pedantic-errors -std=c11  -D_XOPEN_SOURCE=500 -Wall -Wextra -Werror -Wshadow  \
	-Wcast-align -Wunused -Wnull-dereference -Wformat=2
LDFLAGS  := -L/usr/lib  -lm -lrt
BUILD    := ./build
OBJ_DIR  := $(BUILD)/obj
BIN_DIR  := $(BUILD)/bin
BIN_NAME := exe
TARGET   := target
INCLUDE  := -Iinclude/ -I .
SRC_EXT  := c
SRC      := $(wildcard *.$(SRC_EXT))         
OBJECTS := $(SRC:%.$(SRC_EXT)=$(OBJ_DIR)/%.o)

all: build $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.$(SRC_EXT)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:	
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_DIR)/target $(BIN_NAME)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@echo  "SUCCESSFULLY COMPILED "

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(BIN_DIR)/*
	-@$(RM) -f $(BIN_NAME)

