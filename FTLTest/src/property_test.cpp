#include "stdafx.h"
#include "CppUnitTest.h"

#include "Property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace FTL;

namespace FTLTest
{
	template <class OwnerClass, class Type, PropertyType PropType, auto... args>
	using PROP = Property<OwnerClass, Type, PropType, args...>;

	class TestClass
	{
	public:
		friend class PropertyTest;

	public:
		// Value
		PROP<TestClass, int, PropertyType::AutoGen, false, false> defaultProp;

		PROP<TestClass, int, PropertyType::Manual, true, true> prop1{ [this]() { return innerI1; }, [this](int val) { innerI1 = val * val; } };
		PROP<TestClass, int, PropertyType::Manual, false, true> prop2{ [this]() { return innerI2; }, [this](int val) { innerI2 = val * val; } };
		PROP<TestClass, int, PropertyType::Manual, true, false> prop3{ [this]() { return innerI3; }, [this](int val) { innerI3 = val * val; } };
		PROP<TestClass, int, PropertyType::Manual, false, false> prop4{ [this]() { return innerI4; }, [this](int val) { innerI4 = val * val; } };

		int innerI1, innerI2, innerI3, innerI4;

		int *pD, *p1, *p2, *p3, *p4;
		int pID, pI1, pI2, pI3, pI4;

		PROP<TestClass, int*, PropertyType::AutoGen, false, false> pDefaultProp{ pD };

		PROP<TestClass, int*, PropertyType::Manual, true, true> pProp1{
			[this]() { return p1; },
			[this](int* val) { p1 = val; },
			[this](int val) { *p1 = val; } };

		PROP<TestClass, int*, PropertyType::Manual, false, true> pProp2{
			[this]() { return p2; },
			[this](int* val) { p2 = val; },
			[this](int val) { *p2 = val; } };

		PROP<TestClass, int*, PropertyType::Manual, true, false> pProp3{
			[this]() { return p3; },
			[this](int* val) { p3 = val; },
			[this](int val) { *p3 = val; } };

		PROP<TestClass, int*, PropertyType::Manual, false, false> pProp4{
			[this]() { return p4; },
			[this](int* val) { p4 = val; },
			[this](int val) { *p4 = val; } };

		// Pointer
		PROP<TestClass, int*, PropertyType::AutoGen, false, false> ptrProp;

		// Smart Pointer
		PROP<TestClass, shared_ptr<int>, PropertyType::AutoGen, false, false> smptrProp;

		// Getter only
		int goInnerValue;
		PROP<TestClass, int, PropertyType::GetterOnly, false> go{ [this]() { return goInnerValue; } };

		// Setter only
		int soInnerValue;
		PROP<TestClass, int, PropertyType::SetterOnly, false> so{ [this](int value) { soInnerValue = value; } };

		void Test()
		{
			defaultProp = 0;
			prop1 = 1;
			prop2 = 2;
			prop3 = 3;
			prop4 = 4;

			pD = &pID;
			p1 = &pI1;
			p2 = &pI2;
			p3 = &pI3;
			p4 = &pI4;

			pDefaultProp = pD;
			pProp1 = p1;
			pProp2 = p2;
			pProp3 = p3;
			pProp4 = p4;

			pDefaultProp.setValue(0);
			*p1 = 1; // pProp1.setValue(1); // Compile error
			*p2 = 2; // pProp2.setValue(2); // Compile error
			pProp3.setValue(3);
			pProp4.setValue(4);
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
			Assert::AreEqual(false, IsGetterPrivate(cls.defaultProp));
			Assert::AreEqual(true, IsGetterPrivate(cls.prop1));
			Assert::AreEqual(false, IsGetterPrivate(cls.prop2));
			Assert::AreEqual(true, IsGetterPrivate(cls.prop3));
			Assert::AreEqual(false, IsGetterPrivate(cls.prop4));

			Assert::AreEqual(false, IsGetterPrivate(cls.pDefaultProp));
			Assert::AreEqual(true, IsGetterPrivate(cls.pProp1));
			Assert::AreEqual(false, IsGetterPrivate(cls.pProp2));
			Assert::AreEqual(true, IsGetterPrivate(cls.pProp3));
			Assert::AreEqual(false, IsGetterPrivate(cls.pProp4));

			// Setter privateness
			Assert::AreEqual(false, IsSetterPrivate(cls.defaultProp));
			Assert::AreEqual(true, IsSetterPrivate(cls.prop1));
			Assert::AreEqual(true, IsSetterPrivate(cls.prop2));
			Assert::AreEqual(false, IsSetterPrivate(cls.prop3));
			Assert::AreEqual(false, IsSetterPrivate(cls.prop4));

			Assert::AreEqual(false, IsSetterPrivate(cls.pDefaultProp));
			Assert::AreEqual(true, IsSetterPrivate(cls.pProp1));
			Assert::AreEqual(true, IsSetterPrivate(cls.pProp2));
			Assert::AreEqual(false, IsSetterPrivate(cls.pProp3));
			Assert::AreEqual(false, IsSetterPrivate(cls.pProp4));
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
			cls.Test();

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

			// getValue
			Assert::AreEqual(0, static_cast<int>(cls.pDefaultProp.getValue()));
			// Assert::AreEqual(1, static_cast<int>(cls.pProp1.getValue())); // Compile error
			Assert::AreEqual(2, static_cast<int>(cls.pProp2.getValue()));
			// Assert::AreEqual(3, static_cast<int>(cls.pProp3.getValue())); // Compile error
			Assert::AreEqual(4, static_cast<int>(cls.pProp4.getValue()));

			// setValue
			cls.pDefaultProp.setValue(10);
			*cls.p1 = 11; // cls.pProp1.setValue(11); // Compile error
			*cls.p2 = 12; // cls.pProp2.setValue(12); // Compile error
			cls.pProp3.setValue(13);
			cls.pProp4.setValue(14);

			Assert::AreEqual(10, static_cast<int>(cls.pDefaultProp.getValue()));
			// Assert::AreEqual(11, static_cast<int>(*cls.pProp1)); // Compile error
			Assert::AreEqual(12, static_cast<int>(cls.pProp2.getValue()));
			// Assert::AreEqual(13, static_cast<int>(*cls.pProp3)); // Compile error
			Assert::AreEqual(14, static_cast<int>(cls.pProp4.getValue()));
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

		TEST_METHOD(PropertyTest6)
		{
			FTLTest::TestClass cls;
			cls.Test();

			Assert::AreEqual(false, cls.defaultProp.isPointer);
			Assert::AreEqual(false, cls.prop1.isPointer);
			Assert::AreEqual(false, cls.prop2.isPointer);
			Assert::AreEqual(false, cls.prop3.isPointer);
			Assert::AreEqual(false, cls.prop4.isPointer);

			Assert::AreEqual(true, cls.pDefaultProp.isPointer);
			Assert::AreEqual(true, cls.pProp1.isPointer);
			Assert::AreEqual(true, cls.pProp2.isPointer);
			Assert::AreEqual(true, cls.pProp3.isPointer);
			Assert::AreEqual(true, cls.pProp4.isPointer);
			Assert::AreEqual(true, cls.smptrProp.isPointer);
		}

	private:
		template <class T, class = void>
		class IsGetterPrivateCls : public true_type {};

		template <class T>
		class IsGetterPrivateCls<T, void_t<decltype(declval<T>().get())>> : public false_type {};

		template <class T, class = void>
		class IsSetterPrivateCls : public true_type {};

		template <class T>
		class IsSetterPrivateCls<T, void_t<decltype(declval<T>().operator=(typename T::InterfaceType()))>> : public false_type {};

		template <class T>
		constexpr bool IsGetterPrivate(const T&)
		{
			return typename IsGetterPrivateCls<T>::value;
		}

		template <class T>
		constexpr bool IsSetterPrivate(const T&)
		{
			return typename IsSetterPrivateCls<T>::value;
		}
	};
}