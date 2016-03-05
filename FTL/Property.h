#pragma once

#include "PointerDataType.h"

namespace FTL
{
	// Return type specialization
	template <class Type, bool isFundamental = std::is_fundamental<Type>::value>
	class PropertyReturnType;

	template <class Type>
	class PropertyReturnType<Type, true>
	{
	public:
		typedef Type Type;
	};

	template <class Type>
	class PropertyReturnType<Type, false>
	{
	public:
		typedef Type& Type;
	};

	// Default getter & setter
	template <class Type, bool returnReference = IsPointer<Type>::value == false && std::is_fundamental<Type>::value>
	class DefaultSetter;

	template <class Type, bool returnReference = IsPointer<Type>::value == false && std::is_fundamental<Type>::value>
	class DefaultGetter;


	template <class Type>
	class DefaultSetter<Type, true>
	{
	public:
		Type& operator()(const Type& rhs)
		{
			return const_cast<Type&>(rhs);
		}
	};

	template <class Type>
	class DefaultSetter<Type, false>
	{
	public:
		Type operator()(const Type& rhs)
		{
			return rhs;
		}
	};

	template <class Type>
	class DefaultGetter<Type, true>
	{
	public:
		Type& operator()(const Type& rhs)
		{
			return const_cast<Type&>(rhs);
		}
	};

	template <class Type>
	class DefaultGetter<Type, false>
	{
	public:
		Type operator()(const Type& rhs)
		{
			return rhs;
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
		typedef typename PropertyReturnType<Type>::Type ReturnType;

	protected:
		PropertyBase() : PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), Type())
		{

		}

		PropertyBase(Type value) : PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), value)
		{

		}

		PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
			: PropertyBase(setter, getter, Type())
		{

		}

		PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		ReturnType get() const
		{
			return getter(value);
		}

		ReturnType operator=(const Type& rhs)
		{
			return value = setter(rhs);
		}

		operator ReturnType() const
		{
			return get();
		}

	private:
		Type value;

		std::function<Type(Type)> setter;
		std::function<Type(Type)> getter;
	};

	// Base class pointer specialization
	template <class Type>
	class PropertyBase<Type, true>
	{
	public:
		typedef Type Type;
		typedef Type ReturnType;

	protected:
		PropertyBase() : PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), Type())
		{

		}

		PropertyBase(Type value) : PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), value)
		{

		}

		PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
			: PropertyBase(setter, getter, Type())
		{

		}

		PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		ReturnType get() const
		{
			return getter(value);
		}

		ReturnType operator=(const Type& rhs)
		{
			return value = setter(rhs);
		}

		operator ReturnType() const
		{
			return get();
		}

		decltype(*Type()) operator*()
		{
			return *value;
		}

		Type& operator->()
		{
			return value;
		}

		const decltype(*Type()) operator*() const
		{
			return *value;
		}

		const Type& operator->() const
		{
			return value;
		}

	private:
		Type value;

		std::function<Type(Type)> setter;
		std::function<Type(Type)> getter;
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
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}
	};

	// Non-pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, true, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

	private:
		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	public:
		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}
	};

	// Non-pointer specialization : Setter public, Getter private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, false, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}
	};

	// Non-pointer specialization : Both private
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, true, true, false> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

	private:
		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}
	};

	// Pointer specialization : Both public
	template <class OwnerClass, class Type>
	class Property<OwnerClass, Type, false, false, true> : public PropertyBase<Type>
	{
	public:
		friend OwnerClass;

		using PropertyBase::PropertyBase;
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
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
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

	private:
		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	public:
		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
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
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
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
		typedef typename PropertyBase<Type>::ReturnType ReturnType;

	private:
		decltype(auto) get() const
		{
			return PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return PropertyBase::operator=(rhs);
		}

		operator ReturnType() const
		{
			return PropertyBase::operator ReturnType();
		}

		decltype(auto) operator*()
		{
			return PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
		{
			return PropertyBase::operator->();
		}
	};
}