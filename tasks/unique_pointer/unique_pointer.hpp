#pragma once


#include <utility>
template <typename T>
class UniquePtr {
public:
    UniquePtr() noexcept = default;

    UniquePtr(T* ptr) noexcept: ptr_(ptr) {}

    UniquePtr(const UniquePtr<T>& uptr) = delete;
    T& operator = (const UniquePtr<T>& uptr) = delete;

    UniquePtr(UniquePtr<T>&& uptr) noexcept: ptr_(uptr.ptr_) {
        uptr.ptr_ = nullptr;
    }

    void operator = (UniquePtr<T>&& uptr) noexcept {
        if (ptr_ != uptr.ptr_) {
            delete ptr_;
            ptr_ = uptr.ptr_;
        }

        uptr.ptr_ = nullptr;
    }

    T* Release() noexcept {
        T* release_ptr = ptr_;
        ptr_ = nullptr;
        return release_ptr;
    }

    void Reset(T* ptr = nullptr) noexcept {
        delete ptr_;
        ptr_ = ptr;
    }

    constexpr void Swap(UniquePtr<T>& uptr) noexcept {
        std::swap(ptr_, uptr.ptr_);
    }

    constexpr T* Get() const noexcept {
        return ptr_;
    }

    constexpr T& operator * () const {
        return *ptr_;
    }

    constexpr T* operator -> () const noexcept {
        return ptr_;
    }

    constexpr operator bool () const noexcept {
        return (ptr_ != nullptr);
    }

    template <typename U>
    constexpr operator UniquePtr<U> () const {
        return UniquePtr<U>(ptr_);
    }

    ~UniquePtr() {
        delete ptr_;
    }

private:
	T* ptr_ = nullptr;
};

template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
	T* ptr = new T(std::forward <Args>(args)...);
	return UniquePtr(ptr);
}
