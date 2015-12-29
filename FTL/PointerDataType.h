#pragma once

#include "IsPointer.h"

namespace FTL
{
	template <class T, bool isPointer>
	class PointerDataType;

	template <class T>
	class PointerDataType<T, false>
	{
		PointerDataType()
		{
			static_assert(false, "T should be either pointer or pointer-like object.");
		}
	};

	template <class T>
	class PointerDataType<T, true>
	{
	public:
		typedef T type;
	};
	
	template <class T>
	using PointerType = typename PointerDataType<decltype(*declval<T>()), IsPointer<T>::value>::type;
}