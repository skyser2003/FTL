#pragma once

namespace FTL
{
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

	// Base class
	template <class OwnerClass, class Type>
	class _Property
	{
	protected:
		_Property() : _Property(DefaultSetter<Type>(), DefaultGetter<Type>())
		{

		}

		_Property(std::function<Type(Type)> setter, std::function<Type(Type)> getter) : value(Type())
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

	// Both public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, false> : public _Property<OwnerClass, Type>
	{
	public:
		friend OwnerClass;

		using _Property::_Property;

		Type operator=(const Type& rhs)
		{
			return _Property::operator=(rhs);
		}

		operator Type() const
		{
			return _Property::operator Type();
		}
	};

	// Setter private, getter public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, false> : public _Property<OwnerClass, Type>
	{
	public:
		friend OwnerClass;

		using _Property::_Property;

	private:
		Type operator=(const Type& rhs)
		{
			return _Property::operator=(rhs);
		}

	public:
		operator Type() const
		{
			return _Property::operator Type();
		}
	};

	// Setter public, Getter private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, true> : public _Property<OwnerClass, Type>
	{
	public:
		friend OwnerClass;

		using _Property::_Property;

		Type operator=(const Type& rhs)
		{
			return _Property::operator=(rhs);
		}

	private:
		operator Type() const
		{
			return _Property::operator Type();
		}
	};

	// Both private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, true> : public _Property<OwnerClass, Type>
	{
	public:
		friend OwnerClass;

		using _Property::_Property;

	private:
		Type operator=(const Type& rhs)
		{
			return _Property::operator=(rhs);
		}

		operator Type() const
		{
			return _Property::operator Type();
		}
	};
}