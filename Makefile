CXX = g++
CXXFLAGS = -std=c++23 -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror  
LDFLAGS =

SRC = server.cpp urlHandler.cpp requestHandler.cpp converter.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = server

all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS)
