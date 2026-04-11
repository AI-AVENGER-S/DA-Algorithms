CXX = g++
CXXFLAGS = -std=c++20 -pedantic -Wall -Wextra -Werror
INCLUDES = -I./AVL

TARGET = lab2
TEST_TARGET = lab2_test
DEMO_TARGET = lab2_demo
PROFILER_TARGET = lab2_profiler
BENCHMARK_TARGET = benchmark

all: $(TARGET) $(TEST_TARGET) $(DEMO_TARGET) $(PROFILER_TARGET) $(BENCHMARK_TARGET)

$(TARGET): lab2.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(TEST_TARGET): test.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(DEMO_TARGET): AVL/main.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@

$(PROFILER_TARGET): lab2.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) -pg $(INCLUDES) $< -o $@

$(BENCHMARK_TARGET): benchmark.cpp AVL/avl.hpp
	$(CXX) $(CXXFLAGS) -O2 $(INCLUDES) $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

benchmark_run: $(BENCHMARK_TARGET)
ifndef TEST
	$(error Please provide TEST path, e.g., make benchmark_run TEST=tests/01)
endif
	./$(BENCHMARK_TARGET) $(TEST)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(DEMO_TARGET) \
	      $(PROFILER_TARGET) $(BENCHMARK_TARGET) *.o gmon.out

.PHONY: all test clean benchmark_run