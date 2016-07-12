#pragma once

#include "Property.h"

namespace FTL
{
	template <class ValueType, int Dimension>
	class Vector
	{
	public:
		Property<Vector, ValueType, false, false, PropertyType::AutoGen> Value[Dimension];

		Property<Vector, float, false, true, PropertyType::Manual> Magnitude
		{
			[this]()
			{
				float value = 0;

				for (int i = 0; i < Dimension; ++i)
				{
					value += Value[i] * Value[i];
				}

				return value;
			},
			[this](float value)
			{
				// Nothing
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
		Property<Vector2, ValueType, false, false, PropertyType::Manual> X
		{
			[this]() -> ValueType { return Value[0]; },
			[this](ValueType value) { Value[0] = value; }
		};

		Property<Vector2, ValueType, false, false, PropertyType::Manual> Y
		{
			[this]() -> ValueType { return Value[1]; },
			[this](ValueType value) { Value[1] = value; }
		};

		Vector2() {}

		Vector2(const Vector2& rhs) : Vector<ValueType, 2>(rhs) {}

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
		Property<Vector3, ValueType, false, false, PropertyType::Manual> X
		{
			[this]() -> ValueType { return Value[0]; },
			[this](ValueType value) { Value[0] = value; }
		};

		Property<Vector3, ValueType, false, false, PropertyType::Manual> Y
		{
			[this]() -> ValueType { return Value[1]; },
			[this](ValueType value) { Value[1] = value; }
		};

		Property<Vector3, ValueType, false, false, PropertyType::Manual> Z
		{
			[this]() -> ValueType { return Value[2]; },
			[this](ValueType value) { Value[2] = value; }
		};

		Vector3() {}

		Vector3(const Vector3& rhs) : Vector<ValueType, 3>(rhs) {}

		void Set(ValueType x, ValueType y, ValueType z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	};

	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
}