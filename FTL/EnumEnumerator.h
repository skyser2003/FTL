#pragma once

template <class Enum>
class EnumIterator
{
public:
	EnumIterator()
	{

	}

	EnumIterator(Enum val)
	{
		this->val = val;
	}

	bool operator !=(const EnumIterator& rhs)
	{
		return val != rhs.val;
	}

	Enum operator++()
	{
		return val = static_cast<Enum>(std::underlying_type<Enum>::type(val) + 1);
	}

	Enum operator*()
	{
		return val;
	}

private:
	Enum val;
};

template <class Enum, Enum first, Enum last>
class EnumEnumerator
{
public:
	EnumIterator<Enum> begin() const
	{
		if (includeFirst == true)
		{
			return first;
		}
		else
		{
			return static_cast<Enum>(std::underlying_type<Enum>::type(first) + 1);
		}
	}

	EnumIterator<Enum> end() const
	{
		if (includeLast == true)
		{
			return static_cast<Enum>(std::underlying_type<Enum>::type(last) + 1);
		}
		else
		{
			return last;
		}
	}

	void Init(bool includeFirst, bool includeLast)
	{
		this->includeFirst = includeFirst;
		this->includeLast = includeLast;
	}

private:
	bool includeFirst = true;
	bool includeLast = true;
};