.PHONY: all clean benchmark test

CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: benchmark

benchmark: benchmark.cpp
	$(CXX) $(CXXFLAGS) -o benchmark benchmark.cpp

test: benchmark
	@echo "=== Test 1: Length 100 ==="
	python3 test_generator.py 100 random | ./benchmark
	@echo ""
	@echo "=== Test 2: Length 1000 ==="
	python3 test_generator.py 1000 random | ./benchmark
	@echo ""
	@echo "=== Test 3: Length 10000 ==="
	python3 test_generator.py 10000 random | ./benchmark
	@echo ""
	@echo "=== Test 4: Repetitive Pattern ==="
	python3 test_generator.py 5000 repetitive | ./benchmark
	@echo ""
	@echo "=== Test 5: Small Alphabet ==="
	python3 test_generator.py 5000 small_alphabet | ./benchmark
	@echo ""
	@echo "=== Test 6: Worst Case ==="
	python3 test_generator.py 100 worst_case | ./benchmark

clean:
	rm -f benchmark
