#include "stdafx.h"
#include "CppUnitTest.h"

#include "PointerDataType.h"
#include "HelperFunction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FTL;

namespace FTLTest
{
	TEST_CLASS(PointerTest)
	{
	public:
		TEST_METHOD(PointerTest1)
		{
			int a = 3;
			int* pA = &a;
			PointerDataType<decltype(pA)> t1 = &a;

			Assert::AreEqual(pA, t1);
			Assert::AreEqual(&a, t1);

			Assert::AreEqual(3, a);
			Assert::AreEqual(3, *pA);
			Assert::AreEqual(3, *t1);

			a = 4;

			Assert::AreEqual(4, a);
			Assert::AreEqual(4, *pA);
			Assert::AreEqual(4, *t1);

			*pA = 5;

			Assert::AreEqual(5, a);
			Assert::AreEqual(5, *pA);
			Assert::AreEqual(5, *t1);

			*t1 = 6;

			Assert::AreEqual(6, a);
			Assert::AreEqual(6, *pA);
			Assert::AreEqual(6, *t1);
		}

		TEST_METHOD(PointerTest2)
		{
			int* pA = new int(3);
			unique_ptr<int> upt(pA);

			Assert::AreEqual(3, *upt);

			PointerDataType<decltype(upt)> pt;
			pt.swap(upt);

			Assert::AreEqual(true, std::is_same<decltype(upt), decltype(pt)>::value);
			Assert::AreEqual(true, upt.get() == nullptr);
			Assert::AreEqual(3, *pt);
			Assert::AreEqual(pA, pt.get());

			*pA = 4;

			Assert::AreEqual(4, *pA);
			Assert::AreEqual(4, *pt);

			*pt = 5;

			Assert::AreEqual(5, *pA);
			Assert::AreEqual(5, *pt);
		}
	};
}