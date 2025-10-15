// реализация iterator
template <typename T>
class val::vec<T>::iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;


private:
	T* ptr;

public:
	iterator() : ptr(nullptr) {}
	explicit iterator(T* p) : ptr(p) {}

	T& operator*() const { return *ptr; }
	T* operator->() const { return ptr; }

	iterator& operator++() {
		++ptr;
		return *this;
	}
	iterator operator++(int) {
		iterator temp = *this;
		++ptr;
		return temp;
	}

	iterator& operator--() {
		--ptr;
		return *this;
	}
	iterator operator--(int) {
		iterator temp = *this;
		--ptr;
		return temp;
	}

	iterator operator+(std::ptrdiff_t n) const {
		return iterator(ptr + n);
	}
	iterator operator-(std::ptrdiff_t n) const {
		return iterator(ptr - n);
	}

	std::ptrdiff_t operator-(const iterator& other) const {
		return ptr - other.ptr;
	}

	iterator& operator+=(std::ptrdiff_t n) {
		ptr += n;
		return *this;
	}
	iterator& operator-=(std::ptrdiff_t n) {
		ptr -= n;
		return *this;
	}

	bool operator==(const iterator& other) const { return ptr == other.ptr; }
	bool operator!=(const iterator& other) const { return ptr != other.ptr; }
	bool operator<(const iterator& other) const { return ptr < other.ptr; }
	bool operator>(const iterator& other) const { return ptr > other.ptr; }
	bool operator<=(const iterator& other) const { return ptr <= other.ptr; }
	bool operator>=(const iterator& other) const { return ptr >= other.ptr; }

	T& operator[](std::ptrdiff_t n) const { return ptr[n]; }
};
// реализация const_iterator
template <typename T>
class val::vec<T>::const_iterator {
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
	const T* ptr;

public:
	const_iterator() : ptr(nullptr) {}
	explicit const_iterator(const T* p) : ptr(p) {}
	const_iterator(const iterator& it) : ptr(&(*it)) {}

	const T& operator*() const { return *ptr; }
	const T* operator->() const { return ptr; }

	const_iterator& operator++() {
		++ptr;
		return *this;
	}
	const_iterator operator++(int) {
		const_iterator temp = *this;
		++ptr;
		return temp;
	}

	const_iterator& operator--() {
		--ptr;
		return *this;
	}
	const_iterator operator--(int) {
		const_iterator temp = *this;
		--ptr;
		return temp;
	}

	const_iterator operator+(std::ptrdiff_t n) const {
		return const_iterator(ptr + n);
	}
	const_iterator operator-(std::ptrdiff_t n) const {
		return const_iterator(ptr - n);
	}

	std::ptrdiff_t operator-(const const_iterator& other) const {
		return ptr - other.ptr;
	}

	const_iterator& operator+=(std::ptrdiff_t n) {
		ptr += n;
		return *this;
	}
	const_iterator& operator-=(std::ptrdiff_t n) {
		ptr -= n;
		return *this;
	}

	bool operator==(const const_iterator& other) const { return ptr == other.ptr; };
	bool operator!=(const const_iterator& other) const { return ptr != other.ptr; };
	bool operator<(const const_iterator& other) const { return ptr < other.ptr; };
	bool operator>(const const_iterator& other) const { return ptr > other.ptr; };
	bool operator<=(const const_iterator& other) const { return ptr <= other.ptr; };
	bool operator>=(const const_iterator& other) const { return ptr >= other.ptr; };

	const T& operator[](std::ptrdiff_t n) const { return ptr[n]; }
};

// конструкторы
template <typename T>
val::vec<T>::vec() : size(0), capacity(0) {}

template <typename T>
val::vec<T>::vec(size_t initialCapacity) : size(0), capacity(initialCapacity) {
	if (capacity > 0) {
		data = std::make_unique<T[]>(capacity);
	}
}

template <typename T>
val::vec<T>::vec(const vec& other) : size(other.size), capacity(other.capacity) {
	if (capacity > 0) {
		data = std::make_unique<T[]>(capacity);
		for (size_t i = 0; i < size; ++i) {
			data[i] = other.data[i];
		}
	}
}
template <typename T>
val::vec<T>::vec(vec&& other) noexcept : data(std::move(other.data)), size(other.size), capacity(other.capacity) {
	other.data = nullptr;
	other.size = 0;
	other.capacity = 0;
}

// операторы присваивания
template <typename T>
val::vec<T>& val::vec<T>::operator=(const vec& other) {
	if (this != &other) {
		auto newData = std::make_unique<T[]>(other.capacity);
		for (size_t i = 0; i < other.size; ++i) {
			newData[i] = other.data[i];
		}

		data = std::move(newData);
		size = other.size;
		capacity = other.capacity;
	}

	return *this;
}
template <typename T>
val::vec<T>& val::vec<T>::operator=(vec&& other) noexcept {
	if (this != &other) {
		data = std::move(other.data);
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
	data[size++] = value;
}
template <typename T>
void val::vec<T>::append(T&& value) {
	if (size >= capacity) {
		reallocate(capacity == 0 ? 1 : capacity * 2);
	}
	data[size++] = std::move(value);
}
template <typename T>
void val::vec<T>::clear() {
	size = 0;
}
template<typename T>
void val::vec<T>::resize(size_t newSize) {
	if (newSize > capacity) {
		reallocate(newSize);
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
	return data ? iterator(data.get()) : iterator();
}
template <typename T>
auto val::vec<T>::end() noexcept -> vec<T>::iterator {
	return data ? iterator(data.get() + size) : iterator();
}
template <typename T>
auto val::vec<T>::begin() const noexcept -> vec<T>::const_iterator{
	return data ? const_iterator(data.get()) : const_iterator();
}
template <typename T>
auto val::vec<T>::end() const noexcept -> vec<T>::const_iterator{
	return data ? const_iterator(data.get() + size) : const_iterator();
}
template <typename T>
auto val::vec<T>::cbegin() const noexcept -> vec<T>::const_iterator{
	return begin();
}
template <typename T>
auto val::vec<T>::cend() const noexcept -> vec<T>::const_iterator{
	return end();
}

template <typename T>
void val::vec<T>::reallocate(size_t newCapacity) {
	auto newData = std::make_unique<T[]>(newCapacity);

	for (size_t i = 0; i < size; ++i) {
		newData[i] = std::move(data[i]);
	}

	data = std::move(newData);
	capacity = newCapacity;
}