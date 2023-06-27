#pragma once

#include <cstddef>
#include <utility>
#include <string.h>
#include <iostream>


template <typename... T>
class Tuple;

template <>
class Tuple<> {
public:
	Tuple() = default;
	Tuple(const Tuple<>& tp) = default;
	Tuple(Tuple<>&& tp) = default;

	friend std::istream& operator >> (std::istream& in, Tuple<>& tp) {
        (void) tp;
		return in;
	}

	friend std::ostream& operator << (std::ostream& out, const Tuple<>& tp) {
        (void) tp;
		return out;
	}

	Tuple& operator = (const Tuple& other) = default;
	Tuple& operator = (Tuple&& other) = default;

	bool operator == (const Tuple<> other) const {
        (void) other;
		return true;
	}

	std::strong_ordering operator <=> (const Tuple<>& other) const {
        (void) other;
		return std::strong_ordering::equivalent;
	}

	void swap(const Tuple<> other) const {
        (void) other;
    }

	static constexpr size_t size_ = 0;
};

template <typename Head, typename... Tail>
class Tuple<Head, Tail...>: private Tuple<Tail...> {
public:
	Tuple() = default;
    Tuple(const Tuple& tp) = default;
	Tuple(Tuple&& tp) = default;
    
	Tuple(Head&& head, Tail&&... tail): Tuple<Tail...> (std::forward<Tail>(tail)...),
	                                head_(std::forward<Head>(head)) {}

	Tuple(const Head& head, const Tail&... tail): Tuple<Tail...>(tail...),
									head_(head) {}
	
	friend std::istream& operator >> (std::istream& in, Tuple<Head, Tail...>& tp) {
		in >> tp.head_;
		if constexpr (sizeof...(Tail) != 0) {
			return in >> static_cast<Tuple<Tail...>&>(tp);
		} else {
			return in;
		}
	}

	friend std::ostream& operator << (std::ostream& out, const Tuple<Head, Tail...>& tp) {
		out << tp.head_;
		if constexpr (sizeof...(Tail) != 0) {
			return out << " " << static_cast<const Tuple<Tail...>&>(tp);
		} else {
			return out;
		}
	}

	Tuple& operator = (const Tuple& other) = default;
	Tuple& operator = (Tuple&& other) = default;

	bool operator == (const Tuple<Head, Tail...>& other) const {
		if (head_ == other.head_) {
			return static_cast<const Tuple<Tail...>&>(*this) == 
				   static_cast<const Tuple<Tail...>&>(other);
		}
		return false;
	}

	std::strong_ordering operator <=> (const Tuple<Head, Tail...>& other) const {
		if (head_ < other.head_) {
			return std::strong_ordering::less;
		}
		if (head_ > other.head_) {
			return std::strong_ordering::greater;
		}
		return static_cast<const Tuple<Tail...>&> (*this) <=> 
			   static_cast<const Tuple<Tail...>&> (other);
	}

	template <size_t index>
	friend auto& get(Tuple<Head, Tail...>& tp) {
		if constexpr (index != 0) {
			return get<index - 1>(static_cast<Tuple<Tail...>&> (tp));
		} else {
			return tp.head_;
		}
	}

	template <size_t index>
	friend auto&& get (Tuple<Head, Tail...>&& tp) {
		if constexpr (index != 0) {
			return get<index - 1>(static_cast<Tuple<Tail...>&&>(std::forward<Tuple<Head, Tail...>>(tp)));
		} else {
			return std::forward<Head>(tp.head_);
		}
	}

	template <size_t index>
	friend const auto& get (const Tuple<Head, Tail...>& tp) {
		if constexpr (index != 0) {
			return get<index - 1>(static_cast<const Tuple<Tail...>&>(tp));
		} else {
			return tp.head_;
		}
	}

	template <typename T>
	friend T& get (Tuple<Head, Tail...>& tp) {
		if constexpr (std::is_same_v<T, Head>) {
			return tp.head_;
		} else {
			return get<T>(static_cast<Tuple<Tail...>&>(tp));
		}
	}
	
	template <typename T>
	friend const T& get (const Tuple<Head, Tail...>& tp) {
		if constexpr (std::is_same_v<T, Head>) {
			return tp.head_;
		} else {
			return get<T>(static_cast<const Tuple<Tail...>&>(tp));
		}
	}

	void swap(Tuple<Head, Tail...>& other) {
		Tuple<Head, Tail...> temp_tp = std::move(other);
		other = std::move(*this);
		*this = std::move(temp_tp);
	}

	static constexpr size_t size_ = Tuple<Tail...>::size_ + 1;
private:
	Head head_;
};

template <typename T>
constexpr size_t tuple_size_v = T::size_;

namespace std {
	template<typename... Args>
	struct tuple_size<Tuple<Args...>>:
			std::integral_constant<size_t, Tuple<Args...>::size_> {};

	template<size_t index, typename... Args>
	struct tuple_element<index, Tuple<Args...>> {
		using type = std::remove_cvref_t<decltype(
					get<index>(std::declval<Tuple<Args...>>()))>;
	};
}


template <typename... Args>
Tuple<Args...> make_tuple(Args&&... args) {
	return Tuple<Args...>(std::forward<Args>(args)...);
}

template <size_t... indexes>
struct Helper {
	template <typename FirstTuple, typename... Args>
	static auto concat_helper(const FirstTuple& f_tp, const Args&... args) {
		if constexpr (FirstTuple::size_ == sizeof...(indexes)) {
			return Tuple<std::remove_cvref_t<decltype(get<indexes>(std::declval<FirstTuple>()))>..., 
						Args...> (get<indexes>(f_tp)..., args...);
		} else {
			return Helper<indexes..., sizeof... (indexes)>::
						concat_helper(f_tp, args...);
		}
	}
};

template <typename FirstTuple, typename SecondTuple, size_t... indexes>
auto tuple_concat(const FirstTuple& f_tp, const SecondTuple& s_tp) {
	if constexpr (SecondTuple::size_ == sizeof... (indexes)) {
		return Helper<>::concat_helper(f_tp, get<indexes>(s_tp)...);
	} else {
		return tuple_concat<FirstTuple, SecondTuple, 
					indexes..., sizeof... (indexes)>(f_tp, s_tp);
	}
}

template <typename FirstTuple, typename... Tuples>
auto tuple_cat(const FirstTuple& f_tp, const Tuples&... other_tps) {
	if constexpr (sizeof... (Tuples) == 0) {
		return f_tp;
	} else {
		return tuple_concat(f_tp, tuple_cat(other_tps...));
	}
}
