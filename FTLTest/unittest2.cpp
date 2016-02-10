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
		// Value
		Property<TestClass, int, false, false> defaultProp;

		Property<TestClass, int, true, true> prop1{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, true, false> prop2{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, false, true> prop3{ [](int val) {return val*val; }, DefaultGetter<int>() };
		Property<TestClass, int, false, false> prop4{ [](int val) {return val*val; }, DefaultGetter<int>() };

		int pD, p1, p2, p3, p4;

		Property<TestClass, int*, false, false> pDefaultProp{ &pD };

		Property<TestClass, int*, true, true> pProp1{ [](int* val) {return val; }, DefaultGetter<int*>(), &p1 };
		Property<TestClass, int*, true, false> pProp2{ [](int* val) {return val; }, DefaultGetter<int*>(), &p2 };
		Property<TestClass, int*, false, true> pProp3{ [](int* val) {return val; }, DefaultGetter<int*>(), &p3 };
		Property<TestClass, int*, false, false> pProp4{ [](int* val) {return val; }, DefaultGetter<int*>(), &p4 };

		// Pointer
		Property<TestClass, int*, false, false> ptrProp;

		// Smart Pointer
		Property<TestClass, shared_ptr<int>, false, false> smptrProp;

		void Test()
		{
			prop1 = 1;
			prop2 = 2;
			prop3 = 3;
			prop4 = 4;

			pDefaultProp = &pD;
			pProp1 = &p1;
			pProp2 = &p2;
			pProp3 = &p3;
			pProp4 = &p4;

			*pDefaultProp = 0;
			*pProp1 = 1;
			*pProp2 = 2;
			*pProp3 = 3;
			*pProp4 = 4;
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

			Assert::AreEqual(static_cast<int>(*cls.pDefaultProp), 0);
			// Assert::AreEqual(static_cast<int>(*cls.pProp1), 1); // Compile error
			Assert::AreEqual(static_cast<int>(*cls.pProp2), 2);
			// Assert::AreEqual(static_cast<int>(*cls.pProp3), 3); // Compile error
			Assert::AreEqual(static_cast<int>(*cls.pProp4), 4);

			// Setter
			int dummyInt = 0;

			Assert::AreEqual(static_cast<int>(cls.defaultProp = 1), 1);
			// Assert::AreEqual(static_cast<int>(cls.prop1 = 1), 1 * 1); // Compile error
			// Assert::AreEqual(static_cast<int>(cls.prop2 = 2), 2 * 2); // Compile error
			Assert::AreEqual(static_cast<int>(cls.prop3 = 3), 3 * 3);
			Assert::AreEqual(static_cast<int>(cls.prop4 = 4), 4 * 4);

			Assert::AreEqual(static_cast<int*>(cls.pDefaultProp = &dummyInt), &dummyInt);
			// Assert::AreEqual(static_cast<int*>(cls.pProp1 = &dummyInt), &dummyInt); // Compile error
			// Assert::AreEqual(static_cast<int*>(cls.pProp2 = &dummyInt), &dummyInt); // Compile error
			Assert::AreEqual(static_cast<int*>(cls.pProp3 = &dummyInt), &dummyInt);
			Assert::AreEqual(static_cast<int*>(cls.pProp4 = &dummyInt), &dummyInt);

			// Pointer dereference(getter)
			cls.pD = 5;
			cls.p1 = 6;
			cls.p2 = 7;
			cls.p3 = 8;
			cls.p4 = 9;

			Assert::AreEqual(static_cast<int>(*cls.pDefaultProp), 5);
			// Assert::AreEqual(static_cast<int>(*cls.pProp1), 6); // Compile error
			Assert::AreEqual(static_cast<int>(*cls.pProp2), 7);
			// Assert::AreEqual(static_cast<int>(*cls.pProp3), 8); // Compile error
			Assert::AreEqual(static_cast<int>(*cls.pProp4), 9);

			*cls.pDefaultProp = 10;
			// *cls.pProp1 = 11; // Compile error
			*cls.pProp2 = 12;
			// *cls.pProp3 = 13; // Compile error
			*cls.pProp4 = 14;
		}
	};
}