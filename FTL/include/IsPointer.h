#pragma once

#include <type_traits>

#include "VoidTemplate.h"

namespace FTL
{
	template <class, class = void>
	struct IsPointer : public std::false_type {};

	template <class T>
	struct IsPointer<T, VoidTemplate<decltype(*declval<T>())>> : public std::true_type{};
}