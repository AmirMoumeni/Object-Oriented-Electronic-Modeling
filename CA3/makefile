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
