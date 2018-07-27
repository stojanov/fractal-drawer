TARGET=app
STD = 11

CC = g++
CFLAGS = -std=c++$(STD) -Wall -Wextra -pedantic

SRC_DIR ?= ./src
BUILD_DIR ?= ./bin
OBJ_DIR ?= ./build
INC_DIR ?= ./include

LDLIBS := $(shell pkg-config sfml-all --libs) -pthread

SRCS := $(shell find $(SRC_DIR) -name *.cpp)

_OBJS :=  $(addsuffix .o, $(basename $(shell basename -a $(SRCS))))

OBJS := $(patsubst %, $(OBJ_DIR)/%, $(_OBJS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@  

$(TARGET): $(OBJS) 
	$(CC) -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

build: $(TARGET)

run: $(TARGET)
	clear \
	&& $(BUILD_DIR)/$(TARGET)

.PHONY: clean

create_dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)/* $(OBJ_DIR)/*
