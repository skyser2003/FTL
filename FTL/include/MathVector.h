#pragma once

#include <cmath>

#include "Property.h"

namespace FTL
{
	template <class ValueType, int _Dimension>
	class Vector
	{
	public:
		static constexpr int Dimension = _Dimension;

		Property<Vector, ValueType, false, false, PropertyType::AutoGen> Value[Dimension];

		Property<Vector, float, false, true, PropertyType::GetterOnly> Magnitude
		{
			[this]()
			{
				float value = 0;

				for (int i = 0; i < Dimension; ++i)
				{
					value += Value[i] * Value[i];
				}

				value = std::sqrt(value);

				return value;
			}
		};

		Vector() {}
		Vector(const Vector& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] = rhs.Value[i].get();
			}
		}

		Vector& operator=(const Vector& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] = rhs.Value[i].get();
			}

			return *this;
		}

		Vector operator+(const Vector& rhs)
		{
			Vector ret;

			for (int i = 0; i < Dimension; ++i)
			{
				ret.Value[i] = Value[i] + rhs.Value[i];
			}

			return ret;
		}

		Vector operator-(const Vector& rhs)
		{
			Vector ret;

			for (int i = 0; i < Dimension; ++i)
			{
				ret.Value[i] = Value[i] - rhs.Value[i];
			}

			return ret;
		}

		template <class MultiplierType>
		Vector operator*(MultiplierType value)
		{
			Vector ret = *this;

			for (int i = 0; i < Dimension; ++i)
			{
				ret.Value[i] = Value[i] * value;
			}

			return ret;
		}

		template <class MultiplierType>
		Vector operator/(MultiplierType value)
		{
			Vector ret = *this;

			for (int i = 0; i < Dimension; ++i)
			{
				ret.Value[i] = Value[i] / value;
			}

			return ret;
		}

		Vector operator+=(const Vector& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] += rhs.Value[i];
			}

			return *this;
		}

		Vector operator-=(const Vector& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] -= rhs.Value[i];
			}

			return *this;
		}

		template <class MultiplierType>
		Vector operator*=(MultiplierType value)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] *= value;
			}

			return *this;
		}

		template <class MultiplierType>
		Vector operator/=(MultiplierType value)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] /= value;
			}

			return *this;
		}

		void Normalize()
		{
			float magnitude = Magnitude;

			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] /= magnitude;
			}
		}

		void Set(ValueType valueArray[Dimension])
		{
			for (int i = 0; i < Dimension; ++i)
			{
				Value[i] = valueArray[i];
			}
		}

		void Set(int index, ValueType value)
		{
			if (IsOutOfRange(index) == true)
			{
				return;
			}

			Value[index] = value;
		}

		ValueType Get(int index) const
		{
			if (IsOutOfRange(index) == true)
			{
				return ValueType();
			}

			return Value[index].get();
		}

		bool IsInRange(int index) const
		{
			return 0 <= index && index < Dimension;
		}

		bool IsOutOfRange(int index) const
		{
			return IsInRange(index) == false;
		}
	};

	template <class ValueType>
	class Vector2 : public Vector<ValueType, 2>
	{
	public:
		using Vector<ValueType, 2>::Dimension;

		Vector2(ValueType x, ValueType y)
		{
			Set(x, y);
		}

		Property<Vector2, ValueType, false, false, PropertyType::Manual> X
		{
			[this]() -> ValueType { return this->Value[0]; },
			[this](ValueType value) { this->Value[0] = value; }
		};

		Property<Vector2, ValueType, false, false, PropertyType::Manual> Y
		{
			[this]() -> ValueType { return this->Value[1]; },
			[this](ValueType value) { this->Value[1] = value; }
		};

		Vector2() {}

		Vector2(const Vector2& rhs) : Vector<ValueType, 2>(rhs) {}

		Vector2& operator=(const Vector2& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				this->Value[i] = rhs.Value[i].get();
			}

			return *this;
		}

		void Set(ValueType x, ValueType y)
		{
			X = x;
			Y = y;
		}
	};

	template <class ValueType>
	class Vector3 : public Vector<ValueType, 3>
	{
	public:
		using Vector<ValueType, 3>::Dimension;

		Vector3(ValueType x, ValueType y, ValueType z)
		{
			Set(x, y, z);
		}

		Property<Vector3, ValueType, false, false, PropertyType::Manual> X
		{
			[this]() -> ValueType { return this->Value[0]; },
			[this](ValueType value) { this->Value[0] = value; }
		};

		Property<Vector3, ValueType, false, false, PropertyType::Manual> Y
		{
			[this]() -> ValueType { return this->Value[1]; },
			[this](ValueType value) { this->Value[1] = value; }
		};

		Property<Vector3, ValueType, false, false, PropertyType::Manual> Z
		{
			[this]() -> ValueType { return this->Value[2]; },
			[this](ValueType value) { this->Value[2] = value; }
		};

		Vector3() {}

		Vector3(const Vector3& rhs) : Vector<ValueType, 3>(rhs) {}

		Vector3& operator=(const Vector3& rhs)
		{
			for (int i = 0; i < Dimension; ++i)
			{
				this->Value[i] = rhs.Value[i].get();
			}

			return *this;
		}

		void Set(ValueType x, ValueType y, ValueType z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	};

	template <template <class> class VectorOrigin, class ValueType>
	VectorOrigin<ValueType> operator+(const VectorOrigin<ValueType>& lhs, const VectorOrigin<ValueType>& rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			ret.Value[i] = lhs.Value[i] + rhs.Value[i];
		}

		return ret;
	}

	template <template <class> class VectorOrigin, class ValueType>
	VectorOrigin<ValueType> operator-(const VectorOrigin<ValueType>& lhs, const VectorOrigin<ValueType>& rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			ret.Value[i] = lhs.Value[i] - rhs.Value[i];
		}

		return ret;
	}

	template <template <class> class VectorOrigin, class ValueType, class MultiplierType>
	VectorOrigin<ValueType> operator*(const VectorOrigin<ValueType>& lhs, MultiplierType rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			ret.Value[i] = lhs.Value[i] * rhs;
		}

		return ret;
	}

	template <template <class> class VectorOrigin, class ValueType, class MultiplierType>
	VectorOrigin<ValueType> operator/(const VectorOrigin<ValueType>& lhs, MultiplierType rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			ret.Value[i] = lhs.Value[i] / rhs;
		}

		return ret;
	}

	template <template <class> class VectorOrigin, class ValueType>
	VectorOrigin<ValueType> operator+=(VectorOrigin<ValueType>& lhs, VectorOrigin<ValueType> rhs)
	{
		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			lhs.Value[i] += rhs.Value[i];
		}

		return lhs;
	}

	template <template <class> class VectorOrigin, class ValueType>
	VectorOrigin<ValueType> operator-=(VectorOrigin<ValueType>& lhs, VectorOrigin<ValueType> rhs)
	{
		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			lhs.Value[i] -= rhs.Value[i];
		}

		return lhs;
	}

	template <template <class> class VectorOrigin, class ValueType, class MultiplierType>
	VectorOrigin<ValueType> operator*=(VectorOrigin<ValueType>& lhs, MultiplierType rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			lhs.Value[i] *= rhs;
		}

		return lhs;
	}

	template <template <class> class VectorOrigin, class ValueType, class MultiplierType>
	VectorOrigin<ValueType> operator/=(VectorOrigin<ValueType>& lhs, MultiplierType rhs)
	{
		VectorOrigin<ValueType> ret;

		for (int i = 0; i < VectorOrigin<ValueType>::Dimension; ++i)
		{
			lhs.Value[i] /= rhs;
		}

		return lhs;
	}

	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
}