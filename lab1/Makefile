CXX = g++
CXXFLAGS = -std=c++14 -pedantic -Wall -Wextra -Wno-unused-variable -Iinclude

PYTHON = python3
TARGET = lab1

SRC = main.cpp src/sort.cpp
OBJ = $(SRC:.cpp=.o)

INPUT = input.txt
OUTPUT_FILE = output.txt

# Параметры генерации (можно менять при вызове make)
NUM_RECORDS = 10000
MAX_VAL_LEN = 100
MIN_VAL_LEN = 1
SEED =

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

generate:
	$(PYTHON) src/generator.py -n $(NUM_RECORDS) -m $(MAX_VAL_LEN) -M $(MIN_VAL_LEN) $(if $(SEED),-s $(SEED),) -o $(INPUT)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET) generate
	./$(TARGET) < $(INPUT)

test-save: $(TARGET) generate
	./$(TARGET) < $(INPUT) > $(OUTPUT_FILE)

check: test-save
	$(PYTHON) src/checker.py $(OUTPUT_FILE) -m $(MAX_VAL_LEN) -v

clean:
	rm -f $(OBJ) $(TARGET) $(OUTPUT_FILE) $(INPUT)

rebuild: clean all