#pragma once

template <class Type>
class DefaultSetter
{
public:
	Type operator()(const Type& rhs)
	{
		return rhs;
	}
};

template <class Type>
class DefaultGetter
{
public:
	Type operator()(const Type& rhs)
	{
		return rhs;
	}
};

// Declare
template <class OwnerClass, class Type, bool setPrivate, bool getPrivate>
class Property;

// Both public
template <class OwnerClass, class Type>
class Property<OwnerClass, Type, false, false>
{
public:
	Property() :Property(DefaultSetter<Type>(), DefaultGetter<Type>())
	{

	}

	Property(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
	{
		this->setter = setter;
		this->getter = getter;
	}

	Type operator=(const Type& rhs)
	{
		return value = setter(rhs);
	}

	operator Type() const
	{
		return getter(value);
	}

private:
	Type value;

	std::function<Type(Type)> setter;
	std::function<Type(Type)> getter;
};

// Setter private, getter public
template <class OwnerClass, class Type>
class Property<OwnerClass, Type, true, false>
{
public:
	friend OwnerClass;

	Property() :Property(DefaultSetter<Type>(), DefaultGetter<Type>())
	{

	}

	Property(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
	{
		this->setter = setter;
		this->getter = getter;
	}

private:
	Type operator=(const Type& rhs)
	{
		return value = setter(rhs);
	}

public:
	operator Type() const
	{
		return getter(value);
	}

private:
	Type value;

	std::function<Type(Type)> setter;
	std::function<Type(Type)> getter;
};

// Setter public, Getter private
template <class OwnerClass, class Type>
class Property<OwnerClass, Type, false, true>
{
public:
	friend OwnerClass;

	Property() :Property(DefaultSetter<Type>(), DefaultGetter<Type>())
	{

	}

	Property(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
	{
		this->setter = setter;
		this->getter = getter;
	}

	Type operator=(const Type& rhs)
	{
		return value = setter(rhs);
	}

private:
	operator Type() const
	{
		return getter(value);
	}

private:
	Type value;

	std::function<Type(Type)> setter;
	std::function<Type(Type)> getter;
};

// Both private
template <class OwnerClass, class Type>
class Property<OwnerClass, Type, true, true>
{
public:
	friend OwnerClass;

	Property() :Property(DefaultSetter<Type>(), DefaultGetter<Type>())
	{

	}

	Property(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
	{
		this->setter = setter;
		this->getter = getter;
	}

private:
	Type operator=(const Type& rhs)
	{
		return value = setter(rhs);
	}

	operator Type() const
	{
		return getter(value);
	}

private:
	Type value;

	std::function<Type(Type)> setter;
	std::function<Type(Type)> getter;
};