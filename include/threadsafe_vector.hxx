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
    void resize(size_t new_capacity)
    {
        T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
        for (size_t i{}; i < size_; ++i)
        {
            if (std::is_nothrow_move_constructible_v<T>)
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
        capacity_ = new_capacity;
    }

public:
    threadsafe_vector()
        : data_(static_cast<T*>(operator new[](10 * sizeof(T)))),
          size_(0),
          capacity_(10)
    {}

    ~threadsafe_vector()
    {
        std::lock_guard<std::mutex> lock(mtx);
        operator delete[](data_);
    }

    void push_back(const T& value) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (size_ == capacity_)
        {
            resize(capacity_ * 2);
        }
        new(&data_[size_++]) T(value);
    }
    
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (size_ == capacity_)
        {
            resize(capacity_ * 2);
        }
        new(&data_[size_++]) T(std::forward<Args>(args)...);
    }

    void pop_back() noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (size_ > 0)
        {
            data_[--size_].~T();
        }
    }
    
    T& operator[](size_t index) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data_[index];
    }

    T& front() noexcept
    {
        return data_[0];
    }

    T& back() noexcept
    {
        return data_[size_ - 1];
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

    void reserve(size_t new_capacity) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (new_capacity <= capacity_)
        {
            return;
        }

        T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

        for (size_t i{}; i < size_; ++i)
        {
            if (std::is_nothrow_move_constructible_v<T>)
            {
                new(&new_data[i]) T(std::move(data_[i]));
            }
            else
            {
                new(&new_data[i]) T(data_[i]);
            }
        }

        operator delete[](data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};
} // namespace coup
