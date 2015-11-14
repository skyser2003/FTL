#include "stdafx.h"
#include "CppUnitTest.h"

#include "Property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FTL;

namespace FTLTest
{
	class TestClass
	{
	public:
		Property<TestClass, int, false, false> defaultProp;
		Property<TestClass, int, true, true> prop1{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, true, false> prop2{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, false, true> prop3{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, false, false> prop4{ [](int val) {return val*val; }, DefaultGetter<int>() };

		void Test()
		{
			prop1 = 1;
			prop2 = 2;
			prop3 = 3;
			prop4 = 4;
		}
	};

	TEST_CLASS(PropertyTest)
	{
	public:
		TEST_METHOD(PropertyTest1)
		{
			FTLTest::TestClass cls;
			cls.Test();

			// Getter
			Assert::AreEqual(static_cast<int>(cls.defaultProp), 0);
			// Assert::AreEqual(static_cast<int>(cls.prop1), 1 * 1); // Compile error
			Assert::AreEqual(static_cast<int>(cls.prop2), 2 * 2);
			// Assert::AreEqual(static_cast<int>(cls.prop3), 3 * 3); // Compile error
			Assert::AreEqual(static_cast<int>(cls.prop4), 4 * 4);

			// Setter
			Assert::AreEqual(static_cast<int>(cls.defaultProp = 1), 1);
			// Assert::AreEqual(static_cast<int>(cls.prop1 = 1), 1 * 1); // Compile error
			// Assert::AreEqual(static_cast<int>(cls.prop2 = 2), 2 * 2); // Compile error
			Assert::AreEqual(static_cast<int>(cls.prop3 = 3), 3 * 3);
			Assert::AreEqual(static_cast<int>(cls.prop4 = 4), 4 * 4);
		}
	};
}