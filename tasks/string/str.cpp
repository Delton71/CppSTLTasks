#include "str.hpp"
#include <cstddef>
#include <cstring>
#include <new>
#include <stdexcept>


String::String(): size_(0), capacity_(0), data_(nullptr) {}

String::~String() {
	delete[] data_;
}

String::String(size_t size, char symbol): 
            size_(size), capacity_(size), data_(new char[size + 1]) {
	memset(data_, symbol, size * sizeof(symbol));
	data_[size] = '\0';
}

String::String(const char* arr, size_t size): 
            size_(size), capacity_(size), data_(new char[size + 1]) {
	if (arr) {
		memcpy(data_, arr, size * sizeof(*arr));
		data_[size] = '\0';
	}
}

String::String(const char* str): 
            size_(strlen(str)), capacity_(size_),
            data_(new char[capacity_ + 1]) {
	if (str) {
		memcpy(data_, str, (size_ + 1) * sizeof(*data_));
	}
}

String::String(const String& str): 
            size_(str.size_), capacity_(str.capacity_),
            data_(new char[capacity_ + 1]) {
	if (str.data_) {
		memcpy(data_, str.data_, (size_ + 1) * sizeof(*str.data_));
	}
}

String& String::operator = (const String& str) {
	size_ = str.size_;
	capacity_ = str.capacity_;
	if (data_ == str.data_) {
		return *this;
    }

	char* new_data = new char[capacity_ + 1];
	if (str.data_) {
		memcpy(new_data, str.data_, (capacity_ + 1) * sizeof(*str.data_));
	}
	delete[] data_;
	data_ = new_data;
	return *this;
}

char& String::operator [] (size_t i) {
	return data_[i];
}


const char& String::operator [] (size_t i) const {
	return data_[i];
}

char& String::Front() {
	if (!size_) {
		throw std::out_of_range("Front() error: empty.");
    }
	
    return data_[0];
}

const char& String::Front() const {
	if (!size_) {
		throw std::out_of_range("Front() error: empty.");
    }
	
    return data_[0];
}

char& String::Back() {
	if (!size_) {
		throw std::out_of_range("Back() error: empty.");
    }
	
    return data_[size_ - 1];
}

const char& String::Back() const {
	if (!size_) {
		throw std::out_of_range("Back() error: empty.");
    }
	
    return data_[size_ - 1];
}

const char* String::CStr() const noexcept {
	return (size_)? data_ : nullptr;
}

const char* String::Data() const noexcept {
	return (size_)? data_ : nullptr;
}

bool String::Empty() const noexcept {
	return (size_ == 0);
}

size_t String::Size() const noexcept {
	return size_;
}

size_t String::Length() const noexcept {
	return size_;
}

size_t String::Capacity() const noexcept {
	return capacity_;
}

void String::Clear() noexcept {
	if (size_) {
		data_[0] = '\0';
		size_    = 0;
	}
}

void String::Swap(String& other) noexcept {
	std::swap(size_, other.size_);
	std::swap(capacity_, other.capacity_);
	std::swap(data_, other.data_);
}

void String::PopBack() noexcept {
	if (size_) {
		data_[size_] = '\0';
        --size_;
	}
}

void String::PushBack(char symbol) {
    if (size_ == capacity_) {
        size_t new_capacity = (capacity_)? (capacity_ << 1) : 1;
        if (!Reserve(new_capacity)) {
            throw std::bad_alloc();
        }
    }

	data_[size_++] = symbol;
	data_[size_]   = '\0';
}

String& String::operator += (const String& str) {
	size_t new_size = size_ + str.size_;
	if (new_size > capacity_ && !Reserve(new_size)) {
		throw std::bad_alloc();
    }

	size_ = new_size;
	strncat(data_, str.data_, new_size);
	return *this;
}

bool String::Resize (size_t new_size, char symbol) noexcept {
	if (size_ == new_size) {
		return true;
    }

	if (size_ > new_size) {
		size_ = new_size;
		data_[size_] = '\0';
	}

	if (!Reserve(new_size)) {
		return false;
    }

	memset(data_ + size_, symbol, (new_size - size_) * sizeof(*data_));
	size_ = new_size;
	data_[size_] = '\0';

	return true;
}

bool String::Reserve(size_t new_capacity) noexcept {
	if (new_capacity > capacity_) {
		char* new_data = new (std::nothrow) char[new_capacity + 1];
		
        if (!new_data) {
			return false;
        }

		if (data_) {
			memcpy(new_data, data_, (capacity_ + 1) * sizeof(*data_));
		}
		delete[] data_;
		data_     = new_data;
		capacity_ = new_capacity;
	}

	return true;
}

bool String::ShrinkToFit () noexcept {
	if (size_ == capacity_) {
		return true;
    }

	char* new_data = new (std::nothrow) char[size_ + 1];
	
    if (!new_data) {
		return false;
    }

	memcpy(new_data, data_, (size_ + 1) * sizeof(*data_));
	delete[] data_;
	data_     = new_data;
	capacity_ = size_;
	
    return true;
}

String operator + (const String& str1, const String& str2) {
	String new_str = str1;
	new_str += str2;
	return new_str;
}

bool operator == (const String& str1, const String& str2) noexcept {
	if (str1.Size() != str2.Size()) {
		return false;
    }

	if (!str1.Size()) {
		return true;
    }

	const char* s1 = str1.CStr();
	const char* s2 = str2.CStr();

	for (size_t i = 0; i < str1.Size(); ++i) {
		if (s1[i] != s2[i]) {
			return false;
        }
	}

	return true;
}

const std::strong_ordering& operator <=>
            (const String& str1, const String& str2) noexcept {
	size_t size1 = str1.Size();
	size_t size2 = str2.Size();
	size_t min_size = (size1 < size2) ? size1 : size2;

	if (!min_size) {
		if (size1 < size2) {
			return std::strong_ordering::less;
		}
		if (size1 > size2) {
			return std::strong_ordering::greater;
		}
		return std::strong_ordering::equivalent;
	}

	const char* s1 = str1.CStr();
	const char* s2 = str2.CStr();

	for (size_t i = 0; i < min_size; ++i) {
		if (s1[i] != s2[i]) {
			if (s1[i] < s2[i]) {
				return std::strong_ordering::less;
            } else {
				return std::strong_ordering::greater;
            }
		}
	}

	if (size1 < size2) {
		return std::strong_ordering::less;
	}
	if (size1 > size2) {
		return std::strong_ordering::greater;
	}
	return std::strong_ordering::equivalent;
}

std::istream& operator >> (std::istream& input, String& str) {
	str.Clear();
	char buff[2] = "";
	
	input.get(*buff);
	while (!(input.eof() || isspace(*buff))) {
		str += buff;
		input.get(*buff);
	}

	return input;
}

std::ostream& operator << (std::ostream& output, const String& str) noexcept {
	return (str.Size())? (output << str.CStr()) : output;
}
