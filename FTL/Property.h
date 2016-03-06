#pragma once

#include "PointerDataType.h"

namespace FTL
{
	// Return type specialization
	template <class Type, bool isFundamental = std::is_fundamental<Type>::value>
	class PropertyInterfaceType;

	template <class Type>
	class PropertyInterfaceType<Type, true>
	{
	public:
		typedef Type Type;
	};

	template <class Type>
	class PropertyInterfaceType<Type, false>
	{
	public:
		typedef Type& Type;
	};

	// Default getter & setter
	template <class Type, class InterfaceType>
	class DefaultSetter
	{
	public:
		InterfaceType operator()(InterfaceType rhs)
		{
			return static_cast<InterfaceType>(rhs);
		}
	};

	template <class Type, class InterfaceType>
	class DefaultGetter
	{
	public:
		InterfaceType operator()(InterfaceType rhs)
		{
			return static_cast<InterfaceType>(rhs);
		}
	};

	// Base class
	template <class Type, bool IsPointer = IsPointer<Type>::value>
	class PropertyBase;

	// Base class non-pointer specialization
	template <class Type>
	class PropertyBase<Type, false>
	{
	public:
		typedef Type Type;
		typedef typename PropertyInterfaceType<Type>::Type InterfaceType;

		typedef std::function<InterfaceType(InterfaceType)> SetterType;
		typedef std::function<InterfaceType(InterfaceType)> GetterType;

	protected:
		PropertyBase() : PropertyBase(DefaultSetter<Type, InterfaceType>(), DefaultGetter<Type, InterfaceType>())
		{

		}

		PropertyBase(Type value) : PropertyBase(DefaultSetter<Type, InterfaceType>(), DefaultGetter<Type, InterfaceType>(), value)
		{

		}

		PropertyBase(SetterType setter, GetterType getter) : setter(setter), getter(getter)
		{

		}

		PropertyBase(SetterType setter, GetterType getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		InterfaceType get()
		{
			return getter(value);
		}

		InterfaceType operator=(InterfaceType rhs)
		{
			return value = setter(rhs);
		}

		operator InterfaceType()
		{
			return get();
		}

	private:
		Type value;

		SetterType setter;
		GetterType getter;
	};

	// Base class pointer specialization
	template <class Type>
	class PropertyBase<Type, true>
	{
	public:
		typedef Type Type;
		typedef Type InterfaceType;

		typedef std::function<InterfaceType(InterfaceType)> SetterType;
		typedef std::function<InterfaceType(InterfaceType)> GetterType;

	protected:
		PropertyBase() : PropertyBase(DefaultSetter<Type, InterfaceType>(), DefaultGetter<Type, InterfaceType>())
		{

		}

		PropertyBase(Type value) : PropertyBase(DefaultSetter<Type, InterfaceType>(), DefaultGetter<Type, InterfaceType>(), value)
		{

		}

		PropertyBase(SetterType setter, GetterType getter) : setter(setter), getter(getter)
		{

		}

		PropertyBase(SetterType setter, GetterType getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		InterfaceType get()
		{
			return getter(value);
		}

		InterfaceType operator=(InterfaceType rhs)
		{
			return value = setter(rhs);
		}

		operator InterfaceType()
		{
			return get();
		}

		decltype(*Type()) operator*()
		{
			return *getter(value);
		}

		InterfaceType operator->()
		{
			return getter(value);
		}

		const decltype(*Type()) operator*() const
		{
			return *getter(value);
		}

		const InterfaceType operator->() const
		{
			return getter(value);
		}

	private:
		Type value;

		SetterType setter;
		GetterType getter;
	};

	// Declare
	template <class OwnerClass, class Type, bool isGetterPrivate, bool isSetterPrivate, bool isPointer = IsPointer<Type>::value>
	class Property;

	// Non-pointer specialization : Both public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, false, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}
	};

	// Non-pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, true, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) get()
		{
			return PropertyBase::get();
		}

	private:
		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	public:
		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}
	};

	// Non-pointer specialization : Setter public, Getter private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, false, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}
	};

	// Non-pointer specialization : Both private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, true, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

	private:
		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}
	};

	// Pointer specialization : Both public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, false, true> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyBase::operator->();
		}
	};

	// Pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, true, true> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) get()
		{
			return PropertyBase::get();
		}

	private:
		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	public:
		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyBase::operator->();
		}
	};

	// Pointer specialization : Setter public, getter private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, false, true> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyBase::operator->();
		}
	};

	// Pointer specialization : Both private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, true, true> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::InterfaceType InterfaceType;

	private:
		decltype(auto) get()
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyBase::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyBase::operator->();
		}
	};
}