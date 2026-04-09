#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <tuple>

// Поднимаемся на уровень выше, чтобы подключить заголовочный файл
#include "AVL/avl.hpp" 

int main()
{
    avl::AVL<std::string, uint64_t> root;
    
    // Данные для тестирования (Ключ - строка, Значение - uint64_t)
    std::vector<std::tuple<std::string, uint64_t>> records = {
        std::make_tuple("walter", 1),
        std::make_tuple("jesse", 2),
        std::make_tuple("saul", 3),
        std::make_tuple("mike", 4)
    };
    
    //! Проверяем, что дерево пустое.
    assert(root.empty());

    for (auto [key, value]: records)
    {
        //! Проверяем, что добавление уникальных элементов успешно.
        assert(root.insert(key, value));
    }
    
    //! Теперь дерево не должно быть пустым.
    assert(!root.empty());

    //! Количество узлов должно совпадать с количеством records.
    assert(root.size() == records.size());

    //! Проверяем успешный поиск
    {
        auto [key, value] = records.back();
        auto [success, found_value] = root.find(key);

        assert( success );
        assert( value == found_value );
    }

    //! Проверяем успешное удаление
    {
        auto [key, value] = records.front(); // Удаляем "walter"
        assert(root.remove(key));            // Удаление должно вернуть true
        assert(root.size() == records.size() - 1); // Размер должен уменьшиться
        
        // Пытаемся найти удаленный элемент
        auto [success, found_value] = root.find(key);
        assert( !success );
    }

    root.clear();
    //! Теперь дерево пустое.
    assert(root.empty());
    
    //! И мы ничего не найдём больше в нём.
    {
        auto [key, value] = records.back();
        auto [success, found_value] = root.find(key);

        assert( !success );
    }
    
    std::cout << "All basic tests passed successfully!" << std::endl;

    return 0;
}