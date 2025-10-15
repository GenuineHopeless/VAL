#ifndef VAL_VEC_
#define VAL_VEC_

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace val
{
	template <typename T>
	class vec {
	private:
		T* data;
		size_t size;
		size_t capacity;

	public:
		using value_type = T;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;

		// итераторы
		class iterator;
		class const_iterator;

		// конструкторы
		vec() noexcept;
		explicit vec(size_t initialSize);
		vec(std::initializer_list<T> init);
		vec(const vec& other);
		vec(vec&& other) noexcept;
		~vec();

		// оператор присваивания
		vec& operator=(const vec& other);
		vec& operator=(vec&& other) noexcept;

		// метод доступа к элементам
		reference operator[](size_t index);
		const_reference operator[](size_t index) const;

		// основные операции
		void append(const T& value);
		void append(T&& value);

		void clear();
		void resize(size_t newSize);
		void reserve(size_t newCapacity);

		// информационные методы
		size_t getSize() const noexcept;

		// итераторы
		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
	private:
		void reallocate(size_t newCapacity);
		void destroy_range(T* start, T* end) noexcept;
	};
}
#include "vec.tpp"

#endif