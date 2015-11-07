#include "stdafx.h"
#include "CppUnitTest.h"

#include "Property.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace FTLTest
{
	TEST_CLASS(PropertyTest)
	{
	public:
		TEST_METHOD(PropertyTest1)
		{
			char buffer[25];
			Property<PropertyTest, int, true, true> prop([](int val) {return val*val; }, DefaultGetter<int>());
			Logger::WriteMessage(itoa((prop = 3), buffer, 10));
			Logger::WriteMessage("\n");
			Logger::WriteMessage(itoa(prop, buffer, 10));
			Logger::WriteMessage("\n");
		}
	};
}