#include "stdafx.h"
#include "CppUnitTest.h"

#include "EnumEnumerator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FTL;

namespace FTLTest
{
	enum class TestEnum
	{
		BEGIN,

		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		TEN,

		END
	};

	static wstring ToString(const TestEnum& val)
	{
		return L"boo";
	}

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			EnumEnumerator<TestEnum, TestEnum::BEGIN, TestEnum::END> enumIt;
			enumIt.Init(false, false);

			vector<TestEnum> vec;
			for (TestEnum val : enumIt)
			{
				vec.push_back(val);
			}

			Assert::AreEqual(vec.size(), static_cast<size_t>(10));
			Assert::AreEqual(vec[0], TestEnum::ONE);
			Assert::AreEqual(vec[1], TestEnum::TWO);
			Assert::AreEqual(vec[2], TestEnum::THREE);
			Assert::AreEqual(vec[3], TestEnum::FOUR);
			Assert::AreEqual(vec[4], TestEnum::FIVE);
			Assert::AreEqual(vec[5], TestEnum::SIX);
			Assert::AreEqual(vec[6], TestEnum::SEVEN);
			Assert::AreEqual(vec[7], TestEnum::EIGHT);
			Assert::AreEqual(vec[8], TestEnum::NINE);
			Assert::AreEqual(vec[9], TestEnum::TEN);
		}

		TEST_METHOD(TestMethod2)
		{
			EnumEnumerator<TestEnum, TestEnum::BEGIN, TestEnum::END> enumIt;
			enumIt.Init(true, true);

			vector<TestEnum> vec;
			for (TestEnum val : enumIt)
			{
				vec.push_back(val);
			}

			Assert::AreEqual(vec.size(), static_cast<size_t>(12));
			Assert::AreEqual(vec[0], TestEnum::BEGIN);
			Assert::AreEqual(vec[1], TestEnum::ONE);
			Assert::AreEqual(vec[2], TestEnum::TWO);
			Assert::AreEqual(vec[3], TestEnum::THREE);
			Assert::AreEqual(vec[4], TestEnum::FOUR);
			Assert::AreEqual(vec[5], TestEnum::FIVE);
			Assert::AreEqual(vec[6], TestEnum::SIX);
			Assert::AreEqual(vec[7], TestEnum::SEVEN);
			Assert::AreEqual(vec[8], TestEnum::EIGHT);
			Assert::AreEqual(vec[9], TestEnum::NINE);
			Assert::AreEqual(vec[10], TestEnum::TEN);
			Assert::AreEqual(vec[11], TestEnum::END);
		}
	};
}