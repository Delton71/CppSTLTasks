#pragma once

#include <exception>


class Any {
public:
	constexpr Any() = default;

	template <class T>
	Any(const T& val): 
            object_(new certain_object <T> (val)) {}

	Any(const Any& any) {
		delete object_;
		object_ = any.object_->copy();
	}

	Any(Any&& any) noexcept: object_(any.object_) {
		any.object_ = nullptr;
	}

	template <class T>
	Any& operator = (const T& val) {
		Any new_any(val);
		Swap(new_any);
		return *this;
	}
	
	Any& operator = (const Any& val) {
		Any new_any(val);
		Swap(new_any);
		return *this;
	}

	Any& operator = (Any&& any) noexcept {
		Any new_any(std::forward <Any> (any));
		Swap(new_any);
		return *this;
	}
	
	void Reset() {
		if (object_) {
			delete object_;
        }
		object_ = nullptr;
	}

	void Swap (Any& any) noexcept {
		std::swap(object_, any.object_);
	}

	bool HasValue() const noexcept {
		return (object_ != nullptr);
	}

	~Any() {
		if (object_) {
			delete object_;
        }
	}

private:
	template <typename T, typename... Args>
	friend Any&& MakeAny (const Args&&... args);

	template <class T>
	friend T AnyCast (const Any& any);

	struct any_object {
		virtual any_object* copy() const = 0;
		virtual ~any_object() = default;
	};

	template <typename T>
	struct certain_object: any_object {
		T val;

		certain_object(const T& val): val(val) {}

		any_object* copy() const override {
			return new certain_object <T> (val);
		}
	};

	any_object* object_ = nullptr;
};

template <typename T, typename... Args>
Any&& MakeAny(const Args&&... args) {
	Any any;
	any.object_ = new Any::certain_object <T> (std::forward(args)...);
	return std::forward <Any> (any);
}

class bad_any_cast: public std::bad_cast {
public:
	const char* what() const throw() override {
		return "Bad any cast.";
	}
};

template <class T>
T AnyCast (const Any& any) {
	if (!any.object_) {
		throw bad_any_cast();
    }

	try {
		return dynamic_cast <Any::certain_object <T>&> (*any.object_).val;
	}
	catch (const std::bad_cast& exc) {
		throw bad_any_cast();
	}
}
