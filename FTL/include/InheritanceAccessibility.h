#pragma once

namespace FTL
{
	// Declare
	template <class... Args>
	class InheritanceAccessiblity;

	template <class... Args, class ab>
	class InheritanceAccessiblity<ab, Args...>
	{
	public:
	};
}