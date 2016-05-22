#pragma once

#include "IsPointer.h"

namespace FTL
{
	template <class T, bool isPointer>
	class _PointerDataType;

	template <class T>
	class _PointerDataType<T, false>
	{
	public:
		_PointerDataType()
		{
			static_assert(false, "T should be either pointer or pointer-like object.");
		}
	};

	template <class T>
	class _PointerDataType<T, true>
	{
	public:
		typedef T type;
	};

	template <class T>
	using PointerDataType = typename _PointerDataType<T, IsPointer<T>::value>::type;
}