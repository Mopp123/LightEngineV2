
#include "Maths.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

namespace mml
{
	float to_radians(float degrees)
	{
		return 1.0f / ((180.0f / degrees) / M_PI);
	}


	float lerp(const float& v1, const float& v2, float amount)
	{
		return v1 + (v2 - v1) * amount;
	}
	Vec3 lerp(const Vec3& a, const Vec3& b, float amount)
	{
		return a + (b - a) * amount;
	}
	Vec4 lerp(const Vec4& a, const Vec4& b, float amount)
	{
		return a + (b - a) * amount;
	}

	// Copied from wikipedia : https://en.wikipedia.org/wiki/Slerp
	#define QUATERNION_SLERP__DOT_THRESHOLD 0.9995f
	Quat slerp(const Quat& q1, const Quat& q2, float a)
	{
		Quat v0 = q1;
		Quat v1 = q2;

		// Only unit quaternions are valid rotations.
		// Normalize to avoid undefined behavior.
		v0.normalize();
		v1.normalize();

		// Compute the cosine of the angle between the two vectors.
		float dot = v0.dot(v1);

		// If the dot product is negative, slerp won't take
		// the shorter path. Note that v1 and -v1 are equivalent when
		// the negation is applied to all four components. Fix by 
		// reversing one quaternion.
		if (dot < 0.0f) {
			v1 = v1 * -1.0f;
			dot = -dot;
		}

		if (dot > QUATERNION_SLERP__DOT_THRESHOLD) {
			// If the inputs are too close for comfort, linearly interpolate
			// and normalize the result.
			Quat result = v0 + ((v1 - v0) * a);
			result.normalize();
			return result;
		}

		// Since dot is in range [0, DOT_THRESHOLD], acos is safe
		float theta_0 = acos(dot);        // theta_0 = angle between input vectors
		float theta = theta_0 * a;        // theta = angle between v0 and result
		float sin_theta = sin(theta);     // compute this value only once
		float sin_theta_0 = sin(theta_0); // compute this value only once

		float s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
		float s1 = sin_theta / sin_theta_0;

		return (v0 * s0) + (v1 * s1);
	}




	// Returns next closest power of 2 value from v
	unsigned int get_next_pow2(unsigned int v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}

	float get_signf(float val) {
		return (0.0f < val) - (val < 0.0f);
	}


	Mat4 create_projection_matrix__perspective(
		float aspectRatio,
		float fov,
		float zNear, float zFar
	)
	{
		Mat4 matrix(1.0f);
		matrix[0] = 1.0f / (aspectRatio * tan(fov / 2.0f));
		matrix[1 + 1 * 4] = 1.0f / (tan(fov / 2.0f));
		matrix[2 + 2 * 4] = -((zFar + zNear) / (zFar - zNear));
		matrix[3 + 2 * 4] = -1.0f;
		matrix[2 + 3 * 4] = -((2.0f * zFar * zNear) / (zFar - zNear));
		matrix[3 + 3 * 4] = 0.0f;
		return matrix;
	}

	Mat4 create_projection_matrix__orthographic(
		float left, float right,
		float top, float bottom,
		float zNear, float zFar
	)
	{
		Mat4 matrix(1.0f);

		matrix[0] = 2.0f / (right - left);
		matrix[1 + 1 * 4] = 2.0f / (top - bottom);
		matrix[2 + 2 * 4] = -2.0f / (zFar - zNear);
		matrix[3 + 3 * 4] = 1.0f;
		matrix[0 + 3 * 4] = -((right + left) / (right - left));
		matrix[1 + 3 * 4] = -((top + bottom) / (top - bottom));
		matrix[2 + 3 * 4] = -((zFar + zNear) / (zFar - zNear));

		return matrix;
	}

	Mat4 create_translation_matrix(const Vec3& translation)
	{
		Mat4 mat(1.0f);
		mat[0 + 3 * 4] = translation.x;
		mat[1 + 3 * 4] = translation.y;
		mat[2 + 3 * 4] = translation.z;
		return mat;
	}
	Mat4 create_rotation_matrix__euler_x(const float& angle)
	{
		Mat4 mat(1.0f);

		mat[1 + 1 * 4] = cos(angle);
		mat[1 + 2 * 4] = -sin(angle);
		mat[2 + 1 * 4] = sin(angle);
		mat[2 + 2 * 4] = cos(angle);

		return mat;
	}
	Mat4 create_rotation_matrix__euler_y(const float& angle)
	{
		Mat4 mat(1.0f);

		mat[0 + 0 * 4] = cos(angle);
		mat[0 + 2 * 4] = sin(angle);
		mat[2 + 0 * 4] = -sin(angle);
		mat[2 + 2 * 4] = cos(angle);

		return mat;
	}
	Mat4 create_rotation_matrix__euler_z(const float& angle)
	{
		Mat4 mat(1.0f);

		mat[0 + 0 * 4] = cos(angle);
		mat[0 + 1 * 4] = -sin(angle);
		mat[1 + 0 * 4] = sin(angle);
		mat[1 + 1 * 4] = cos(angle);

		return mat;
	}
	Mat4 create_scale_matrix(const Vec3& scale)
	{
		Mat4 mat(1.0f);
		mat[0 + 0 * 4] = scale.x;
		mat[1 + 1 * 4] = scale.y;
		mat[2 + 2 * 4] = scale.z;
		return mat;
	}
	Mat4 create_transformation_matrix(const Vec3& pos, const Vec3& eulerAngles, const Vec3& scale)
	{
		Mat4 translationMatrix = create_translation_matrix(pos);
		Mat4 rotMatX = create_rotation_matrix__euler_x(eulerAngles.x);
		Mat4 rotMatY = create_rotation_matrix__euler_y(eulerAngles.y);
		Mat4 rotMatZ = create_rotation_matrix__euler_z(eulerAngles.z);
		Mat4 scaleMatrix = create_scale_matrix(scale);

		return translationMatrix * rotMatX * rotMatY * rotMatZ * scaleMatrix; // Order here may be wrong!
	}

	void update_translation_matrix(Mat4& matrix, const Vec3& translation)
	{
		matrix = matrix * create_translation_matrix(translation);
	}
	void update_rotation_matrix__euler_x(Mat4& matrix, const float& angle)
	{
		matrix = matrix * create_rotation_matrix__euler_x(angle);
	}
	void update_rotation_matrix__euler_y(Mat4& matrix, const float& angle)
	{
		matrix = matrix * create_rotation_matrix__euler_y(angle);
	}
	void update_rotation_matrix__euler_z(Mat4& matrix, const float& angle)
	{
		matrix = matrix * create_rotation_matrix__euler_z(angle);
	}
	void update_scale_matrix(Mat4& matrix, const Vec3& scale)
	{
		matrix = matrix * create_scale_matrix(scale);
	}
	void update_transformation_matrix(Mat4& matrix, const Vec3& pos, const Vec3& eulerAngles, const Vec3& scale)
	{
		matrix.setIdentity();
		update_translation_matrix(matrix, pos);

		update_rotation_matrix__euler_x(matrix, eulerAngles.x);
		update_rotation_matrix__euler_y(matrix, eulerAngles.y);
		update_rotation_matrix__euler_z(matrix, eulerAngles.z);

		update_scale_matrix(matrix, scale);
	}

	void update_transformation_matrix(Mat4& matrix, const Vec3& pos, const Quat& rotation, const Vec3& scale)
	{
		// ..Its not possible to do this shit with less operations.. This should be the most optimal way..
		matrix.setIdentity();
		const Quat& q = rotation;

		Mat4 tm = create_translation_matrix(pos);
		Mat4 sm = create_scale_matrix(scale);
		Mat4 rm = q.toRotationMatrix();

		matrix = tm * rm * sm;

		//q.normalizeFast(); // We propably should force the usage of unit quaternion here...?
		/*
		const float s = 1.0f;

		const float sqx = q.x * q.x;
		const float sqy = q.y * q.y;
		const float sqz = q.z * q.z;

		matrix[0] = (1.0f - 2 * s * (sqy + sqz)) * scale.x;
		matrix[4] = (2 * s * (q.x * q.y - q.z * q.w)) * scale.y;
		matrix[8] = (2 * s * (q.x * q.z + q.y * q.w)) * scale.z;

		matrix[1] = (2 * s * (q.x * q.y + q.z * q.w)) * scale.x;
		matrix[5] = (1.0f - 2 * s * (sqx + sqz)) * scale.y;
		matrix[9] = (2 * s * (q.y * q.z - q.x * q.w)) * scale.z;

		matrix[2] = (2 * s * (q.x * q.z - q.y * q.w)) * scale.x;
		matrix[6] = (2 * s * (q.y * q.z + q.x * q.w)) * scale.y;
		matrix[10] = (1.0f - 2 * s * (sqx + sqy)) * scale.z;

		matrix[12] = pos.x;
		matrix[13] = pos.y;
		matrix[14] = pos.z;*/
	}


	Mat4 create_view_matrix(const Vec3& viewPos, const Vec3& viewRotation)
	{
		Mat4 rotMatX = create_rotation_matrix__euler_x(viewRotation.x);
		Mat4 rotMatY = create_rotation_matrix__euler_y(viewRotation.y);
		Mat4 rotMatZ = create_rotation_matrix__euler_z(viewRotation.z);

		Mat4 negTranslationMatrix = create_translation_matrix(viewPos * -1.0f);

		return rotMatX * rotMatY * rotMatZ * negTranslationMatrix; // Order here may be wrong!
	}
	void update_view_matrix(Mat4& matrix, const Vec3& viewPos, const Quat& rotation)
	{
		matrix.setIdentity();
		matrix = matrix * rotation.conjugate().toRotationMatrix(); // *Not sure why rotation is conjugate here!!???
		update_translation_matrix(matrix, Vec3(-viewPos.x, -viewPos.y, -viewPos.z));
	}

	void disassemble_transformation_matrix(const Mat4& matrix, Vec3& outPos, Quat& outRot, Vec3& outScale)
	{
		Mat4 temp = matrix;

		// Position
		outPos.x = temp[0 + 3 * 4];
		outPos.y = temp[1 + 3 * 4];
		outPos.z = temp[2 + 3 * 4];
		temp[0 + 3 * 4] = 0.0f;
		temp[1 + 3 * 4] = 0.0f;
		temp[2 + 3 * 4] = 0.0f;
		// Scale
		outScale.x = Vec3(temp[0 + 0 * 4], temp[1 + 0 * 4], temp[2 + 0 * 4]).magnitude();
		outScale.y = Vec3(temp[0 + 1 * 4], temp[1 + 1 * 4], temp[2 + 1 * 4]).magnitude();
		outScale.z = Vec3(temp[0 + 2 * 4], temp[1 + 2 * 4], temp[2 + 2 * 4]).magnitude();

		// rotation matrix..
		temp[0 + 0 * 4] /= outScale.x;
		temp[1 + 0 * 4] /= outScale.x;
		temp[2 + 0 * 4] /= outScale.x;

		temp[0 + 1 * 4] /= outScale.y;
		temp[1 + 1 * 4] /= outScale.y;
		temp[2 + 1 * 4] /= outScale.y;

		temp[0 + 2 * 4] /= outScale.z;
		temp[1 + 2 * 4] /= outScale.z;
		temp[2 + 2 * 4] /= outScale.z;

		// Finally get the rotation as quaternion from the rotation matrix
		outRot = extract_quat_from_rot_matrix(temp);
	}

	// Extracts quaternion from rotation matrix
	//		* Copied from site : https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quat extract_quat_from_rot_matrix(const Mat4& matrix)
	{
		Mat4 m = matrix;
		Quat q;
		q.w = sqrtf(std::max(0.0f, 1 + m[0 + 0 * 4] + m[1 + 1 * 4] + m[2 + 2 * 4])) / 2;
		q.x = sqrtf(std::max(0.0f, 1 + m[0] - m[1 + 1 * 4] - m[2 + 2 * 4])) / 2;
		q.y = sqrtf(std::max(0.0f, 1 - m[0] + m[1 + 1 * 4] - m[2 + 2 * 4])) / 2;
		q.z = sqrtf(std::max(0.0f, 1 - m[0] - m[1 + 1 * 4] + m[2 + 2 * 4])) / 2;
		q.x *= get_signf(q.x * (m[2 + 1 * 4] - m[1 + 2 * 4]));
		q.y *= get_signf(q.y * (m[0 + 2 * 4] - m[2 + 0 * 4]));
		q.z *= get_signf(q.z * (m[1 + 0 * 4] - m[0 + 1 * 4]));
		return q;
	}

}