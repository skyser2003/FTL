#include "stdafx.h"
#include "HelperFunction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FTLTest
{
	void Log(const std::string& val)
	{
		Logger::WriteMessage(val.c_str());
	}

	void Log(int val)
	{
		char buffer[255];
		Logger::WriteMessage(itoa(val, buffer, 10));
	}

	void Logln(const std::string& val)
	{
		Log(val);
		Log("\n");
	}

	void Logln(int val)
	{
		Log(val);
		Log("\n");
	}
}