#pragma once

#include "PointerDataType.h"

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

	template <class OwnerClass, class Type, bool IsPointer>
	class _PropertyBase;

	// Base class non-pointer specialization
	template <class OwnerClass, class Type>
	class _PropertyBase<OwnerClass, Type, false>
	{
	public:
		typedef Type Type;

	protected:
		_PropertyBase() : _PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), Type())
		{

		}

		_PropertyBase(Type value) : _PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), value)
		{

		}

		_PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
			: _PropertyBase(setter, getter, Type())
		{

		}

		_PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		Type get() const
		{
			return getter(value);
		}

		Type& operator=(const Type& rhs)
		{
			return value = setter(rhs);
		}

		operator Type() const
		{
			return get();
		}

	private:
		Type value;

		std::function<Type(Type)> setter;
		std::function<Type(Type)> getter;
	};

	// Base class pointer specialization
	template <class OwnerClass, class Type>
	class _PropertyBase<OwnerClass, Type, true>
	{
	public:
		typedef Type Type;

	protected:
		_PropertyBase() : _PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), Type())
		{

		}

		_PropertyBase(Type value) : _PropertyBase(DefaultSetter<Type>(), DefaultGetter<Type>(), value)
		{

		}

		_PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter)
			: _PropertyBase(setter, getter, Type())
		{

		}

		_PropertyBase(std::function<Type(Type)> setter, std::function<Type(Type)> getter, Type value)
			: setter(setter), getter(getter), value(value)
		{

		}

		Type get() const
		{
			return getter(value);
		}

		Type& operator=(const Type& rhs)
		{
			return value = setter(rhs);
		}

		operator Type() const
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
	template <class OwnerClass, class Type, bool isGetterPrivate, bool isSetterPrivate, bool isPointer>
	class _Property;

	template <class OwnerClass, class Type, bool isGetterPrivate, bool isSetterPrivate>
	using Property = _Property<OwnerClass, Type, isGetterPrivate, isSetterPrivate, IsPointer<Type>::value>;

	// Non-pointer specialization : Both public
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, false, false, false> : public _PropertyBase<OwnerClass, Type, false>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}
	};

	// Non-pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, false, true, false> : public _PropertyBase<OwnerClass, Type, false>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

	private:
		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

	public:
		operator Type() const
		{
			return _PropertyBase::operator Type();
		}
	};

	// Non-pointer specialization : Setter public, Getter private
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, true, false, false> : public _PropertyBase<OwnerClass, Type, false>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}
	};

	// Non-pointer specialization : Both private
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, true, true, false> : public _PropertyBase<OwnerClass, Type, false>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

	private:
		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}
	};

	// Pointer specialization : Both public
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, false, false, true> : public _PropertyBase<OwnerClass, Type, true>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}

		decltype(auto) operator*()
		{
			return _PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return _PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return _PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
		{
			return _PropertyBase::operator->();
		}
	};

	// Pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, false, true, true> : public _PropertyBase<OwnerClass, Type, true>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

	private:
		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

	public:
		operator Type() const
		{
			return _PropertyBase::operator Type();
		}

		decltype(auto) operator*()
		{
			return _PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return _PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return _PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
		{
			return _PropertyBase::operator->();
		}
	};

	// Pointer specialization : Setter public, getter private
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, true, false, true> : public _PropertyBase<OwnerClass, Type, true>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

	private:
		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}

		decltype(auto) operator*()
		{
			return _PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return _PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return _PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
		{
			return _PropertyBase::operator->();
		}
	};

	// Pointer specialization : Both private
	template <class OwnerClass, class Type>
	class _Property<OwnerClass, Type, true, true, true> : public _PropertyBase<OwnerClass, Type, true>
	{
	public:
		friend OwnerClass;

		using _PropertyBase::_PropertyBase;

	private:
		decltype(auto) get() const
		{
			return _PropertyBase::get();
		}

		decltype(auto) operator=(const Type& rhs)
		{
			return _PropertyBase::operator=(rhs);
		}

		operator Type() const
		{
			return _PropertyBase::operator Type();
		}

		decltype(auto) operator*()
		{
			return _PropertyBase::operator*();
		}

		decltype(auto) operator->()
		{
			return _PropertyBase::operator->();
		}

		const decltype(*Type()) operator*() const
		{
			return _PropertyBase::operator*();
		}

		const decltype(*Type()) operator->() const
		{
			return _PropertyBase::operator->();
		}
	};
}