#pragma once
#include <iostream>
#include <cstddef>


class String {
public:
    String();
    ~String();
    String(size_t size, char symbol);
    String(const char* arr, size_t size);
    String(const char* str);
    String(const String& str);

    String& operator = (const String& str);

    char& operator [] (size_t i);
    const char& operator [] (size_t i) const;

    char& Front();
    const char& Front() const;
    char& Back();
    const char& Back() const;

    const char* CStr() const noexcept;
    const char* Data() const noexcept;

    bool Empty() const noexcept;

    size_t Size() const noexcept;
    size_t Length() const noexcept;
    size_t Capacity() const noexcept;
    
    void Clear() noexcept;

    void Swap(String& other) noexcept;

    void PopBack() noexcept;
    void PushBack(char symbol);

    String& operator += (const String& str);

    bool Resize(size_t new_size, char symbol) noexcept;
    bool Reserve(size_t new_capacity) noexcept;
    bool ShrinkToFit() noexcept;

private:
	size_t size_     = 0;
	size_t capacity_ = 0;
	char*  data_     = nullptr;
};


String operator + (const String& str1, const String& str2);
		
bool operator == (const String& str1, const String& str2) noexcept;

const std::strong_ordering& operator <=>
            (const String& str1, const String& str2) noexcept;

std::istream& operator >> (std::istream& input, String& str);
std::ostream& operator << (std::ostream& output, const String& str) noexcept;
