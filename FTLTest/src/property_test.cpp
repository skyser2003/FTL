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
		Property<TestClass, int, false, false, PropertyType::AutoGen> defaultProp;

		Property<TestClass, int, true, true, PropertyType::Manual> prop1{ [this]() { return innerI1; }, [this](int val) { innerI1 = val * val; } };
		Property<TestClass, int, false, true, PropertyType::Manual> prop2{ [this]() { return innerI2; }, [this](int val) { innerI2 = val * val; } };
		Property<TestClass, int, true, false, PropertyType::Manual> prop3{ [this]() { return innerI3; }, [this](int val) { innerI3 = val * val; } };
		Property<TestClass, int, false, false, PropertyType::Manual> prop4{ [this]() { return innerI4; }, [this](int val) { innerI4 = val * val; } };

		int innerI1, innerI2, innerI3, innerI4;

		int *pD, *p1, *p2, *p3, *p4;
		int pID, pI1, pI2, pI3, pI4;

		Property<TestClass, int*, false, false, PropertyType::AutoGen> pDefaultProp{ pD };

		Property<TestClass, int*, true, true, PropertyType::Manual> pProp1{ [this]() { return p1; }, [this](int* val) { p1 = val; } };
		Property<TestClass, int*, false, true, PropertyType::Manual> pProp2{ [this]() { return p2; }, [this](int* val) { p2 = val; } };
		Property<TestClass, int*, true, false, PropertyType::Manual> pProp3{ [this]() { return p3; }, [this](int* val) { p3 = val; } };
		Property<TestClass, int*, false, false, PropertyType::Manual> pProp4{ [this]() { return p4; }, [this](int* val) { p4 = val; } };

		// Pointer
		Property<TestClass, int*, false, false, PropertyType::AutoGen> ptrProp;

		// Smart Pointer
		Property<TestClass, shared_ptr<int>, false, false, PropertyType::AutoGen> smptrProp;

		// Getter only
		int goInnerValue;
		Property <TestClass, int, false, false, PropertyType::GetterOnly> go{ [this]() { return goInnerValue; } };

		// Setter only
		int soInnerValue;
		Property <TestClass, int, false, false, PropertyType::SetterOnly> so{ [this](int value) { soInnerValue = value; } };

		void Test()
		{
			defaultProp = 0;
			prop1 = 1;
			prop2 = 2;
			prop3 = 3;
			prop4 = 4;

			pDefaultProp = &pID;
			pProp1 = &pI1;
			pProp2 = &pI2;
			pProp3 = &pI3;
			pProp4 = &pI4;

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

			// Getter privateness
			Assert::AreEqual(false, IsGetterPrivate(cls.defaultProp, nullptr));
			Assert::AreEqual(true, IsGetterPrivate(cls.prop1, nullptr));
			Assert::AreEqual(false, IsGetterPrivate(cls.prop2, nullptr));
			Assert::AreEqual(true, IsGetterPrivate(cls.prop3, nullptr));
			Assert::AreEqual(false, IsGetterPrivate(cls.prop4, nullptr));

			Assert::AreEqual(false, IsGetterPrivate(cls.pDefaultProp, nullptr));
			Assert::AreEqual(true, IsGetterPrivate(cls.pProp1, nullptr));
			Assert::AreEqual(false, IsGetterPrivate(cls.pProp2, nullptr));
			Assert::AreEqual(true, IsGetterPrivate(cls.pProp3, nullptr));
			Assert::AreEqual(false, IsGetterPrivate(cls.pProp4, nullptr));

			// Setter privateness
			// Sealed until Visual Studio's bug is fixed
			/*
			Assert::AreEqual(false, IsSetterPrivate(cls.defaultProp, nullptr));
			Assert::AreEqual(true, IsSetterPrivate(cls.prop1, nullptr));
			Assert::AreEqual(true, IsSetterPrivate(cls.prop2, nullptr));
			Assert::AreEqual(false, IsSetterPrivate(cls.prop3, nullptr));
			Assert::AreEqual(false, IsSetterPrivate(cls.prop4, nullptr));

			Assert::AreEqual(false, IsSetterPrivate(cls.pDefaultProp, nullptr));
			Assert::AreEqual(true, IsSetterPrivate(cls.pProp1, nullptr));
			Assert::AreEqual(true, IsSetterPrivate(cls.pProp2, nullptr));
			Assert::AreEqual(false, IsSetterPrivate(cls.pProp3, nullptr));
			Assert::AreEqual(false, IsSetterPrivate(cls.pProp4, nullptr));
			*/
		}

		TEST_METHOD(PropertyTest2)
		{
			FTLTest::TestClass cls;
			cls.Test();

			// Getter value
			Assert::AreEqual(0, static_cast<int>(cls.defaultProp));
			// Assert::AreEqual(1 * 1, static_cast<int>(cls.prop1)); // Compile error
			Assert::AreEqual(2 * 2, static_cast<int>(cls.prop2));
			// Assert::AreEqual(3 * 3, static_cast<int>(cls.prop3)); // Compile error
			Assert::AreEqual(4 * 4, static_cast<int>(cls.prop4));

			// Setter value
			Assert::AreEqual(1, static_cast<int>(cls.defaultProp = 1));
			// Assert::AreEqual(2 * 2, static_cast<int>(cls.prop1 = 2)); // Compile error
			// Assert::AreEqual(3 * 3, static_cast<int>(cls.prop2 = 3)); // Compile error
			Assert::AreEqual(4, static_cast<int>(cls.prop3 = 4));
			Assert::AreEqual(5, static_cast<int>(cls.prop4 = 5));

			// After set
			Assert::AreEqual(1 * 1, static_cast<int>(cls.defaultProp));
			// Assert::AreEqual(2 * 2, static_cast<int>(cls.prop1)); // Setter private, compile error
			// Assert::AreEqual(3 * 3, static_cast<int>(cls.prop2)); // Setter private
			// Assert::AreEqual(4 * 4, static_cast<int>(cls.prop3)); // Compile error
			Assert::AreEqual(5 * 5, static_cast<int>(cls.prop4));
		}

		TEST_METHOD(PropertyTest3)
		{
			FTLTest::TestClass cls;

			int dummyInt = 5555;
			int dummyInt1 = 6666;
			int dummyInt2 = 7777;
			int dummyInt3 = 8888;
			int dummyInt4 = 9999;

			Assert::AreEqual(&dummyInt, static_cast<int*>(cls.pDefaultProp = &dummyInt));
			// Assert::AreEqual(&dummyInt1, static_cast<int*>(cls.pProp1 = &dummyInt1)); // Compile error
			// Assert::AreEqual(&dummyInt2, static_cast<int*>(cls.pProp2 = &dummyInt2)); // Compile error
			Assert::AreEqual(&dummyInt3, static_cast<int*>(cls.pProp3 = &dummyInt3));
			Assert::AreEqual(&dummyInt4, static_cast<int*>(cls.pProp4 = &dummyInt4));
		}

		TEST_METHOD(PropertyTest4)
		{
			FTLTest::TestClass cls;
			cls.Test();

			// Pointer dereference(getter)
			Assert::AreEqual(0, static_cast<int>(*cls.pDefaultProp));
			// Assert::AreEqual(1, static_cast<int>(*cls.pProp1)); // Compile error
			Assert::AreEqual(2, static_cast<int>(*cls.pProp2));
			// Assert::AreEqual(3, static_cast<int>(*cls.pProp3)); // Compile error
			Assert::AreEqual(4, static_cast<int>(*cls.pProp4));

			// Pointer deference(setter)
			*cls.pDefaultProp = 10;
			// *cls.pProp1 = 11; // Compile error
			*cls.pProp2 = 12;
			// *cls.pProp3 = 13; // Compile error
			*cls.pProp4 = 14;

			Assert::AreEqual(10, static_cast<int>(*cls.pDefaultProp));
			// Assert::AreEqual(11, static_cast<int>(*cls.pProp1)); // Compile error
			Assert::AreEqual(12, static_cast<int>(*cls.pProp2));
			// Assert::AreEqual(13, static_cast<int>(*cls.pProp3)); // Compile error
			Assert::AreEqual(14, static_cast<int>(*cls.pProp4));
		}

		TEST_METHOD(PropertyTest5)
		{
			FTLTest::TestClass cls;
			cls.Test();

			// Getter only
			cls.goInnerValue = 3;
			// cls.go = 4; // Compile error
			Assert::AreEqual(3, cls.go.get());

			// Setter only
			cls.so = 6;
			Assert::AreEqual(6, cls.soInnerValue);
			// Assert::AreEqual(6, cls.so.get()); // Compile error
		}

	private:
		template <class T>
		constexpr bool IsGetterPrivate(const T&, ...) const
		{
			return true;
		}

		template <class T>
		constexpr bool IsGetterPrivate(const T&, decltype(typename declval<T>().operator T::InterfaceType())* ptr) const
		{
			return false;
		}

		template <class T>
		constexpr bool IsSetterPrivate(const T&, ...) const
		{
			return true;
		}

		template <class T>
		constexpr bool IsSetterPrivate(const T&, decltype(typename declval<T>().operator=(T()))* ptr) const
		{
			return false;
		}

		// Sealed due to Visual Studio C++ compiler's bug.  Works well on clang.
		/*
		template <class T, class = void>
		class IsGetterPrivate : public true_type {};

		template <class T>
		class IsGetterPrivate<T, VoidTemplate<decltype(typename declval<T>().operator T::Type())>> : public false_type {};
		*/
	};
}