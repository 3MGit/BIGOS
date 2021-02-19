#pragma once
#include "mat4.h"

namespace BIGOS {
	namespace math {

		struct quat
		{
			float x, y, z, w;

			quat();
			quat(const quat& quaternion);
			quat(float x, float y, float z, float w);
			quat(const vec3& xyz, float w);
			quat(const vec4& vec);
			quat(float scalar);

			quat& operator=(const quat& quaternion);

			quat& SetXYZ(const vec3& vec);
			const vec3 GetXYZ() const;

			quat& SetElem(uint32_t idx, float value);
			float GetElem(uint32_t idx) const;
			vec3 GetAxis() const;
			vec3 ToEulerAngles() const;

			const quat operator+(const quat& quaternion) const;
			const quat operator-(const quat& quaternion) const;
			const quat operator*(const quat& quaternion) const;
			const quat operator*(float scalar) const;
			const quat operator/(float scalar) const;
			float operator[](uint32_t idx) const;

			quat& operator+=(const quat& quaternion)
			{
				*this = *this + quaternion;
				return *this;
			}

			quat& operator-=(const quat& quaternion)
			{
				*this = *this - quaternion;
				return *this;
			}

			quat& operator*=(const quat& quaternion)
			{
				*this = *this * quaternion;
				return *this;
			}

			quat& operator*=(float scalar)
			{
				*this = *this * scalar;
				return *this;
			}

			quat& operator/=(float scalar)
			{
				*this = *this / scalar;
				return *this;
			}

			const quat operator-() const;
			bool operator==(const quat& quaternion) const;
			bool operator!=(const quat& quaternion) const;
			static quat Identity();
			static quat FromEulerAngles(const vec3& angles);

			static vec3 Rotate(const quat& quaternion, const vec3& vec);

			static const quat Rotation(const vec3& unitVec0, const vec3& unitVec1);
			static const quat Rotation(float radians, const vec3& unitVec);

			static const quat RotationX(float radians)
			{
				float angle = radians * 0.5f;
				return quat(sin(angle), 0.0f, 0.0f, cos(angle));
			}

			static const quat RotationY(float radians)
			{
				float angle = radians * 0.5f;
				return quat(0.0f, sin(angle), 0.0f, cos(angle));
			}

			static const quat RotationZ(float radians)
			{
				float angle = radians * 0.5f;
				return quat(0.0f, 0.0f, sin(angle), cos(angle));
			}

			float Dot(const quat& other) const;
			quat Conjugate() const;

		};

	}
}