#ifndef STACK_HPP_
#define STACK_HPP_

#include <memory>
#include <stdexcept>
#include <cstddef>
#include <type_traits>

namespace val {
    template <typename T>
    class stack {
    private:
        static constexpr size_t DEFAULT_CAPACITY = 10;

        struct ArrayDeleter{
            void operator()(T* ptr) const{
                ::operator delete(ptr);
            }
        };

        std::unique_ptr<T[], ArrayDeleter> data;
        size_t capacity;
        size_t size;

        void resize(size_t newCapacity);
        void deleteElements();
    public:
        // конструкторы
        stack();
        stack(const stack& other) = delete;
        stack(stack&& other) noexcept = default;
        ~stack();
        
        // оператор присваивания
        stack& operator=(const stack& other) = delete;
        stack& operator=(stack&& other) noexcept = default;

        // основные операции
        void push(const T& value);
        void push(T&& value);
        void pop();
        T& top();
        const T& top() const;
        
        // информационные методы
        const size_t getSize() const;
    };

}

#include "stack.tpp"

#endif