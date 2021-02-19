#include "bgspch.h"

#include "quaternion.h"
#include "math_func.h"

namespace BIGOS {
	namespace math {

#define _VECTORMATH_SLERP_TOL 0.999f

		quat::quat()
			: x(0), y(0), z(0), w(1)
		{
		}

		quat::quat(const quat& quaternion)
		{
			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
			w = quaternion.w;
		}

		quat::quat(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
		}

		quat::quat(const vec4& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			w = vec.w;
		}

		quat::quat(float scalar)
		{
			x = scalar;
			y = scalar;
			z = scalar;
			w = scalar;
		}

		quat::quat(const vec3& xyz, float w)
		{
			this->SetXYZ(xyz);
			this->w = w;
		}

		quat quat::Identity()
		{
			return quat(0.0f, 0.0f, 0.0f, 1.0f);
		}

		quat quat::FromEulerAngles(const vec3& angles)
		{
			quat pitch(vec3(1.0, 0.0, 0.0), angles.x);
			quat yaw(vec3(0.0, 1.0, 0.0), angles.y);
			quat roll(vec3(0.0, 0.0, 1.0), angles.z);
			return pitch * yaw * roll;
		}

		quat& quat::operator =(const quat& quaternion)
		{
			x = quaternion.x;
			y = quaternion.y;
			z = quaternion.z;
			w = quaternion.w;
			return *this;
		}

		quat& quat::SetXYZ(const vec3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}

		const vec3 quat::GetXYZ() const
		{
			return vec3(x, y, z);
		}

		quat& quat::SetElem(uint32_t idx, float value)
		{
			*(&x + idx) = value;
			return *this;
		}

		float quat::GetElem(uint32_t idx) const
		{
			return *(&x + idx);
		}

		float quat::operator [](uint32_t idx) const
		{
			return *(&x + idx);
		}

		vec3 quat::GetAxis() const
		{
			float x = 1.0f - w * w;
			if (x < 0.0000001f) // Divide by zero safety check
				return vec3::XAxis();

			float x2 = x * x;
			return GetXYZ() / x2;
		}

		vec3 quat::ToEulerAngles() const
		{
			return vec3(atan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z),
				atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z),
				asin(2 * x * y + 2 * z * w));
		}

		const quat quat::operator+(const quat& quaternion) const
		{
			return quat(x + quaternion.x, y + quaternion.y, z + quaternion.z, w + quaternion.w);
		}

		const quat quat::operator-(const quat& quaternion) const
		{
			return quat(x - quaternion.x, y - quaternion.y, z - quaternion.z, w - quaternion.w);
		}

		const quat quat::operator*(float scalar) const
		{
			return quat(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		const quat quat::operator/(float scalar) const
		{
			return quat(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		const quat quat::operator-() const
		{
			return quat(-x, -y, -z, -w);
		}

		bool quat::operator ==(const quat& quaternion) const
		{
			return (x == quaternion.x) && (y == quaternion.y) && (z == quaternion.z) && (w == quaternion.w);
		}

		bool quat::operator !=(const quat& quaternion) const
		{
			return !(*this == quaternion);
		}

		float Norm(const quat& quaternion)
		{
			float result;
			result = (quaternion.x * quaternion.x);
			result = (result + (quaternion.y * quaternion.y));
			result = (result + (quaternion.z * quaternion.z));
			result = (result + (quaternion.w * quaternion.w));
			return result;
		}

		float Length(const quat& quaternion)
		{
			return sqrt(Norm(quaternion));
		}

		const quat Normalize(const quat& quaternion)
		{
			float lenSqr, lenInv;
			lenSqr = Norm(quaternion);
			lenInv = rsqrt(lenSqr);
			return quaternion * lenInv;
		}

		const quat NormalizeEst(const quat& quaternion)
		{
			float lenSqr, lenInv;
			lenSqr = Norm(quaternion);
			lenInv = rsqrt(lenSqr);
			return quaternion * lenInv;
		}

		const quat quat::Rotation(const vec3& unitVec0, const vec3& unitVec1)
		{
			float cosHalfAngleX2, recipCosHalfAngleX2;
			cosHalfAngleX2 = sqrt((2.0f * (1.0f + unitVec0.Dot(unitVec1))));
			recipCosHalfAngleX2 = (1.0f / cosHalfAngleX2);
			return quat((unitVec0.Cross(unitVec1) * recipCosHalfAngleX2), (cosHalfAngleX2 * 0.5f));
		}

		const quat quat::Rotation(float radians, const vec3& unitVec)
		{
			float angle = radians * 0.5f;
			return quat((unitVec * sin(angle)), cos(angle));
		}

		const quat quat::operator*(const quat& quaternion) const
		{
			return Normalize(quat(
				(((w * quaternion.x) + (x * quaternion.w)) + (y * quaternion.z)) - (z * quaternion.y),
				(((w * quaternion.y) + (y * quaternion.w)) + (z * quaternion.x)) - (x * quaternion.z),
				(((w * quaternion.z) + (z * quaternion.w)) + (x * quaternion.y)) - (y * quaternion.x),
				(((w * quaternion.w) - (x * quaternion.x)) - (y * quaternion.y)) - (z * quaternion.z)
			));
		}

		vec3 quat::Rotate(const quat& quaternion, const vec3& vec)
		{
			float tmpX, tmpY, tmpZ, tmpW;
			tmpX = (((quaternion.w * vec.x) + (quaternion.y * vec.z)) - (quaternion.z * vec.y));
			tmpY = (((quaternion.w * vec.y) + (quaternion.z * vec.x)) - (quaternion.x * vec.z));
			tmpZ = (((quaternion.w * vec.z) + (quaternion.x * vec.y)) - (quaternion.y * vec.x));
			tmpW = (((quaternion.x * vec.x) + (quaternion.y * vec.y)) + (quaternion.z * vec.z));
			return vec3(
				((((tmpW * quaternion.x) + (tmpX * quaternion.w)) - (tmpY * quaternion.z)) + (tmpZ * quaternion.y)),
				((((tmpW * quaternion.y) + (tmpY * quaternion.w)) - (tmpZ * quaternion.x)) + (tmpX * quaternion.z)),
				((((tmpW * quaternion.z) + (tmpZ * quaternion.w)) - (tmpX * quaternion.y)) + (tmpY * quaternion.x))
			);
		}

		quat quat::Conjugate() const
		{
			return quat(-x, -y, -z, w);
		}

		const quat Select(const quat& quat0, const quat& quat1, bool select1)
		{
			return quat(select1 ? quat1.x : quat0.x, select1 ? quat1.y : quat0.y, select1 ? quat1.z : quat0.z, select1 ? quat1.w : quat0.w);
		}

		float quat::Dot(const quat& other) const
		{
			float result = (x * other.x);
			result = (result + (y * other.y));
			result = (result + (z * other.z));
			result = (result + (w * other.w));
			return result;
		}

	}
}