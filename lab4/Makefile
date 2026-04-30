CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++20

TARGET = benchmark
SRC = benchmark.cpp
GEN_SCRIPT = test_generator.py
TEST_DATA = test_input.txt

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

gen:
	@echo "Generating test data..."
	python3 $(GEN_SCRIPT)

run: $(TARGET)
	@if [ ! -f $(TEST_DATA) ]; then \
		$(MAKE) gen; \
	fi
	@echo "Running benchmark..."
	./$(TARGET) < $(TEST_DATA)

bench: clean all gen run

clean:
	rm -f $(TARGET) $(TEST_DATA)
	rm -rf __pycache__

