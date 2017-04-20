#pragma once

#include "IsPointer.h"
#include "InheritanceAccessibility.h"

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

	// Return type specialization
	template <class Type,
		bool isReference =
		std::is_fundamental_v<std::remove_reference_t<Type>> == false &&
		std::is_enum_v<Type> == false &&
		IsPointer<Type>::value == false>
	class PropertyInterfaceType;

	template <class _Type>
	class PropertyInterfaceType<_Type, false>
	{
	public:
		typedef std::remove_reference_t<_Type> Type;
	};

	template <class _Type>
	class PropertyInterfaceType<_Type, true>
	{
	public:
		typedef _Type& Type;
	};

	// Getter base class
	template <class Type, bool isPointer = IsPointer<Type>::value>
	class PropertyGetterBase;

	// Non pointer getter
	template <class Type>
	class PropertyGetterBase<Type, false>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;
		using GetterInterfaceType = const InterfaceType;

		using GetterType = std::function<GetterInterfaceType(void)>;

		PropertyGetterBase() = delete;
		PropertyGetterBase(const PropertyGetterBase& rhs) = delete;

		PropertyGetterBase(GetterType getter) : getter(getter) {}

		InterfaceType get() const
		{
			return getter();
		}

		operator InterfaceType() const
		{
			return get();
		}

		GetterInterfaceType operator+(const PropertyGetterBase& rhs) const
		{
			return get() + rhs.get();
		}

		GetterInterfaceType operator+(GetterInterfaceType rhs) const
		{
			return get() + rhs;
		}

		GetterInterfaceType operator-(const PropertyGetterBase& rhs) const
		{
			return get() - rhs.get();
		}

		GetterInterfaceType operator-(GetterInterfaceType rhs) const
		{
			return get() - rhs;
		}

		GetterInterfaceType operator*(const PropertyGetterBase& rhs) const
		{
			return get() * rhs.get();
		}

		GetterInterfaceType operator*(GetterInterfaceType rhs) const
		{
			return get() * rhs;
		}

		GetterInterfaceType operator/(const PropertyGetterBase& rhs) const
		{
			return get() / rhs.get();
		}

		GetterInterfaceType operator/(GetterInterfaceType rhs) const
		{
			return get() / rhs;
		}

	protected:
		const GetterType getter;
	};

	// Pointer getter
	template <class Type>
	class PropertyGetterBase<Type, true>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;
		using GetterInterfaceType = const InterfaceType;

		using GetterType = std::function<GetterInterfaceType(void)>;

		PropertyGetterBase() = delete;
		PropertyGetterBase(const PropertyGetterBase& rhs) = delete;

		PropertyGetterBase(GetterType getter) : getter(getter) {}

		GetterInterfaceType get() const
		{
			return getter();
		}

		const decltype(*Type()) getValue() const
		{
			return *getter();
		}

		GetterInterfaceType operator->()
		{
			return getter();
		}

		const GetterInterfaceType operator->() const
		{
			return getter();
		}

	protected:
		const GetterType getter;
	};

	// Setter base class
	template <class Type, bool isPointer = IsPointer<Type>::value>
	class PropertySetterBase;

	// Non pointer setter
	template <class Type>
	class PropertySetterBase<Type, false>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using SetterType = std::function<void(InterfaceType)>;

		PropertySetterBase() = delete;
		PropertySetterBase(const PropertySetterBase& rhs) = delete;

		PropertySetterBase(SetterType setter) : setter(setter) {}

		InterfaceType operator=(InterfaceType rhs)
		{
			setter(rhs);
			return rhs;
		}

	protected:
		const SetterType setter;
	};

	// Pointer setter
	template <class Type>
	class PropertySetterBase<Type, true>
	{
	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;
		using ValueInterfaceType = typename PropertyInterfaceType<decltype(*(Type()))>::Type;

		using SetterType = std::function<void(InterfaceType)>;
		using ValueSetterType = std::function<void(ValueInterfaceType)>;

		PropertySetterBase() = delete;
		PropertySetterBase(const PropertySetterBase& rhs) = delete;

		PropertySetterBase(SetterType setter, ValueSetterType valueSetter)
			: setter(setter), valueSetter(valueSetter) {}

		InterfaceType operator=(InterfaceType rhs)
		{
			setter(rhs);
			return rhs;
		}

		void setValue(ValueInterfaceType rhs)
		{
			valueSetter(rhs);
		}

	protected:
		const SetterType setter;
		const ValueSetterType valueSetter;
	};

	// Getter & Setter base class
	template <class Type, bool isGetterPrivate, bool isSetterPrivate, bool isPointer = IsPointer<Type>::value>
	class PropertyGetterSetterBase;
	
	// Non pointer specialization
	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate, false> :
		public InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>,
		public InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>
	{
	private:
		using GetterParent = InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>;
		using SetterParent = InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>;

	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;
		using GetterInterfaceType = typename PropertyGetterBase<Type>::GetterInterfaceType;

		using GetterType = typename PropertyGetterBase<Type>::GetterType;
		using SetterType = typename PropertySetterBase<Type>::SetterType;

		PropertyGetterSetterBase() = delete;
		PropertyGetterSetterBase(const PropertyGetterSetterBase& rhs) = delete;

		PropertyGetterSetterBase(GetterType getter, SetterType setter)
			: GetterParent(getter), SetterParent(setter) {}

		using SetterParent::operator=;

		InterfaceType operator+=(const PropertyGetterSetterBase& rhs)
		{
			return operator+=(rhs.get());
		}

		InterfaceType operator+=(InterfaceType rhs)
		{
			InterfaceType ret = get() + rhs;
			this->setter(ret);

			return ret;
		}

		InterfaceType operator-=(const PropertyGetterSetterBase& rhs)
		{
			return operator-=(rhs.get());
		}

		InterfaceType operator-=(InterfaceType rhs)
		{
			InterfaceType ret = get() - rhs;
			this->setter(ret);

			return ret;
		}

		InterfaceType operator*=(const PropertyGetterSetterBase& rhs)
		{
			return operator*=(rhs.get());
		}

		InterfaceType operator*=(InterfaceType rhs)
		{
			InterfaceType ret = get() * rhs;
			this->setter(ret);

			return ret;
		}

		InterfaceType operator/=(const PropertyGetterSetterBase& rhs)
		{
			return operator/=(rhs.get());
		}

		InterfaceType operator/=(InterfaceType rhs)
		{
			InterfaceType ret = get() / rhs;
			this->setter(ret);

			return ret;
		}
	};

	// Pointer specialization
	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate, true> :
		public InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>,
		public InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>
	{
	private:
		using GetterParent = InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>;
		using SetterParent = InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>;

	public:
		static const bool isPointer = IsPointer<Type>::value;

		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

		using GetterType = typename PropertyGetterBase<Type>::GetterType;
		using SetterType = typename PropertySetterBase<Type>::SetterType;
		using ValueSetterType = typename PropertySetterBase<Type>::ValueSetterType;

		PropertyGetterSetterBase() = delete;
		PropertyGetterSetterBase(const PropertyGetterSetterBase& rhs) = delete;

		PropertyGetterSetterBase(GetterType getter, SetterType setter, ValueSetterType valueSetter)
			: GetterParent(getter), SetterParent(setter, valueSetter) {}

		using SetterParent::operator=;
	};

	// Default getter & setter
	template <class Type, bool isPointer = IsPointer<Type>::value>
	class AutoGeneratedGetterSetter;

	// Non pointer specialization
	template <class Type>
	class AutoGeneratedGetterSetter<Type, false>
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;

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

	// Pointer specialization
	template <class Type>
	class AutoGeneratedGetterSetter<Type, true>
	{
	public:
		using InterfaceType = typename PropertyInterfaceType<Type>::Type;
		using ValueInterfaceType = typename PropertySetterBase<Type>::ValueInterfaceType;

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

		void setValue(ValueInterfaceType rhs)
		{
			*value = rhs;
		}

	private:
		Type value;
	};

	// Autogen constructor
	template <class Type, bool isGetterPrivate, bool isSetterPrivate, bool IsPointer = IsPointer<Type>::value>
	class PropertyConstructorAutogen;

	// Non pointer specialization
	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructorAutogen<Type, isGetterPrivate, isSetterPrivate, false>
		: public PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>
	{
	public:
		using Parent = PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>;
		using InterfaceType = typename Parent::InterfaceType;

	private:
		using AutoGen = AutoGeneratedGetterSetter<Type>;

	public:
		PropertyConstructorAutogen() :
			Parent
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1)
			) {}

		explicit PropertyConstructorAutogen(InterfaceType value) : autoGen(value),
			Parent
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1)
			) {}

		using Parent::operator=;

	private:
		AutoGen autoGen;
	};

	// Pointer specialization
	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructorAutogen<Type, isGetterPrivate, isSetterPrivate, true>
		: public PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>
	{
	public:
		using Parent = PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>;
		using InterfaceType = typename Parent::InterfaceType;

	private:
		using AutoGen = AutoGeneratedGetterSetter<Type>;

	public:
		PropertyConstructorAutogen() :
			Parent
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1),
				std::bind(&AutoGen::setValue, &this->autoGen, std::placeholders::_1)
			) {}

		explicit PropertyConstructorAutogen(InterfaceType value) : autoGen(value),
			Parent
			(
				std::bind(&AutoGen::get, &this->autoGen),
				std::bind(&AutoGen::set, &this->autoGen, std::placeholders::_1),
				std::bind(&AutoGen::setValue, &this->autoGen, std::placeholders::_1)
			) {}

		using Parent::operator=;

	private:
		AutoGen autoGen;
	};

	// Constructor
	template <class Type, bool isGetterPrivate, bool isSetterPrivate, PropertyType PropType>
	class PropertyConstructor;

	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructor<Type, isGetterPrivate, isSetterPrivate, PropertyType::GetterOnly>
		: public InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>
	{
	public:
		using Parent = InheritanceAccessiblity<!isGetterPrivate, PropertyGetterBase<Type>>;

		using Parent::Parent;
	};

	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructor<Type, isGetterPrivate, isSetterPrivate, PropertyType::SetterOnly>
		: public InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>
	{
	public:
		using Parent = InheritanceAccessiblity<!isSetterPrivate, PropertySetterBase<Type>>;

		using Parent::Parent;
		using Parent::operator=;
	};

	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructor<Type, isGetterPrivate, isSetterPrivate, PropertyType::Manual>
		: public PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>
	{
	public:
		using Parent = PropertyGetterSetterBase<Type, isGetterPrivate, isSetterPrivate>;

		using Parent::Parent;
		using Parent::operator=;
	};

	template <class Type, bool isGetterPrivate, bool isSetterPrivate>
	class PropertyConstructor<Type, isGetterPrivate, isSetterPrivate, PropertyType::AutoGen>
		: public PropertyConstructorAutogen<Type, isGetterPrivate, isSetterPrivate>
	{
	public:
		using Parent = PropertyConstructorAutogen<Type, isGetterPrivate, isSetterPrivate>;

		using Parent::Parent;
		using Parent::operator=;
	};

	// Property declare
	template <class OwnerClass, class Type, bool isGetterPrivate, bool isSetterPrivate, PropertyType PropType>
	class Property;

	// Setter public
	template <class OwnerClass, class Type, bool isGetterPrivate, PropertyType PropType>
	class Property<OwnerClass, Type, isGetterPrivate, false, PropType>
		: public PropertyConstructor<Type, isGetterPrivate, false, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, isGetterPrivate, false, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;
		using Parent::operator=;
	};

	// Setter private
	template <class OwnerClass, class Type, bool isGetterPrivate, PropertyType PropType>
	class Property<OwnerClass, Type, isGetterPrivate, true, PropType>
		: public PropertyConstructor<Type, isGetterPrivate, true, PropType>
	{
	private:
		using Parent = PropertyConstructor<Type, isGetterPrivate, true, PropType>;

	public:
		friend OwnerClass;

		using Parent::Parent;

	private:
		using Parent::operator=;
	};
}