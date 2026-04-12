#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>

#include "AVL/avl.hpp"

enum class ExpectedType { OK, EXIST, NO_SUCH_WORD, UNKNOWN };

struct Operation {
    char type;
    std::string key;
    uint64_t value = 0;
    ExpectedType expected;
};

ExpectedType parse_expected(const std::string& ans) {
    if (ans == "OK") return ExpectedType::OK;
    if (ans == "Exist") return ExpectedType::EXIST;
    if (ans == "NoSuchWord") return ExpectedType::NO_SUCH_WORD;
    return ExpectedType::UNKNOWN; 
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_file_prefix>\n";
        std::cerr << "Example: " << argv[0] << " tests/01\n";
        return 1;
    }

    const std::string prefix = argv[1];
    std::ifstream input(prefix + ".t");
    std::ifstream answers(prefix + ".a");

    if (!input.is_open() || !answers.is_open()) {
        std::cerr << "ERROR: Cannot open input or answer files.\n";
        return 1;
    }

    std::cout << "Loading data into memory..." << std::flush;

    std::ios::sync_with_stdio(false);

    std::vector<Operation> ops;
    std::string action, expected_str;
    
    while (input >> action) {
        Operation op;
        if (action == "+") {
            op.type = '+';
            input >> op.key >> op.value;
        } else if (action == "-") {
            op.type = '-';
            input >> op.key;
        } else {
            op.type = '?';
            op.key = action;
        }

        if (!(answers >> expected_str)) {
            std::cerr << "\nERROR: Answer file is shorter than input file.\n";
            return 1;
        }
        op.expected = parse_expected(expected_str);
        ops.push_back(op);
    }
    
    std::cout << " Done. Loaded " << ops.size() << " operations.\n\n";

    std::map<std::string, uint64_t> rb_tree;
    size_t rb_mismatches = 0;
    
    std::cout << "Running std::map... " << std::flush;
    auto rb_start = std::chrono::high_resolution_clock::now();
    
    for (const auto& op : ops) {
        if (op.type == '+') {
            auto [it, inserted] = rb_tree.insert({op.key, op.value});
            ExpectedType res = inserted ? ExpectedType::OK : ExpectedType::EXIST;
            if (res != op.expected) rb_mismatches++;
        } else if (op.type == '-') {
            size_t erased = rb_tree.erase(op.key);
            ExpectedType res = erased ? ExpectedType::OK : ExpectedType::NO_SUCH_WORD;
            if (res != op.expected) rb_mismatches++;
        }
    }
    
    auto rb_end = std::chrono::high_resolution_clock::now();
    auto rb_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(rb_end - rb_start).count();
    std::cout << "Done.\n";

    avl::AVL<std::string, uint64_t> avl_tree;
    size_t avl_mismatches = 0;

    std::cout << "Running avl::AVL... " << std::flush;
    auto avl_start = std::chrono::high_resolution_clock::now();
    
    for (const auto& op : ops) {
        if (op.type == '+') {
            bool inserted = avl_tree.insert(op.key, op.value);
            ExpectedType res = inserted ? ExpectedType::OK : ExpectedType::EXIST;
            if (res != op.expected) avl_mismatches++;
        } else if (op.type == '-') {
            bool removed = avl_tree.remove(op.key);
            ExpectedType res = removed ? ExpectedType::OK : ExpectedType::NO_SUCH_WORD;
            if (res != op.expected) avl_mismatches++;
        }
    }
    
    auto avl_end = std::chrono::high_resolution_clock::now();
    auto avl_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(avl_end - avl_start).count();
    std::cout << "Done.\n\n";

    std::cout << "=== BENCHMARK RESULTS ===\n";
    std::cout << "Total Operations : " << ops.size() << "\n";
    std::cout << "-------------------------\n";
    std::cout << "std::map (RB)    : " << std::setw(6) << rb_time_ms << " ms | Mismatches: " << rb_mismatches << "\n";
    std::cout << "avl::AVL         : " << std::setw(6) << avl_time_ms << " ms | Mismatches: " << avl_mismatches << "\n";
    std::cout << "-------------------------\n";
    
    if (avl_mismatches == 0 && rb_mismatches == 0) {
        std::cout << "SUCCESS: Both trees produced 100% correct results.\n";
    } else {
        std::cout << "WARNING: There were logic errors (mismatches with .a file)!\n";
    }

    return 0;
}