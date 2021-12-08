
#include "Mat4.h"
#include <string>
#include <stdio.h>

namespace mml
{

	Mat4::Mat4()
	{
		setDiagonal(0.0f);
	}
	Mat4::Mat4(float diag)
	{
		setDiagonal(diag);
	}
	Mat4::Mat4(const Mat4& mat)
	{
		memcpy(_m, mat._m, sizeof(float) * 16);
	}
	Mat4::Mat4(float values[])
	{
		// Convert to column major
		for (int j = 0; j < 4; j++)
			for (int i = 0; i < 4; i++)
				_m[j + i * 4] = values[i + j * 4];
	}

	void Mat4::setDiagonal(float val)
	{
		memset(_m, 0, sizeof(float) * 16);
		_m[0] = val;
		_m[5] = val;
		_m[10] = val;
		_m[15] = val;
	}

	void Mat4::setIdentity()
	{
		setDiagonal(1.0f);
	}

	void Mat4::print() const
	{
		for (int j = 0; j < 4; j++)
		{
			printf("[");
			for (int i = 0; i < 4; i++)
				printf("%.2f ", _m[j + i * 4]);
			printf("]\n");
		}
	}

	void Mat4::transpose()
	{
		float temp[16];
		memcpy(temp, _m, sizeof(float) * 16);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				_m[i + j * 4] = temp[j + i * 4];
	}

	/* This was copied from StackOverflow : https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
			comment on the site about this :
				"This was lifted from MESA implementation of the GLU library."
	*/
	void Mat4::inverse()
	{
		mml::Mat4 inverseMatrix;

		inverseMatrix._m[0] = _m[5] * _m[10] * _m[15] -
			_m[5] * _m[11] * _m[14] -
			_m[9] * _m[6] * _m[15] +
			_m[9] * _m[7] * _m[14] +
			_m[13] * _m[6] * _m[11] -
			_m[13] * _m[7] * _m[10];

		inverseMatrix._m[4] = -_m[4] * _m[10] * _m[15] +
			_m[4] * _m[11] * _m[14] +
			_m[8] * _m[6] * _m[15] -
			_m[8] * _m[7] * _m[14] -
			_m[12] * _m[6] * _m[11] +
			_m[12] * _m[7] * _m[10];

		inverseMatrix._m[8] = _m[4] * _m[9] * _m[15] -
			_m[4] * _m[11] * _m[13] -
			_m[8] * _m[5] * _m[15] +
			_m[8] * _m[7] * _m[13] +
			_m[12] * _m[5] * _m[11] -
			_m[12] * _m[7] * _m[9];

		inverseMatrix._m[12] = -_m[4] * _m[9] * _m[14] +
			_m[4] * _m[10] * _m[13] +
			_m[8] * _m[5] * _m[14] -
			_m[8] * _m[6] * _m[13] -
			_m[12] * _m[5] * _m[10] +
			_m[12] * _m[6] * _m[9];

		inverseMatrix._m[1] = -_m[1] * _m[10] * _m[15] +
			_m[1] * _m[11] * _m[14] +
			_m[9] * _m[2] * _m[15] -
			_m[9] * _m[3] * _m[14] -
			_m[13] * _m[2] * _m[11] +
			_m[13] * _m[3] * _m[10];

		inverseMatrix._m[5] = _m[0] * _m[10] * _m[15] -
			_m[0] * _m[11] * _m[14] -
			_m[8] * _m[2] * _m[15] +
			_m[8] * _m[3] * _m[14] +
			_m[12] * _m[2] * _m[11] -
			_m[12] * _m[3] * _m[10];

		inverseMatrix._m[9] = -_m[0] * _m[9] * _m[15] +
			_m[0] * _m[11] * _m[13] +
			_m[8] * _m[1] * _m[15] -
			_m[8] * _m[3] * _m[13] -
			_m[12] * _m[1] * _m[11] +
			_m[12] * _m[3] * _m[9];

		inverseMatrix._m[13] = _m[0] * _m[9] * _m[14] -
			_m[0] * _m[10] * _m[13] -
			_m[8] * _m[1] * _m[14] +
			_m[8] * _m[2] * _m[13] +
			_m[12] * _m[1] * _m[10] -
			_m[12] * _m[2] * _m[9];

		inverseMatrix._m[2] = _m[1] * _m[6] * _m[15] -
			_m[1] * _m[7] * _m[14] -
			_m[5] * _m[2] * _m[15] +
			_m[5] * _m[3] * _m[14] +
			_m[13] * _m[2] * _m[7] -
			_m[13] * _m[3] * _m[6];

		inverseMatrix._m[6] = -_m[0] * _m[6] * _m[15] +
			_m[0] * _m[7] * _m[14] +
			_m[4] * _m[2] * _m[15] -
			_m[4] * _m[3] * _m[14] -
			_m[12] * _m[2] * _m[7] +
			_m[12] * _m[3] * _m[6];

		inverseMatrix._m[10] = _m[0] * _m[5] * _m[15] -
			_m[0] * _m[7] * _m[13] -
			_m[4] * _m[1] * _m[15] +
			_m[4] * _m[3] * _m[13] +
			_m[12] * _m[1] * _m[7] -
			_m[12] * _m[3] * _m[5];

		inverseMatrix._m[14] = -_m[0] * _m[5] * _m[14] +
			_m[0] * _m[6] * _m[13] +
			_m[4] * _m[1] * _m[14] -
			_m[4] * _m[2] * _m[13] -
			_m[12] * _m[1] * _m[6] +
			_m[12] * _m[2] * _m[5];

		inverseMatrix._m[3] = -_m[1] * _m[6] * _m[11] +
			_m[1] * _m[7] * _m[10] +
			_m[5] * _m[2] * _m[11] -
			_m[5] * _m[3] * _m[10] -
			_m[9] * _m[2] * _m[7] +
			_m[9] * _m[3] * _m[6];

		inverseMatrix._m[7] = _m[0] * _m[6] * _m[11] -
			_m[0] * _m[7] * _m[10] -
			_m[4] * _m[2] * _m[11] +
			_m[4] * _m[3] * _m[10] +
			_m[8] * _m[2] * _m[7] -
			_m[8] * _m[3] * _m[6];

		inverseMatrix._m[11] = -_m[0] * _m[5] * _m[11] +
			_m[0] * _m[7] * _m[9] +
			_m[4] * _m[1] * _m[11] -
			_m[4] * _m[3] * _m[9] -
			_m[8] * _m[1] * _m[7] +
			_m[8] * _m[3] * _m[5];

		inverseMatrix._m[15] = _m[0] * _m[5] * _m[10] -
			_m[0] * _m[6] * _m[9] -
			_m[4] * _m[1] * _m[10] +
			_m[4] * _m[2] * _m[9] +
			_m[8] * _m[1] * _m[6] -
			_m[8] * _m[2] * _m[5];


		float determinant = _m[0] * inverseMatrix._m[0] + _m[1] * inverseMatrix._m[4] + _m[2] * inverseMatrix._m[8] + _m[3] * inverseMatrix._m[12];

		*this = inverseMatrix; // THIS MAY NOT WORK! <- wtf is going on here!?!? don't remember at all..

		if (determinant == 0)
			return;

		for (int i = 0; i < 16; i++)
			_m[i] *= (1.0f / determinant);
	}


	Mat4 operator*(const Mat4& left, const Mat4& right)
	{
		Mat4 result;

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				result._m[j + i * 4] =
					left[j + 0 * 4] * right[0 + i * 4] +
					left[j + 1 * 4] * right[1 + i * 4] +
					left[j + 2 * 4] * right[2 + i * 4] +
					left[j + 3 * 4] * right[3 + i * 4];
			}
		}

		return result;
	}
	void Mat4::operator*=(const Mat4& other)
	{
	}

	Mat4 operator*(const Mat4& left, float right)
	{
		return Mat4(1.0f);
	}
	void Mat4::operator*=(float val)
	{
	}

	const float& Mat4::operator[](int i) const
	{
		return _m[i];
	}
	float& Mat4::operator[](int i)
	{
		return _m[i];
	}
}