#pragma once
#include <mutex>
#include <type_traits>
#include <utility>

namespace coup
{

template<typename T>
class threadsafe_vector
{
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    std::mutex mtx;
    
    // assumes caller is holding the lock
    void resize(size_t capacity)
    {
        T* new_data = new T[capacity];
        for (size_t i{}; i < size_; ++i)
        {
            if (std::is_move_constructible_v<T>)
            {
                new(&new_data[i]) T(std::move(data_[i]));
            }
            else
            {
                new(&new_data[i]) T(data_[i]);
            }
        }
        delete(data_);
        data_ = new_data;
        capacity_ = capacity;
    }

public:
    threadsafe_vector()
        : data_(new T[10]),
          size_(0),
          capacity_(10)
    {}

    ~threadsafe_vector()
    {
        std::lock_guard<std::mutex> lock(mtx);
        delete(data_);
    }

    void push_back(T&& value) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (size_ == capacity_)
        {
            resize(capacity_ * 2);
        }
        new(&data_[size_++]) T(std::forward<T>(value));
    }

    void pop_back() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        delete(&data_[size_--]);
    }
    
    T& operator[](size_t index) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data_[index];
    }

    size_t size() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return size_;
    }

    size_t capacity() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return capacity_;
    }


    bool empty() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return size_ == 0;
    }

    void reserve(size_t capacity)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (capacity <= capacity_)
        {
            return;
        }

        T* new_data = new T[capacity];

        for (size_t i{}; i < size_; ++i)
        {
            if (std::is_move_constructible_v<T>)
            {
                new(&new_data[i]) T(std::move(data_[i]));
            }
            else
            {
                new(&new_data[i]) T(data_[i]);
            }
        }

        delete(data_);
        data_ = new_data;
        capacity_ = capacity;
    }
};
} // namespace coup
