#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Quat.h"


namespace mml
{
	float to_radians(float degrees);
	float lerp(const float& v1, const float& v2, float amount);
	Vec3 lerp(const Vec3& a, const Vec3& b, float amount);
	Vec4 lerp(const Vec4& a, const Vec4& b, float amount);
	Quat slerp(const Quat& q1, const Quat& q2, float a);

	// Returns next closest power of 2 value from v
	unsigned int get_next_pow2(unsigned int v);

	// Returns sign of val
	float get_signf(float val);

	Mat4 create_projection_matrix__perspective(
		float aspectRatio,
		float fov,
		float zNear, float zFar
	);

	Mat4 create_projection_matrix__orthographic(
		float left, float right,
		float top, float bottom,
		float zNear, float zFar
	);


	Mat4 create_translation_matrix(const Vec3& translation);
	Mat4 create_rotation_matrix__euler_x(const float& angle);
	Mat4 create_rotation_matrix__euler_y(const float& angle);
	Mat4 create_rotation_matrix__euler_z(const float& angle);
	Mat4 create_scale_matrix(const Vec3& scale);

	Mat4 create_transformation_matrix(const Vec3& pos, const Vec3& eulerAngles, const Vec3& scale);

	void update_translation_matrix(Mat4& matrix, const Vec3& translation);
	void update_rotation_matrix__euler_x(Mat4& matrix, const float& angle);
	void update_rotation_matrix__euler_y(Mat4& matrix, const float& angle);
	void update_rotation_matrix__euler_z(Mat4& matrix, const float& angle);
	void update_scale_matrix(Mat4& matrix, const Vec3& scale);
	void update_transformation_matrix(Mat4& matrix, const Vec3& pos, const Vec3& eulerAngles, const Vec3& scale);

	void update_transformation_matrix(Mat4& matrix, const Vec3& pos, const Quat& rotation, const Vec3& scale);


	Mat4 create_view_matrix(const Vec3& viewPos, const Vec3& viewRotation);
	void update_view_matrix(Mat4& matrix, const Vec3& viewPos, const Quat& rotation);
	

	


	void disassemble_transformation_matrix(const Mat4& matrix, Vec3& outPos, Quat& outRot, Vec3& outScale);

	// Extracts quaternion from rotation matrix
	//		* Copied from site : https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	Quat extract_quat_from_rot_matrix(const Mat4& matrix);
}