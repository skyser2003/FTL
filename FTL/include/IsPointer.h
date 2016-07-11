#pragma once

#include <type_traits>

namespace FTL
{
	template <class, class = void>
	struct IsPointer : public std::false_type {};

	template <class T>
	struct IsPointer<T, std::void_t<decltype(*std::declval<T>())>> : public std::true_type{};
}