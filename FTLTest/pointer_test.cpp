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
		TEST_METHOD(TestMethod1)
		{
			unique_ptr<int> ptpt;
			int a = 3;
			int* pA = &a;
			PointerDataType<int*> t1 = &a;
			int*& ppA = pA;
			*ppA;
			decltype((*ppA)) pppA = *pA;
			Log(*t1);
		}
	};
}