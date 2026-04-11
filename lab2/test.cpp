#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <tuple>

#include "AVL/avl.hpp" 

int main() {
    avl::AVL<std::string, uint64_t> root;
    
    std::vector<std::tuple<std::string, uint64_t>> records = {
        std::make_tuple("walter", 1),
        std::make_tuple("jesse", 2),
        std::make_tuple("saul", 3),
        std::make_tuple("mike", 4)
    };
    
    assert(root.empty());

    for (auto [key, value]: records){
        assert(root.insert(key, value));
    }
    
    assert(!root.empty());
    assert(root.size() == records.size());

    {
        auto [key, value] = records.back();
        auto [success, found_value] = root.find(key);

        assert( success );
        assert( value == found_value );
    }

    {
        auto [key, value] = records.front();
        assert(root.remove(key));
        assert(root.size() == records.size() - 1);
        
        auto [success, found_value] = root.find(key);
        assert( !success );
    }

    root.clear();
    assert(root.empty());
    
    {
        auto [key, value] = records.back();
        auto [success, found_value] = root.find(key);

        assert(!success);
    }
    
    std::cout << "тесты прошли успешно!" << std::endl;

    return 0;
}