#pragma once

template <typename... T>
class Tuple;

template <>
class Tuple<> {
public:
	Tuple() = default;
};

template <typename Head, typename... Tail>
class Tuple<Head, Tail...>: private Tuple<Tail...> {
public:
	Tuple() = default;
private:
	Head head_;
};
