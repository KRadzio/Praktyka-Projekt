#compiler
CC=g++
#include directories
IMGUI_DIR=./lib/imgui
ALGS_DIR=/algorithms
#project include dir
INCLUDE=./include
INCLUDE_DIRS=$(INCLUDE)
INCLUDE_DIRS+=$(IMGUI_DIR)
INCLUDE_DIRS+=$(INCLUDE)$(ALGS_DIR)
#project src dir
SRC=./src
SRC_DIRS=$(SRC)
SRC_DIRS+=$(IMGUI_DIR)
SRC_DIRS+=$(SRC)$(ALGS_DIR)
# libs
# `pkg-config --cflags --libs opencv` (if opencv)
LIBS=-ldl -lSDL2_image `sdl2-config --libs`
# standard
STD=-std=c++17
#warnings
WARN=-Wall -Wextra -Wnull-dereference -Wpedantic -Wformat=2
#extra warnings (for later)
WARNEXTRA=-Wshadow -Wcast-align -Wconversion -Wsign-conversion
# dependency flags
DEPFLAGS=-MP -MD
#debug flag
DEBUGFLAG=-g -fsanitize=address
#all flags exept debug flag
CFLAGS=$(foreach D, $(INCLUDE_DIRS), -I$(D)) $(STD) $(WARN) $(DEPFLAGS) `sdl2-config --cflags` 
CPPFILES=$(foreach D, $(SRC_DIRS), $(wildcard $(D)/*.cpp))
CPPFILES+=$(SRC_BACKEND)

#obj and dep
OBJS=$(addsuffix .o, $(basename $(notdir $(CPPFILES))))
DEPS=$(addsuffix .d, $(basename $(notdir $(CPPFILES))))

#relase obj and dep
OBJSR=$(addprefix $(RELASE), $(OBJS))
DEPSR=$(addprefix $(RELASE), $(DEPS))

#debug obj and dep
OBJSD=$(addprefix $(DEBUG), $(OBJS))
DEPSD=$(addprefix $(DEBUG), $(DEPS))

#output locations
OUT=prog
DEBUG=./bin/debug/
RELASE=./bin/relase/

all: $(RELASE)

$(RELASE): $(OBJSR)
	$(CC) -o $@$(OUT) $^ $(LIBS)

$(RELASE)%.o:$(SRC)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(RELASE)%.o:$(IMGUI_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(RELASE)%.o:$(SRC)$(ALGS_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


debug: $(DEBUG)

$(DEBUG): $(OBJSD)
	$(CC) $(DEBUGFLAG) -o $@$(OUT) $^ $(LIBS)

$(DEBUG)%.o:$(SRC)/%.cpp
	$(CC) $(CFLAGS) $(DEBUGFLAG) -c -o $@ $<

$(DEBUG)%.o:$(IMGUI_DIR)/%.cpp
	$(CC) $(CFLAGS) $(DEBUGFLAG) -c -o $@ $<

$(DEBUG)%.o:$(SRC)$(ALGS_DIR)/%.cpp
	$(CC) $(CFLAGS) $(DEBUGFLAG) -c -o $@ $<

clean:
	rm -rf $(RELASE)$(OUT) $(DEBUG)$(OUT) $(OBJSR) $(DEPSR) $(OBJSD) $(DEPSD)

cleanRelase:
	rm -rf $(RELASE)$(OUT) $(OBJSR) $(DEPSR)

cleanDebug:
	rm -rf $(DEBUG)$(OUT) $(OBJSD) $(DEPSD)

diff:
	@git status
	@git diff --stat

-include $(DEPSR)
-include $(DEPSD)