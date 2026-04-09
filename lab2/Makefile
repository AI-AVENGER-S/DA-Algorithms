CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra -Werror
INCLUDES = -I./AVL

all: lab2 lab2_test lab2_demo lab2_profiler

lab2: lab2.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) lab2.cpp -o lab2

lab2_test: test.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) test.cpp -o lab2_test

lab2_demo: AVL/main.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) AVL/main.cpp -o lab2_demo

lab2_profiler: lab2.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) -pg $(INCLUDES) lab2.cpp -o lab2_profiler

test:
	./lab2_test

clean:
	rm -f lab2 lab2_test lab2_demo lab2_profiler *.o