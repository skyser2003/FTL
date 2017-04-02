#include "stdafx.h"
#include "CppUnitTest.h"

#include "InheritanceAccessibility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FTL;

namespace FTLTest
{
	class Pub
	{
	public:
		constexpr bool IsPublic() const
		{
			return true;
		}
	};

	class Pri
	{
	public:
		constexpr bool IsPrivate() const
		{
			return true;
		}
	};

	template <class T, class = void>
	class CheckPublicCls : public false_type {};

	template <class T>
	class CheckPublicCls<T, void_t<decltype(declval<T>().IsPublic())>> : public true_type {};

	template <class T, class = void>
	class CheckPrivateCls : public false_type {};

	template <class T>
	class CheckPrivateCls<T, void_t<decltype(declval<T>().IsPrivate())>> : public true_type {};

	template <class T>
	constexpr bool CheckPublic(const T&)
	{
		return typename CheckPublicCls<T>::value;
	}

	template <class T>
	constexpr bool CheckPrivate(const T&)
	{
		return typename CheckPrivateCls<T>::value;
	}

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(AccessibilityTest1)
		{
			InheritanceAccessiblity<false_type, Pri, true_type, Pub> p1;
			InheritanceAccessiblity<true_type, Pub, false_type, Pri> p2;

			Assert::AreEqual(true, CheckPublic(p1));
			Assert::AreEqual(false, CheckPrivate(p1));

			Assert::AreEqual(true, CheckPublic(p2));
			Assert::AreEqual(false, CheckPrivate(p2));
		}
	};
}