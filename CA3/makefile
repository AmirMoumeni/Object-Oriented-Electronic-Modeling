SYSTEMC_HOME = /home/amir/Desktop/systemc-install
SRC = test.cpp
INCLUDE = -I$(SYSTEMC_HOME)/include
LIBRARY_PATH = -L$(SYSTEMC_HOME)/lib-linux64
LIBRARY = -lsystemc
CXX = g++
CXXFLAGS = -std=c++17
TARGET = test_systemc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INCLUDE) $(LIBRARY_PATH) $(LIBRARY) -o $(TARGET)

clean:
	rm -f $(TARGET)





# SYSTEMC_HOME ?= /Users/mahdis/systemc
# CXX       := g++
# CXXFLAGS  := -std=c++17 -Wall -Wextra
# INCLUDE   := -I$(SYSTEMC_HOME)/include
# LIBPATH   := -L$(SYSTEMC_HOME)/lib
# LIBS      := -lsystemc -lm -pthread
# SRC       := test.cpp
# OBJ       := $(SRC:.cpp=.o)
# TARGET    := test_systemc
# all: $(TARGET)
# $(TARGET): $(OBJ)
# 	$(CXX) $(CXXFLAGS) $^ $(LIBPATH) $(LIBS) -o $@
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
# clean:
# 	rm -f $(OBJ) $(TARGET)
# .PHONY: all clean
