#pragma once


namespace mml
{
	class Mat4
	{
	private:

		float _m[16] = { 0.0f };

	public:
	
		Mat4();
		Mat4(float diag);
		Mat4(const Mat4& mat);

		// U can give the array nicely in "normal form"!
		// This converts it into column major order.
		Mat4(float values[]);

		void setDiagonal(float val);
		void setIdentity();
		void print() const;
		void transpose();
		
		/* This was copied from StackOverflow : https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
			comment on the site about this :
				"This was lifted from MESA implementation of the GLU library."
		*/
		void inverse();
		

		friend Mat4 operator*(const Mat4& left, const Mat4& right);
		void operator*=(const Mat4& other);
		
		friend Mat4 operator*(const Mat4& left, float right);
		void operator*=(float val);

		const float& operator[](int i) const;
		float& operator[](int i); // *NOTE Don't know if this is legal to have const and non const versions of the same operator's overloading..


		inline float* getRawArray() { return _m; }
		inline const float* getRawArray() const { return _m; }

	};
}