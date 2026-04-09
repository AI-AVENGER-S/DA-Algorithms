#include <iostream>
#include <vector>
#include <cassert>
#include <tuple>
#include <string>
#include <cstdint>

#include "avl.hpp"

int main()
{
    avl::AVL<std::string, uint64_t> tree;

    std::string word;
    std::vector<std::string> words;
    uint64_t index = 0;

    while (std::cin >> word) {
        bool inserted = tree.insert(word, index);
        assert(inserted);
        words.push_back(word);
        ++index;
    }

    for (size_t i = 0; i < words.size(); ++i) {
        auto [found, value] = tree.find(words[i]);
        assert(found);
        assert(value == i);
    }

    
    std::string unknown_word = "gustavo";
    auto [found, value] = tree.find(unknown_word);
    assert(!found);
    

    for (const auto& word : words) {
        bool removed = tree.remove(word);
        assert(removed);

        auto [found, value] = tree.find(word);
        assert(!found);
    }

    assert(tree.empty());
    assert(tree.size() == 0);

    avl::AVL<std::string, uint64_t> save_tree;

    for (size_t i = 0; i < words.size(); ++i) {
        save_tree.insert(words[i], i);
    }

    const std::string filename = "avl_test.bin";

    assert(save_tree.save(filename));

    avl::AVL<std::string, uint64_t> load_tree;
    assert(load_tree.load(filename));
    assert(load_tree.size() == words.size());

    for (size_t i = 0; i < words.size(); ++i) {
        auto [found, value] = load_tree.find(words[i]);
        assert(found);
        assert(value == i);
    }

    std::cout << "OK" << std::endl;
    return 0;
}