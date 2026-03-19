#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
	T *data;
	size_t currentSize;
	size_t currentCapacity;

	void resize(size_t newCapacity) {
		if (newCapacity <= currentCapacity) return;

		T *newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));
		for (size_t i = 0; i < currentSize; ++i) {
			new (newData + i) T(std::move(data[i]));
			data[i].~T();
		}
		if (data) ::operator delete(data);
		data = newData;
		currentCapacity = newCapacity;
	}

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		T *ptr;
		vector<T> *container;

		friend class vector<T>;
	public:
		iterator(T *p = nullptr, vector<T> *c = nullptr) : ptr(p), container(c) {}

		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, container);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, container);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator temp = *this;
			++ptr;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator temp = *this;
			--ptr;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const {
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T *ptr;
		const vector<T> *container;

		friend class vector<T>;

	public:
		const_iterator(const T *p = nullptr, const vector<T> *c = nullptr) : ptr(p), container(c) {}
		const_iterator(const iterator &it) : ptr(it.ptr), container(it.container) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, container);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, container);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (container != rhs.container) {
				throw invalid_iterator();
			}
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator temp = *this;
			++ptr;
			return temp;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator temp = *this;
			--ptr;
			return temp;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const T& operator*() const {
			return *ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), currentSize(0), currentCapacity(0) {}
	vector(const vector &other) : data(nullptr), currentSize(0), currentCapacity(0) {
		if (other.data && other.currentCapacity > 0) {
			data = static_cast<T*>(::operator new(other.currentCapacity * sizeof(T)));
			currentCapacity = other.currentCapacity;
			for (size_t i = 0; i < other.currentSize; ++i) {
				new (data + i) T(other.data[i]);
				++currentSize;
			}
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		clear();
		if (data) ::operator delete(data);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return *this;

		clear();
		if (data) ::operator delete(data);

		data = nullptr;
		currentSize = 0;
		currentCapacity = 0;

		if (other.currentCapacity > 0) {
			data = static_cast<T*>(::operator new(other.currentCapacity * sizeof(T)));
			currentCapacity = other.currentCapacity;
			for (size_t i = 0; i < other.currentSize; ++i) {
				new (data + i) T(other.data[i]);
				++currentSize;
			}
		}

		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= currentSize) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= currentSize) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= currentSize) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= currentSize) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	T & front() {
		if (currentSize == 0) {
			throw container_is_empty();
		}
		return data[0];
	}
	const T & front() const {
		if (currentSize == 0) {
			throw container_is_empty();
		}
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	T & back() {
		if (currentSize == 0) {
			throw container_is_empty();
		}
		return data[currentSize - 1];
	}
	const T & back() const {
		if (currentSize == 0) {
			throw container_is_empty();
		}
		return data[currentSize - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data, this);
	}
	const_iterator begin() const {
		return const_iterator(data, this);
	}
	const_iterator cbegin() const {
		return const_iterator(data, this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + currentSize, this);
	}
	const_iterator end() const {
		return const_iterator(data + currentSize, this);
	}
	const_iterator cend() const {
		return const_iterator(data + currentSize, this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return currentSize == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return currentSize;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < currentSize; ++i) {
			data[i].~T();
		}
		currentSize = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.container != this) {
			throw invalid_iterator();
		}

		size_t index = pos - begin();
		if (currentSize + 1 > currentCapacity) {
			resize(currentCapacity == 0 ? 1 : currentCapacity * 2);
		}

		// Move elements to make space
		for (size_t i = currentSize; i > index; --i) {
			new (data + i) T(std::move(data[i - 1]));
			data[i - 1].~T();
		}

		// Insert new element
		new (data + index) T(value);
		++currentSize;

		return iterator(data + index, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > currentSize) {
			throw index_out_of_bound();
		}
		return insert(begin() + ind, value);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		if (pos.container != this || pos.ptr < data || pos.ptr >= data + currentSize) {
			throw invalid_iterator();
		}

		size_t index = pos - begin();
		data[index].~T();

		// Move elements to fill the gap
		for (size_t i = index; i < currentSize - 1; ++i) {
			new (data + i) T(std::move(data[i + 1]));
			data[i + 1].~T();
		}

		--currentSize;
		return iterator(data + index, this);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= currentSize) {
			throw index_out_of_bound();
		}
		return erase(begin() + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (currentSize + 1 > currentCapacity) {
			resize(currentCapacity == 0 ? 1 : currentCapacity * 2);
		}
		new (data + currentSize) T(value);
		++currentSize;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (currentSize == 0) {
			throw container_is_empty();
		}
		data[currentSize - 1].~T();
		--currentSize;
	}
};


}

#endif
