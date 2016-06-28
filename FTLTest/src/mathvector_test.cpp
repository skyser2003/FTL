#include "stdafx.h"
#include "CppUnitTest.h"

#include "MathVector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FTL;

namespace FTLTest
{
	TEST_CLASS(MathVectorTest)
	{
	public:
		TEST_METHOD(MathVectorTest1)
		{
			Vector2i vec2i;
			Vector3i vec3i;
			Vector2f vec2f;
			Vector3f vec3f;

			vec2i.Value[0] = 1;
			vec2i.Value[1] = 2;

			vec3i.Value[0] = 4;
			vec3i.Value[1] = 5;
			vec3i.Value[2] = 6;

			vec2f.Value[0] = 1.5f;
			vec2f.Value[1] = 2.5f;

			vec3f.Value[0] = 4.5f;
			vec3f.Value[1] = 5.5f;
			vec3f.Value[2] = 6.5f;

			// Int
			Assert::AreEqual(1, static_cast<int>(vec2i.Value[0]));
			Assert::AreEqual(2, static_cast<int>(vec2i.Value[1]));

			Assert::AreEqual(1, static_cast<int>(vec2i.X));
			Assert::AreEqual(2, static_cast<int>(vec2i.Y));

			Assert::AreEqual(4, static_cast<int>(vec3i.Value[0]));
			Assert::AreEqual(5, static_cast<int>(vec3i.Value[1]));
			Assert::AreEqual(6, static_cast<int>(vec3i.Value[2]));

			Assert::AreEqual(4, static_cast<int>(vec3i.X));
			Assert::AreEqual(5, static_cast<int>(vec3i.Y));
			Assert::AreEqual(6, static_cast<int>(vec3i.Z));
			Assert::AreEqual(1, static_cast<int>(vec2i.Value[0]));
			Assert::AreEqual(2, static_cast<int>(vec2i.Value[1]));

			// Float
			Assert::AreEqual(1.5f, static_cast<float>(vec2f.X));
			Assert::AreEqual(2.5f, static_cast<float>(vec2f.Y));

			Assert::AreEqual(4.5f, static_cast<float>(vec3f.Value[0]));
			Assert::AreEqual(5.5f, static_cast<float>(vec3f.Value[1]));
			Assert::AreEqual(6.5f, static_cast<float>(vec3f.Value[2]));

			Assert::AreEqual(4.5f, static_cast<float>(vec3f.X));
			Assert::AreEqual(5.5f, static_cast<float>(vec3f.Y));
			Assert::AreEqual(6.5f, static_cast<float>(vec3f.Z));
		}

		TEST_METHOD(MathVectorTest2)
		{
			Pos.get().Set(0, 0, 0);
		}

	private:
		FTL::Property<MathVectorTest, FTL::Vector3f, false, false, true> Pos;
	};
}