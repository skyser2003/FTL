#pragma once

#include <type_traits>

namespace FTL
{
	// Declare
	template <class... Args>
	class InheritanceAccessiblity;

	// Public
	template <class CLS, class... Args>
	class InheritanceAccessiblity<std::true_type, CLS, Args...> : public CLS, public InheritanceAccessiblity<Args...> {};

	// Private
	template <class CLS, class... Args>
	class InheritanceAccessiblity<std::false_type, CLS, Args...> : private CLS, public InheritanceAccessiblity<Args...> {};

	// End
	template <>
	class InheritanceAccessiblity<> {};
}