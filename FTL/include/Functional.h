#pragma once

#include <functional>
#include <type_traits>

namespace FTL
{
	template <typename Container, typename Lambda>
	auto TransformToVector(const Container& container, Lambda func)
	{
		using InputType = typename Container::value_type;
		using RetType = typename std::invoke_result<Lambda, InputType>::type;

		std::vector<RetType> ret;

		for (const InputType& val : container)
		{
			ret.push_back(func(val));
		}

		return ret;
	}
}