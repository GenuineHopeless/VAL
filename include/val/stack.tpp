#include <utility>
#include <new>

template <typename T>
val::stack<T>::stack() : capacity(DEFAULT_CAPACITY), size(0) {
    data.reset(static_cast<T*>(::operator new(sizeof(T) * capacity)));
}
template <typename T>
val::stack<T>::~stack() {
    deleteElements();
}

template <typename T>
void val::stack<T>::push(const T& value) {
    if (size >= capacity) {
        resize(capacity * 2);
    }

    new (&data[size]) T(value);
    ++size;
}
template <typename T>
void val::stack<T>::push(T&& value) {
    if (size >= capacity) {
        resize(capacity * 2);
    }

    new (&data[size]) T(std::move(value));
    ++size;
}

template <typename T>
void val::stack<T>::pop() {
    if (size == 0) {
        throw std::out_of_range("Pop with empty stack");
    }
    if constexpr (!std::is_trivially_destructible_v<T>) {
        data[size - 1].~T();
    }

    --size;
}

template<typename T>
T& val::stack<T>::top() {
    if (size == 0) {
        throw std::out_of_range("stack is empty");
    }

    return data[size - 1];
}
template<typename T>
const T& val::stack<T>::top() const {
    if (size == 0) {
        throw std::out_of_range("stack is empty");
    }

    return data[size - 1];
}

template <typename T>
const size_t val::stack<T>::getSize() const {
    return size;
}

template <typename T>
void val::stack<T>::resize(size_t newCapacity) {
    std::unique_ptr<T[], ArrayDeleter> newData(
        static_cast<T*>(::operator new(sizeof(T) * newCapacity))
    );

    for (size_t i = 0; i < size; ++i){
        new (&newData[i]) T(std::move(data[i]));

        if constexpr(!std::is_trivially_destructible_v<T>){
            data[i].~T();
        }
    }

    data = std::move(newData);
    capacity = newCapacity;
}

template <typename T>
void val::stack<T>::deleteElements(){
    if (data){
        if constexpr(!std::is_trivially_destructible_v<T>){
            for (size_t i = 0; i < size; ++i){
                data[i].~T();
            }
        }
    }
}
