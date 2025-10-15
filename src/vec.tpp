using namespace val;

#include <new>
#include <memory>
#include <algorithm>

// реализация iterator
template <typename T>
class vec<T>::iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

private:
	T* ptr;

public:
	iterator() noexcept : ptr(nullptr) {}
	explicit iterator(T* p) noexcept : ptr(p) {}

	reference operator*() const noexcept { return *ptr; }
	pointer operator->() const noexcept { return ptr; }

	iterator& operator++() noexcept {
		++ptr;
		return *this;
	}
	iterator operator++(int) noexcept {
		iterator temp = *this;
		++ptr;
		return temp;
	}

	iterator& operator--() noexcept {
		--ptr;
		return *this;
	}
	iterator operator--(int) noexcept {
		iterator temp = *this;
		--ptr;
		return temp;
	}

	iterator operator+(difference_type n) const noexcept {
		return iterator(ptr + n);
	}
	iterator operator-(difference_type n) const noexcept {
		return iterator(ptr - n);
	}

	difference_type operator-(const iterator& other) const noexcept {
		return ptr - other.ptr;
	}

	iterator& operator+=(difference_type n) noexcept {
		ptr += n;
		return *this;
	}
	iterator& operator-=(difference_type n) noexcept {
		ptr -= n;
		return *this;
	}

	bool operator==(const iterator& other) const noexcept { return ptr == other.ptr; }
	bool operator!=(const iterator& other) const noexcept { return ptr != other.ptr; }
	bool operator<(const iterator& other) const noexcept { return ptr < other.ptr; }
	bool operator>(const iterator& other) const noexcept { return ptr > other.ptr; }
	bool operator<=(const iterator& other) const noexcept { return ptr <= other.ptr; }
	bool operator>=(const iterator& other) const noexcept { return ptr >= other.ptr; }

	reference operator[](difference_type n) const noexcept { return ptr[n]; }
};
// реализация const_iterator
template <typename T>
class vec<T>::const_iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;

private:
	const T* ptr;

public:
	const_iterator() noexcept : ptr(nullptr) {}
	explicit const_iterator(const T* p) noexcept : ptr(p) {}

	const_iterator(const iterator& it) noexcept : ptr(it.operator->()) {}

	reference operator*() const noexcept { return *ptr; }
	pointer operator->() const noexcept { return ptr; }

	const_iterator& operator++() noexcept {
		++ptr;
		return *this;
	}
	const_iterator operator++(int) noexcept {
		const_iterator temp = *this;
		++ptr;
		return temp;
	}

	const_iterator& operator--() noexcept {
		--ptr;
		return *this;
	}
	const_iterator operator--(int) noexcept {
		const_iterator temp = *this;
		--ptr;
		return temp;
	}

	const_iterator operator+(difference_type n) const noexcept {
		return const_iterator(ptr + n);
	}
	const_iterator operator-(difference_type n) const noexcept {
		return const_iterator(ptr - n);
	}

	difference_type operator-(const const_iterator& other) const noexcept {
		return ptr - other.ptr;
	}

	const_iterator& operator+=(difference_type n) noexcept {
		ptr += n;
		return *this;
	}
	const_iterator& operator-=(difference_type n) noexcept {
		ptr -= n;
		return *this;
	}

	bool operator==(const const_iterator& other) const noexcept { return ptr == other.ptr; };
	bool operator!=(const const_iterator& other) const noexcept { return ptr != other.ptr; };
	bool operator<(const const_iterator& other) const noexcept { return ptr < other.ptr; };
	bool operator>(const const_iterator& other) const noexcept { return ptr > other.ptr; };
	bool operator<=(const const_iterator& other) const noexcept { return ptr <= other.ptr; };
	bool operator>=(const const_iterator& other) const noexcept { return ptr >= other.ptr; };

	reference operator[](difference_type n) const { return ptr[n]; }
};

// конструкторы
template <typename T>
val::vec<T>::vec() noexcept : data(nullptr), size(0), capacity(0) {}
template <typename T>
val::vec<T>::vec(size_t initialSize) : data(nullptr), size(initialSize), capacity(initialSize) {
	if (capacity > 0) {
		data = static_cast<T*>(::operator new(capacity * sizeof(T)));
		size_t constructed = 0;
		try {
			for (; constructed < size; ++constructed) {
				new (data + constructed) T{};
			}
		}
		catch (...) {
			destroy_range(data, data + constructed);
			::operator delete(data);
			throw;
		}
	}
}
template <typename T>
val::vec<T>::vec(std::initializer_list<T> init) : data(nullptr), size(init.size()), capacity(init.size()) {
	if (capacity > 0) {
		data = static_cast<T*>(::operator new(capacity * sizeof(T)));
		size_t constructed = 0;
		try {
			for (const auto& item : init) {
				new(data + constructed) T(item);
				constructed++;
			}
		}
		catch (...) {
			destroy_range(data, data + constructed);
			::operator delete (data);
			throw;
		}
	}
}
template <typename T>
val::vec<T>::vec(const vec& other) : data(nullptr), size(other.size), capacity(other.capacity) {
	if (capacity > 0) {
		data = static_cast<T*>(::operator new(capacity * sizeof(T)));
		size_t constructed = 0;

		try {
			for (; constructed < size; ++constructed) {
				new (data + constructed) T(other.data[constructed]);
			}
		}
		catch (...) {
			destroy_range(data, data + constructed);
			::operator delete(data);
			throw;
		}
	}
}
template <typename T>
val::vec<T>::vec(vec&& other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
	other.data = nullptr;
	other.size = 0;
	other.capacity = 0;
}
template <typename T>
val::vec<T>::~vec() {
	if (data) {
		destroy_range(data, data + size);
		::operator delete(data);
	}
}

// операторы присваивания
template <typename T>
val::vec<T>& val::vec<T>::operator=(const vec& other) {
	if (this != &other) {
		vec<T> temp(other);
		*this = std::move(temp);
	}

	return *this;
}
template <typename T>
val::vec<T>& val::vec<T>::operator=(vec&& other) noexcept {
	if (this != &other) {
		if (data) {
			destroy_range(data, data + size);
			::operator delete(data);
		}

		data = other.data;
		size = other.size;
		capacity = other.capacity;

		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}

	return *this;
}

// методы доступа к элементам
template <typename T>
T& val::vec<T>::operator[](size_t index) {
	return data[index];
}
template <typename T>
const T& val::vec<T>::operator[](size_t index) const {
	return data[index];
}

// основные операции
template <typename T>
void val::vec<T>::append(const T& value) {
	if (size >= capacity) {
		reallocate(capacity == 0 ? 1 : capacity * 2);
	}
	new (data + size) T(value);
	++size;
}
template <typename T>
void val::vec<T>::append(T&& value) {
	if (size >= capacity) {
		reallocate(capacity == 0 ? 1 : capacity * 2);
	}
	new (data + size) T(std::move(value));
	++size;
}
template <typename T>
void val::vec<T>::clear() {
	destroy_range(data, data + size);
	size = 0;
}
template<typename T>
void val::vec<T>::resize(size_t newSize) {
	if (newSize > capacity) {
		reallocate(newSize);
	}

	if (newSize > size) {
		for (size_t i = size; i < newSize; ++i) {
			new (data + i) T{};
		}
	}
	else if (newSize < size) {
		destroy_range(data + newSize, data + size);
	}

	size = newSize;
}
template <typename T>
void val::vec<T>::reserve(size_t newCapacity) {
	if (newCapacity > capacity) {
		reallocate(newCapacity);
	}
}

// информационные методы
template <typename T>
size_t val::vec<T>::getSize() const noexcept {
	return size;
}

// итераторы
template <typename T>
auto val::vec<T>::begin() noexcept -> vec<T>::iterator {
	return iterator(data);
}
template <typename T>
auto val::vec<T>::end() noexcept -> vec<T>::iterator {
	return iterator(data + size);
}
template <typename T>
auto val::vec<T>::begin() const noexcept -> vec<T>::const_iterator {
	return const_iterator(data);
}
template <typename T>
auto val::vec<T>::end() const noexcept -> vec<T>::const_iterator {
	return const_iterator(data + size);
}
template <typename T>
auto val::vec<T>::cbegin() const noexcept -> vec<T>::const_iterator {
	return const_iterator(data);
}
template <typename T>
auto val::vec<T>::cend() const noexcept -> vec<T>::const_iterator {
	return const_iterator(data + size);
}

template <typename T>
void val::vec<T>::reallocate(size_t newCapacity) {
	T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
	size_t moved = 0;

	try {
		for (; moved < size; ++moved) {
			new (newData + moved) T(data[moved]);
		}
	}
	catch (...) {
		destroy_range(newData, newData + moved);
		::operator delete(newData);
		throw;
	}

	destroy_range(data, data + size);
	::operator delete(data);

	data = newData;
	capacity = newCapacity;
}

template <typename T>
void val::vec<T>::destroy_range(T* start, T* end) noexcept {
	if constexpr (!std::is_trivially_destructible_v<T>) {
		for (T* ptr = start; ptr != end; ++ptr) {
			ptr->~T();
		}
	}
}