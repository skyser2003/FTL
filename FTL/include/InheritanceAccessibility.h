#pragma once

#include <type_traits>

namespace FTL
{
	// Declare
	template <bool value, class CLS>
	class InheritanceAccessiblity;

	// Public
	template <class CLS>
	class InheritanceAccessiblity<true, CLS> : public CLS
	{
	public:
		using CLS::CLS;

		using CLS::operator=;
	};

	// Private
	template <class CLS>
	class InheritanceAccessiblity<false, CLS> : private CLS
	{
	public:
		using CLS::CLS;

	private:
		using CLS::operator=;
	};
}