#include <cctype>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

#include "AVL/avl.hpp"

void to_lower(std::string& str) {
    for (char& c : str) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    avl::AVL<std::string, uint64_t> dictionary;
    std::string command;

    while (std::cin >> command) {
        try {
            if (command == "+") {
                std::string word;
                uint64_t value;
                std::cin >> word >> value;
                to_lower(word);

                if (dictionary.insert(word, value)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "Exist\n";
                }
            } else if (command == "-") {
                std::string word;
                std::cin >> word;
                to_lower(word);

                if (dictionary.remove(word)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
            } else if (command == "!") {
                std::string action;
                std::string path;
                std::cin >> action >> path;

                if (action == "Save") {
                    if (dictionary.save(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Failed to save dictionary to "
                                  << path << '\n';
                    }
                } else if (action == "Load") {
                    if (dictionary.load(path)) {
                        std::cout << "OK\n";
                    } else {
                        std::cout << "ERROR: Failed to load dictionary from "
                                  << path << '\n';
                    }
                }
            } else {
                std::string word = command;
                to_lower(word);

                auto [success, value] = dictionary.find(word);
                if (success) {
                    std::cout << "OK: " << value << "\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
            }
        } catch (const std::bad_alloc& e) {
            std::cout << "ERROR: Out of memory (" << e.what() << ")\n";
        } catch (const std::exception& e) {
            std::cout << "ERROR: System error occurred (" << e.what() << ")\n";
        }
    }

    return 0;
}