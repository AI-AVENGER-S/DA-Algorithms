CXX = g++
CXXFLAGS = -std=c++14 -O3 -pedantic -Wall -Wextra -Wno-unused-variable -Iinclude

PYTHON = python3
TARGET = lab1

BENCH_SRC = src/benchmark.cpp src/sort.cpp
BENCH_OBJ = src/benchmark.o src/sort.o

SRC = main.cpp src/sort.cpp
OBJ = main.o src/sort.o

INPUT = input.txt
OUTPUT_FILE = output.txt
NUM_RECORDS = 10_000

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

generate:
	$(PYTHON) src/generator.py $(NUM_RECORDS) $(INPUT)

test-save: $(TARGET) generate
	./$(TARGET) < $(INPUT) > $(OUTPUT_FILE)

check: test-save
	$(PYTHON) src/checker.py $(OUTPUT_FILE)

benchmark: $(BENCH_OBJ)
	$(CXX) $(CXXFLAGS) $(BENCH_OBJ) -o benchmark_run

run-bench: benchmark generate
	./benchmark_run < $(INPUT)

clean:
	rm -f $(OBJ) $(TARGET) $(OUTPUT_FILE) $(INPUT)

rebuild: clean all