#pragma once
#include <stdexcept>
#include <iostream>

namespace STDev
{
	template<typename T>
	class vector
	{
	private:
		T* vectorData_;
		size_t capacity_;
		size_t size_;

		void resize_internal()
		{
			capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
			T* newVectorData_ = new T[capacity_];
			for (size_t i = 0; i < size_; ++i)
			{
				newVectorData_[i] = vectorData_[i];
			}
			delete[] vectorData_;
			vectorData_ = newVectorData_;
		}

	public:
		// Costruttore di default
		vector() : vectorData_{ nullptr }, capacity_{ 0 }, size_{ 0 }
		{}

		// Copy constructor
		vector(const vector& other)
			: vectorData_(nullptr), capacity_(other.capacity_), size_(other.size_)
		{
			if (capacity_ > 0)
			{
				vectorData_ = new T[capacity_];
				for (size_t i = 0; i < size_; ++i)
				{
					vectorData_[i] = other.vectorData_[i];
				}
			}
		}

		// Copy assignment
		vector& operator=(const vector& other)
		{
			if (this != &other)
			{
				delete[] vectorData_;

				capacity_ = other.capacity_;
				size_ = other.size_;

				if (capacity_ > 0)
				{
					vectorData_ = new T[capacity_];
					for (size_t i = 0; i < size_; ++i)
					{
						vectorData_[i] = other.vectorData_[i];
					}
				}
				else
				{
					vectorData_ = nullptr;
				}
			}
			return *this;
		}

		// Move constructor
		vector(vector&& other) noexcept
			: vectorData_(other.vectorData_), capacity_(other.capacity_), size_(other.size_)
		{
			other.vectorData_ = nullptr;
			other.capacity_ = 0;
			other.size_ = 0;
		}

		// Move assignment
		vector& operator=(vector&& other) noexcept
		{
			if (this != &other)
			{
				delete[] vectorData_;

				vectorData_ = other.vectorData_;
				capacity_ = other.capacity_;
				size_ = other.size_;

				other.vectorData_ = nullptr;
				other.capacity_ = 0;
				other.size_ = 0;
			}
			return *this;
		}

		// Distruttore
		~vector()
		{
			delete[] vectorData_;
		}

		// Push/Pop operations
		void push_back(const T& t)
		{
			if (size_ >= capacity_)
			{
				resize_internal();
			}
			vectorData_[size_++] = t;
		}

		void pop_back()
		{
			if (size_ == 0)
			{
				throw std::out_of_range("pop_back on empty vector");
			}
			size_--;
		}

		// Insert operation (fixed)
		void insert(const T& t, size_t insertPosition)
		{
			if (insertPosition > size_)
			{
				throw std::out_of_range("Insert position out of range");
			}

			if (size_ >= capacity_)
			{
				resize_internal();
			}

			// Sposta elementi a destra
			for (size_t i = size_; i > insertPosition; --i)
			{
				vectorData_[i] = vectorData_[i - 1];
			}

			vectorData_[insertPosition] = t;
			size_++;
		}

		// Erase operation
		void erase(size_t position)
		{
			if (position >= size_)
			{
				throw std::out_of_range("Erase position out of range");
			}

			// Sposta elementi a sinistra
			for (size_t i = position; i < size_ - 1; ++i)
			{
				vectorData_[i] = vectorData_[i + 1];
			}
			size_--;
		}

		// Access operations
		T& operator[](size_t i)
		{
			return vectorData_[i];
		}

		const T& operator[](size_t i) const
		{
			return vectorData_[i];
		}

		T& at(size_t i)
		{
			if (i >= size_)
			{
				throw std::out_of_range("Index out of range");
			}
			return vectorData_[i];
		}

		const T& at(size_t i) const
		{
			if (i >= size_)
			{
				throw std::out_of_range("Index out of range");
			}
			return vectorData_[i];
		}

		T& front()
		{
			if (size_ == 0)
			{
				throw std::out_of_range("front on empty vector");
			}
			return vectorData_[0];
		}

		const T& front() const
		{
			if (size_ == 0)
			{
				throw std::out_of_range("front on empty vector");
			}
			return vectorData_[0];
		}

		T& back()
		{
			if (size_ == 0)
			{
				throw std::out_of_range("back on empty vector");
			}
			return vectorData_[size_ - 1];
		}

		const T& back() const
		{
			if (size_ == 0)
			{
				throw std::out_of_range("back on empty vector");
			}
			return vectorData_[size_ - 1];
		}

		// Capacity operations
		size_t size() const
		{
			return size_;
		}

		size_t capacity() const
		{
			return capacity_;
		}

		bool empty() const
		{
			return size_ == 0;
		}

		void clear()
		{
			size_ = 0;
		}

		void reserve(size_t new_capacity)
		{
			if (new_capacity > capacity_)
			{
				T* newVectorData_ = new T[new_capacity];
				for (size_t i = 0; i < size_; ++i)
				{
					newVectorData_[i] = vectorData_[i];
				}
				delete[] vectorData_;
				vectorData_ = newVectorData_;
				capacity_ = new_capacity;
			}
		}

		void shrink_to_fit()
		{
			if (size_ < capacity_)
			{
				T* newVectorData_ = new T[size_];
				for (size_t i = 0; i < size_; ++i)
				{
					newVectorData_[i] = vectorData_[i];
				}
				delete[] vectorData_;
				vectorData_ = newVectorData_;
				capacity_ = size_;
			}
		}

		// Utility
		void print() const
		{
			std::cout << "[";
			for (size_t i = 0; i < size_; ++i)
			{
				std::cout << vectorData_[i];
				if (i < size_ - 1) std::cout << ", ";
			}
			std::cout << "]" << std::endl;
		}

		T* data()
		{
			return vectorData_;
		}

		const T* data() const
		{
			return vectorData_;
		}
	};
}