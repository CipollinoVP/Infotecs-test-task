CC=g++
CXXFLAGS = -std=c++17 -Wall -I./include
LIB_DIR = lib
SRC_DIR = src
INC_DIR = include

all: make_library make_application

make_library:
	mkdir -p $(LIB_DIR)
	$(CC) $(CXXFLAGS) -shared -fPIC -o ./$(LIB_DIR)/libjournal.so ./$(SRC_DIR)/journal.cpp

make_application:
	$(CC) $(CXXFLAGS) -o test_application ./main.cpp -L./$(LIB_DIR) -ljournal -Wl,-rpath,./$(LIB_DIR)

clean:
	rm -r ./lib
	rm test_application
