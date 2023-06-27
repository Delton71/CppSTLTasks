#pragma once
#include <cstddef>
#include <memory>


template <typename T>
class SharedPtr;
template <typename T>
class WeakPtr;


template <typename T>
class SharedPtr {
public:
	SharedPtr() noexcept = default;
    SharedPtr(std::nullptr_t) noexcept: SharedPtr() {}

	SharedPtr(T* ptr) {
		if (ptr) {
			block_ = new PtrCounts(ptr, 1, 0);
        } else {
			block_ = nullptr;
        }
	}

	SharedPtr(const SharedPtr<T>& sptr) noexcept: SharedPtr(sptr.block_) {}

	SharedPtr& operator = (const SharedPtr<T>& sptr) noexcept {
		if (block_ == sptr.block_) {
			return *this;
        }

        SharedPtr<T> temp_ptr(sptr.block_);
        Swap(temp_ptr);
		return *this;
	}

	SharedPtr(const WeakPtr<T>& wptr) {
		if (wptr.Expired()) {
			throw std::bad_weak_ptr();
        }

        SharedPtr<T> temp_ptr(wptr.block_);
        Swap(temp_ptr);
	}

	SharedPtr(SharedPtr&& sptr) noexcept: block_(sptr.block_) {
		sptr.block_ = nullptr;
	}

	SharedPtr& operator = (SharedPtr&& sptr) noexcept {
		if (sptr.block_ == block_) {
			sptr.block_ = nullptr;
			return *this;
		}

		SharedPtr<T> temp_ptr(std::move(sptr));
        Swap(temp_ptr);
		return *this;
	}

	void Reset(T* ptr = nullptr) {
        SharedPtr<T> temp_ptr(ptr);
        Swap(temp_ptr);
	}

	constexpr void Swap(SharedPtr<T>& sptr) noexcept {
		std::swap(block_, sptr.block_);
	}

	constexpr T* Get() const noexcept {
		return (block_)? block_->ptr : nullptr;
	}

	constexpr std::size_t UseCount() const noexcept {
		return (block_)? block_->strong_ptr_count : 0;
	}

	constexpr T& operator * () const {
		return *block_->ptr;
	}

	constexpr T* operator -> () const {
		return block_->ptr;
	}

	constexpr operator bool () const noexcept {
		return (block_ != nullptr);
	}

	template <typename U>
	operator SharedPtr<U> () const noexcept {
		SharedPtr<U> sptr;
		sptr.block_ = block_;
		return sptr;
	}

	~SharedPtr() noexcept {
		if (block_) {
			if (block_->strong_ptr_count == 1) {
				delete block_->ptr;
				block_->ptr = nullptr;
				block_->strong_ptr_count = 0;

				if (!block_->weak_ptr_count) {
					delete block_;
                }
			} else {
				--block_->strong_ptr_count;
            }
		}
	}

private:
    friend WeakPtr<T>;

	struct PtrCounts {
		T* ptr = nullptr;
		std::size_t strong_ptr_count = 0;
		std::size_t weak_ptr_count   = 0;

        PtrCounts(T *ptr, std::size_t s_cnt, std::size_t w_cnt):
                    ptr(ptr), strong_ptr_count(s_cnt), weak_ptr_count(w_cnt) {}
	};

	PtrCounts* block_ = nullptr;

    SharedPtr(PtrCounts* c_block) noexcept: block_(c_block) {
        if (c_block) {
            ++c_block->strong_ptr_count;
        }
    }
};


template <typename T>
class WeakPtr {
public:
	WeakPtr() noexcept = default;
    WeakPtr(std::nullptr_t) noexcept: WeakPtr() {}

	WeakPtr(const WeakPtr<T>& wptr) noexcept: WeakPtr(wptr.block_) {}

	WeakPtr& operator = (const WeakPtr<T>& wptr) noexcept {
		if (block_ == wptr.block_) {
			return *this;
        }

		WeakPtr<T> temp_ptr(wptr);
        Swap(temp_ptr);
		return *this;
	}

	WeakPtr(WeakPtr&& wptr) noexcept: block_(wptr.block_) {
		wptr.block_ = nullptr;
	}

	WeakPtr& operator = (WeakPtr&& wptr) noexcept {
		if (wptr.block_ == block_) {
			wptr.block_ = nullptr;
			return *this;
		}

        WeakPtr<T> temp_ptr(std::move(wptr));
        Swap(temp_ptr);
		return *this;
	}

	WeakPtr(const SharedPtr<T>& sptr): WeakPtr(sptr.block_) {}

	constexpr void Swap(WeakPtr<T>& wptr) noexcept  {
		std::swap(block_, wptr.block_);
	}

	void Reset() noexcept {
		this->~WeakPtr<T>();
		block_ = nullptr;
	}

	constexpr std::size_t UseCount() const noexcept {
		return (block_)? block_->strong_ptr_count : 0;
	}

	constexpr bool Expired() const noexcept {
        if (block_) {
            return block_->strong_ptr_count == 0;
        }
        return true;
	}

	SharedPtr<T> Lock() const noexcept {
		SharedPtr<T> sptr;
		if (block_) {
			if (block_->strong_ptr_count == 0) {
				return sptr;
            }

			++block_->strong_ptr_count;
			sptr.block_ = block_;
		}
		
		return sptr;
	}

	template <typename U>
	operator WeakPtr<U> () const noexcept {
		WeakPtr<U> wptr;
		wptr.block_ = block_;
		return wptr;
	}

	~WeakPtr() noexcept {
		if (block_) {
			if (block_->weak_ptr_count <= 1) {
				block_->weak_ptr_count = 0;
				if (!block_->strong_ptr_count) {
					delete block_;
                }
			} else {
				--block_->weak_ptr_count;
			}
		}
	}

private:
    friend SharedPtr<T>;

	typename SharedPtr<T>::PtrCounts* block_ = nullptr;

    WeakPtr(typename SharedPtr<T>::PtrCounts* c_block) noexcept: block_(c_block) {
        if (c_block) {
            ++c_block->weak_ptr_count;
        }
    }
};


template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
	T* ptr = new T (std::forward<Args>(args)...);
	return SharedPtr(ptr);
}
