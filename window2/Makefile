# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC_DIR 	= src
BUILD_DIR 	= build
TARGET 		= main
FLFLAGS = $(shell fltk-config --cxxflags)
FLLIBS = $(shell fltk-config --ldflags)

SRC = ${wildcard $(SRC_DIR)/*.cpp}
OBJ = ${patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,${SRC}}

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p ${dir $@}
	$(CC) $(FLFLAGS) $(CFLAGS) -o $@ $< -c

all: ${OBJ}
	$(CC) $(FLFLAGS) $(CFLAGS) -I$(SRC_DIR) -o $(TARGET) ${OBJ} $(FLLIBS)

clean:
	$(RM) -rf $(BUILD_DIR)
