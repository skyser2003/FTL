#pragma once

#include "IsPointer.h"

namespace FTL
{
	// Property type
	enum class PropertyType
	{
		Manual,
		AutoGen,
		GetterOnly,
		SetterOnly,
	};

	enum class PropertyAccessorSaveType
	{
		GetterOnly,
		SetterOnly,
		Both
	};

	// Return type specialization
	template <class Type, bool isReference = std::is_fundamental<Type>::value == false && IsPointer<Type>::value == false>
	class PropertyInterfaceType;

	template <class _Type>
	class PropertyInterfaceType<_Type, false>
	{
	public:
		typedef _Type Type;
	};

	template <class _Type>
	class PropertyInterfaceType<_Type, true>
	{
	public:
		typedef _Type& Type;
	};

	// Default getter & setter
	template <class Type, class InterfaceType>
	class AutoGeneratedGetterSetter
	{
	public:
		AutoGeneratedGetterSetter() {}
		AutoGeneratedGetterSetter(Type value) : value(value) {}

		InterfaceType get()
		{
			return static_cast<InterfaceType>(value);
		}

		void set(InterfaceType rhs)
		{
			value = rhs;
		}

	private:
		Type value;
	};

	// Getter base class
	template <class Type, bool isPointer = IsPointer<Type>::value>
	class PropertyGetterBase;

	// Non pointer getter
	template <class Type>
	class PropertyGetterBase<Type, false>
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = std::function<InterfaceType(void)>;

	protected:
		PropertyGetterBase(GetterType getter) : getter(getter) {}

		InterfaceType get()
		{
			return getter();
		}

		InterfaceType get() const
		{
			return getter();
		}

		operator InterfaceType()
		{
			return get();
		}

		typename std::remove_reference<InterfaceType>::type* operator->()
		{
			return &getter();
		}

		InterfaceType operator+(const PropertyGetterBase& rhs) const
		{
			return get() + rhs.get();
		}

		InterfaceType operator+(InterfaceType rhs) const
		{
			return get() + rhs;
		}

		InterfaceType operator-(const PropertyGetterBase& rhs) const
		{
			return get() - rhs.get();
		}

		InterfaceType operator-(InterfaceType rhs) const
		{
			return get() - rhs;
		}

		InterfaceType operator*(const PropertyGetterBase& rhs) const
		{
			return get() * rhs.get();
		}

		InterfaceType operator*(InterfaceType rhs) const
		{
			return get() * rhs;
		}

		InterfaceType operator/(const PropertyGetterBase& rhs) const
		{
			return get() / rhs.get();
		}

		InterfaceType operator/(InterfaceType rhs) const
		{
			return get() / rhs;
		}

	protected:
		GetterType getter;
	};

	// Pointer getter
	template <class Type>
	class PropertyGetterBase<Type, true>
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = std::function<InterfaceType(void)>;

	protected:
		PropertyGetterBase(GetterType getter) : getter(getter) {}

		InterfaceType get()
		{
			return getter();
		}

		InterfaceType get() const
		{
			return getter();
		}

		operator InterfaceType()
		{
			return get();
		}

		decltype(*Type()) operator*()
		{
			return *getter();
		}

		InterfaceType operator->()
		{
			return getter();
		}

		const decltype(*Type()) operator*() const
		{
			return *getter();
		}

		const InterfaceType operator->() const
		{
			return getter();
		}

	protected:
		GetterType getter;
	};

	// Setter base class
	template <class Type>
	class PropertySetterBase
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using SetterType = std::function<void(InterfaceType)>;

	protected:
		PropertySetterBase(SetterType setter) : setter(setter) {}

		InterfaceType operator=(InterfaceType rhs)
		{
			setter(rhs);
			return rhs;
		}

	protected:
		SetterType setter;
	};

	// Base class
	template <class Type, PropertyAccessorSaveType AccSaveType>
	class PropertyBase;

	// Base class getter only specialization
	template <class Type>
	class PropertyBase<Type, PropertyAccessorSaveType::GetterOnly> : public PropertyGetterBase<Type>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = typename PropertyGetterBase<Type>::GetterType;
		using SetterType = typename PropertySetterBase<Type>::SetterType;

	protected:
		PropertyBase() = delete;
		PropertyBase(const PropertyBase& rhs) = delete;

		PropertyBase(GetterType getter) : PropertyGetterBase<Type>(getter) {}

		using PropertyGetterBase<Type>::get;

		InterfaceType operator+(const PropertyBase& rhs)
		{
			return get() + rhs.get();
		}

		InterfaceType operator+(InterfaceType rhs)
		{
			return get() + rhs;
		}

		InterfaceType operator-(const PropertyBase& rhs)
		{
			return get() - rhs.get();
		}

		InterfaceType operator-(InterfaceType rhs)
		{
			return get() - rhs;
		}

		InterfaceType operator*(const PropertyBase& rhs)
		{
			return get() * rhs.get();
		}

		InterfaceType operator*(InterfaceType rhs)
		{
			return get() * rhs;
		}

		InterfaceType operator/(const PropertyBase& rhs)
		{
			return get() / rhs.get();
		}

		InterfaceType operator/(InterfaceType rhs)
		{
			return get() / rhs;
		}
	};

	// Base class setter only specialization
	template <class Type>
	class PropertyBase<Type, PropertyAccessorSaveType::SetterOnly> : public PropertySetterBase<Type>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = typename PropertyGetterBase<Type>::GetterType;
		using SetterType = typename PropertySetterBase<Type>::SetterType;

	protected:
		PropertyBase() = delete;
		PropertyBase(const PropertyBase& rhs) = delete;

		PropertyBase(SetterType setter) : PropertySetterBase<Type>(setter) {}
	};

	// Base class both specialization
	template <class Type>
	class PropertyBase<Type, PropertyAccessorSaveType::Both> : public PropertyGetterBase<Type>, public PropertySetterBase<Type>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = typename PropertyGetterBase<Type>::GetterType;
		using SetterType = typename PropertySetterBase<Type>::SetterType;

	protected:
		PropertyBase() = delete;
		PropertyBase(const PropertyBase& rhs) = delete;

		PropertyBase(GetterType getter, SetterType setter) : PropertyGetterBase<Type>(getter), PropertySetterBase<Type>(setter) {}

		using PropertyGetterBase<Type>::get;

		InterfaceType operator+(const PropertyBase& rhs) const
		{
			return get() + rhs.get();
		}

		InterfaceType operator+(InterfaceType rhs) const
		{
			return get() + rhs;
		}

		InterfaceType operator-(const PropertyBase& rhs) const
		{
			return get() - rhs.get();
		}

		InterfaceType operator-(InterfaceType rhs) const
		{
			return get() - rhs;
		}

		InterfaceType operator*(const PropertyBase& rhs) const
		{
			return get() * rhs.get();
		}

		InterfaceType operator*(InterfaceType rhs) const
		{
			return get() * rhs;
		}

		InterfaceType operator/(const PropertyBase& rhs) const
		{
			return get() / rhs.get();
		}

		InterfaceType operator/(InterfaceType rhs) const
		{
			return get() / rhs;
		}

		InterfaceType operator+=(const PropertyBase& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator+=(rhs);
		}

		InterfaceType operator+=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator+=(rhs);
		}

		InterfaceType operator-=(const PropertyBase& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator-=(rhs);
		}

		InterfaceType operator-=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator-=(rhs);
		}

		InterfaceType operator*=(const PropertyBase& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator*=(rhs);
		}

		InterfaceType operator*=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator*=(rhs);
		}

		InterfaceType operator/=(const PropertyBase& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator/=(rhs);
		}

		InterfaceType operator/=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator/=(rhs);
		}
	};

	// Constructor
	template <class Type, PropertyType PropType>
	class PropertyConstructor;

	// Auto generated
	template <class Type>
	class PropertyConstructor<Type, PropertyType::AutoGen> : public PropertyBase<Type, PropertyAccessorSaveType::Both>
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = typename PropertyBase<Type, PropertyAccessorSaveType::Both>::GetterType;
		using SetterType = typename PropertyBase<Type, PropertyAccessorSaveType::Both>::SetterType;

	private:
		using AutoGen = AutoGeneratedGetterSetter<Type, InterfaceType>;

	public:
		PropertyConstructor()
			: PropertyBase<Type, PropertyAccessorSaveType::Both>
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1)
			) {}

		explicit PropertyConstructor(InterfaceType value) : autoGen(value),
			PropertyBase<Type, PropertyAccessorSaveType::Both>
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1)
			) {}

	private:
		AutoGen autoGen;
	};

	// Manual
	template <class Type>
	class PropertyConstructor<Type, PropertyType::Manual> : public PropertyBase<Type, PropertyAccessorSaveType::Both>
	{
	public:
		using InterfaceType = typename PropertyBase<Type, PropertyAccessorSaveType::Both>::InterfaceType;
		using GetterType = typename PropertyBase<Type, PropertyAccessorSaveType::Both>::GetterType;
		using SetterType = typename PropertyBase<Type, PropertyAccessorSaveType::Both>::SetterType;

		PropertyConstructor(GetterType getter, SetterType setter) : PropertyBase<Type, PropertyAccessorSaveType::Both>(getter, setter) {}
	};

	// Getter only
	template <class Type>
	class PropertyConstructor<Type, PropertyType::GetterOnly> : public PropertyBase<Type, PropertyAccessorSaveType::GetterOnly>
	{
	public:
		using InterfaceType = typename PropertyBase<Type, PropertyAccessorSaveType::GetterOnly>::InterfaceType;
		using GetterType = typename PropertyBase<Type, PropertyAccessorSaveType::GetterOnly>::GetterType;
		using SetterType = typename PropertyBase<Type, PropertyAccessorSaveType::GetterOnly>::SetterType;

		PropertyConstructor(GetterType getter) : PropertyBase<Type, PropertyAccessorSaveType::GetterOnly>(getter) {}
	};

	// Setter only
	template <class Type>
	class PropertyConstructor<Type, PropertyType::SetterOnly> : public PropertyBase<Type, PropertyAccessorSaveType::SetterOnly>
	{
	public:
		using InterfaceType = typename PropertyBase<Type, PropertyAccessorSaveType::SetterOnly>::InterfaceType;
		using GetterType = typename PropertyBase<Type, PropertyAccessorSaveType::SetterOnly>::GetterType;
		using SetterType = typename PropertyBase<Type, PropertyAccessorSaveType::SetterOnly>::SetterType;

		PropertyConstructor(SetterType setter) : PropertyBase<Type, PropertyAccessorSaveType::SetterOnly>(setter) {}
	};

	// Declare
	template <class OwnerClass, class Type, bool isGetterPrivate, bool isSetterPrivate, PropertyType PropType, bool isPointer = IsPointer<Type>::value>
	class Property;

	// Non-pointer specialization : Both public
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, false, false, PropType, false> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		InterfaceType operator+(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator+(rhs);
		}

		InterfaceType operator+(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator+(rhs);
		}

		InterfaceType operator-(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator-(rhs);
		}

		InterfaceType operator-(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator-(rhs);
		}

		InterfaceType operator*(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator*(rhs);
		}

		InterfaceType operator*(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator*(rhs);
		}

		InterfaceType operator/(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator/(rhs);
		}

		InterfaceType operator/(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator/(rhs);
		}

		InterfaceType operator+=(const Property& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator+=(rhs);
		}

		InterfaceType operator+=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator+=(rhs);
		}

		InterfaceType operator-=(const Property& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator-=(rhs);
		}

		InterfaceType operator-=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator-=(rhs);
		}

		InterfaceType operator*=(const Property& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator*=(rhs);
		}

		InterfaceType operator*=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator*=(rhs);
		}

		InterfaceType operator/=(const Property& rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator/=(rhs);
		}

		InterfaceType operator/=(InterfaceType rhs)
		{
			return PropertyBase<Type, PropertyAccessorSaveType::Both>::operator/=(rhs);
		}
	};

	// Non-pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, false, true, PropType, false> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

	private:
		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

	public:
		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		InterfaceType operator+(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator+(rhs);
		}

		InterfaceType operator+(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator+(rhs);
		}

		InterfaceType operator-(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator-(rhs);
		}

		InterfaceType operator-(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator-(rhs);
		}

		InterfaceType operator*(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator*(rhs);
		}

		InterfaceType operator*(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator*(rhs);
		}

		InterfaceType operator/(const Property& rhs) const
		{
			return PropertyGetterBase<Type>::operator/(rhs);
		}

		InterfaceType operator/(InterfaceType rhs) const
		{
			return PropertyGetterBase<Type>::operator/(rhs);
		}
	};

	// Non-pointer specialization : Setter public, Getter private
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, true, false, PropType, false> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

	private:
		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};

	// Non-pointer specialization : Both private
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, true, true, PropType, false> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

	private:
		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};

	// Pointer specialization : Both public
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, false, false, PropType, true> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};

	// Pointer specialization : Setter private, getter public
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, false, true, PropType, true> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

	private:
		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

	public:
		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};

	// Pointer specialization : Setter public, getter private
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, true, false, PropType, true> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

	private:
		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};

	// Pointer specialization : Both private
	template <class OwnerClass, class Type, PropertyType PropType>
	class Property<OwnerClass, Type, true, true, PropType, true> : public PropertyConstructor<Type, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

		using InterfaceType = typename Parent::InterfaceType;

	private:
		decltype(auto) get()
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) get() const
		{
			return PropertyGetterBase<Type>::get();
		}

		decltype(auto) operator=(InterfaceType rhs)
		{
			return PropertySetterBase<Type>::operator=(rhs);
		}

		operator InterfaceType()
		{
			return PropertyGetterBase<Type>::operator InterfaceType();
		}

		decltype(auto) operator*()
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->()
		{
			return PropertyGetterBase<Type>::operator->();
		}

		decltype(auto) operator*() const
		{
			return PropertyGetterBase<Type>::operator*();
		}

		decltype(auto) operator->() const
		{
			return PropertyGetterBase<Type>::operator->();
		}
	};
}