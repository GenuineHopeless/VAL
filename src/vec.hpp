#ifndef VEC_HPP_
#define VEC_HPP_

#include <memory>

namespace val
{
	template <typename T>
	class vec
	{

	public:
		using value_type = T;
		using size_type = size_t;
		using reference = T &;
		using const_reference = const T &;

	private:
		std::unique_ptr<T[]> data;
		size_t size;
		size_t capacity;

	public:
		// итераторы
		class iterator;
		class const_iterator;

		// конструкторы
		vec();
		explicit vec(size_t initialCapacity);
		vec(std::initializer_list<T> init);
		vec(const vec &other);
		vec(vec &&other) noexcept;
		~vec() = default;

		// оператор присваивания
		vec &operator=(const vec &other);
		vec &operator=(vec &&other) noexcept;

		// метод доступа к элементам
		T &operator[](size_t index);
		const T &operator[](size_t index) const;

		// основные операции
		void append(const T &value);
		void append(T &&value);

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
	};
}
#include "vec.tpp"

#endif