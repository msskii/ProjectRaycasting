//
//  Matrix.hpp
//  Dimension++
//
//  Created by Aaron Hodel on 29.01.19.
//  Copyright © 2019 Aaron Hodel. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include "Vector.hpp"

namespace math {
    
    template <int M, int N>
    class Matrix {
    public:
        float *values = new float[N * M];

    public:
        Matrix();
        Matrix(float filler);
        Matrix(float *values);
        Matrix(Matrix<M, N> const& other) { values = new float[N * M]; for(int i = 0; i < M * N; i++) values[i] = other.values[i]; }
		Matrix(Vector<M> v);
        ~Matrix() { delete[] values; }
        
        inline void print() {
            printf("Matrix (%d|%d)\n[\n", M, N);
            for(int i = 0; i < M; i++) {
                for(int j = 0; j < N; j++) {
                    printf(j == N-1 ? "\t%f" : "\t%f, ", values[i * N + j]);
                }
                printf("\n");
            }
            printf("]\n");
        }
        
        inline float &operator()(int y, int x) { return values[y * N + x]; }
        inline float &operator[](int index) { return values[index]; }

		Matrix<N, M> transpose();
		float det();
		Matrix<M, N> inv();
        
        inline Vector<N> getRow(int row) { return Vector<N>(&values[N * row]); }
        inline Vector<M> getColumn(int column) { float *d = new float[M]; for(int i = 0; i < M; i++) d[i] = values[N * i + column]; return Vector<M>(d); }

        inline bool operator==(Matrix<M, N> other) { for(int i = 0; i < M * N; i++) if(other.values[i] != values[i]) return false; return true; }
        inline bool operator!=(Matrix<M, N> other) { for(int i = 0; i < M * N; i++) if(other.values[i] != values[i]) return true; return false; }

        inline bool operator==(float value) { for(int i = 0; i < M * N; i++) if(values[i] != (((int)(i/N)==(i%N)) ? value : 0)) return false; return true; }
        inline bool operator!=(float value) { for(int i = 0; i < M * N; i++) if(values[i] != (((int)(i/N)==(i%N)) ? value : 0)) return true; return false; }

        template <int T>
        Matrix<M, T> operator*(Matrix<N, T> other);
        Vector<M> operator*(Vector<N> other);

        
		Matrix<M, N> operator*(float scalar);
		Matrix<M, N> operator*=(float scalar);
		Matrix<M, N> operator/(float scalar);
		Matrix<M, N> operator/=(float scalar);
	};
}

template <int M, int N>
math::Matrix<M, N>::Matrix() {
    for(int i = 0; i < M * N; i++) values[i] = 0;
}

template <int M, int N>
math::Matrix<M, N>::Matrix(float filler) {
    for(int i = 0; i < M * N; i++) values[i] = filler;
}

template <int M, int N>
math::Matrix<M, N>::Matrix(float *v) {
    for(int i = 0; i < M * N; i++) values[i] = v[i];
}

template <int M, int N>
math::Matrix<M, N>::Matrix(Vector<M> v) {
	for (int i = 0; i < M * N; i++) values[i] = v[i / N];
}

template <int M, int N>
template <int T>
inline math::Matrix<M, T> math::Matrix<M, N>::operator*(Matrix<N, T> other) {
    math::Matrix<M, T> result;
    for(int i = 0; i < M * T; i++) for(int j = 0; j < N; j++) result.values[i] += other.values[i % T + j * T] * values[(i / T) * N + j];
    return result;
}

template <int M, int N>
inline math::Vector<M> math::Matrix<M, N>::operator*(Vector<N> other) {
    math::Vector<M> result;
    for(int i = 0; i < M; i++) for(int j = 0; j < N; j++) result[i] += values[i * N + j] * other[j];
    return result;
}


template <int M, int N>
math::Matrix<M, N> math::Matrix<M, N>::operator*(float scalar) {
	Matrix<M, N> cpy = math::Matrix<M, N>(values);
	for (int i = 0; i < M * N; i++) cpy.values[i] *= scalar;
	return cpy;
}

template <int M, int N>
math::Matrix<M, N> math::Matrix<M, N>::operator*=(float scalar) {
	for (int i = 0; i < M * N; i++) values[i] *= scalar;
	return *this;
}

template <int M, int N>
math::Matrix<M, N> math::Matrix<M, N>::operator/(float scalar) {
	Matrix<M, N> cpy = math::Matrix<M, N>(values);
	for (int i = 0; i < M * N; i++) cpy.values[i] /= scalar;
	return cpy;
}

template <int M, int N>
math::Matrix<M, N> math::Matrix<M, N>::operator/=(float scalar) {
	for (int i = 0; i < M * N; i++) values[i] /= scalar;
	return *this;
}

template <int M, int N>
inline math::Matrix<N, M> math::Matrix<M, N>::transpose() {
	Matrix<N, M> n = Matrix<N, M>();
	for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) n.values[i + j * M] = values[i * N + j];
	return n;
}

template <int M, int N>
inline float math::Matrix<M, N>::det() { // Calculate the determinant recursive until we have a 2 by 2 sub-matrix
	if (M != N) {
		printf("[ERROR] Trying to calculate determinant of no square matrix!\n");
		return 0;
	}

	bool negative = false;
	float det = 0;
	for (int i = 0; i < M; i++) {
		float *v = new float[(M - 1) * (M - 1)];
		int index = 0;
		for (int j = 0; j < M; j++) {
			if (j == i) continue;
			for (int k = 0; k < M - 1; k++) {
				v[index++] = values[(k + 1) * M + j];
			}
		}
		Matrix<M - 1, M - 1> sub = Matrix<M - 1, M - 1>(v);
		delete[] v;

		float x = values[i] * sub.det();
		if (negative) det -= x;
		else det += x;
		negative = !negative;
	}
	return det;
}

template <>
inline float math::Matrix<2, 2>::det() { // "Exit-Condition" for the recursive determinant calculation
	return values[0] * values[3] - values[1] * values[2];
}

template <>
inline float math::Matrix<1, 1>::det() { // Security, Don't want to crash (or not compile) without any error
	return values[0];
}

template <int M, int N>
inline math::Matrix<M, N> math::Matrix<M, N>::inv() {
	if (M != N) {
		printf("[ERROR] Trying to get inverse of not square matrix!\n");
		return *this;
	}

	Matrix<M, M> minors = Matrix<M, M>();
	float *v = new float[(M - 1) * (M - 1)]; // Create sub-matrix values
	bool negative = false;
	for (int i = 0; i < M * M; i++) {
		int index = 0;
		for (int j = 0; j < M; j++) {
			if (j == (i % M)) continue;
			for (int k = 0; k < M; k++) {
				if (k == (int)(i / M)) continue;
				v[index++] = values[k * M + j];
			}
		}
		Matrix<M - 1, M - 1> sub = Matrix<M - 1, M - 1>(v);
		minors.values[i] = sub.det() * (negative ? -1 : 1);
		negative = !negative;
	}
	delete[] v;

	float det = 0;
	for (int i = 0; i < M; i++) det += minors.values[i] * values[i];
	minors = minors.transpose();

	return minors / det;
}

// Util functions to create often used matrices

namespace math {
    template <int D>
    static math::Matrix<D, D> diagonal(float diagonal) {
        math::Matrix<D, D> m;
        for(int i = 0; i < D; i++) m(i, i) = diagonal;
        return m;
    }
    
    static math::Matrix<2, 2> rotate2(float angle) {
        return Matrix<2, 2>(new float[4] { cos(angle), -sin(angle), sin(angle), cos(angle) });
    }

	static math::Matrix<3, 3> rotate3_z(float angle) {
		return Matrix<3, 3>(new float[9]{
			cos(angle), -sin(angle), 0,
			sin(angle), cos(angle), 0,
			0, 0, 1
		});
	}

	static math::Matrix<3, 3> rotate3_y(float angle) {
		return Matrix<3, 3>(new float[9]{
			cos(angle), 0, sin(angle),
			0, 1, 0,
			-sin(angle), 0, cos(angle)
		});
	}

	static math::Matrix<3, 3> rotate3_x(float angle) {
		return Matrix<3, 3>(new float[9]{
			1, 0, 0,
			0, cos(angle), -sin(angle),
			0, sin(angle), cos(angle)
		});
	}

	static math::Matrix<4, 4> gl_4d(Matrix<3, 3> d) { // Convert 3d Matrix to 4d by adding 0s & 1s
		return Matrix<4, 4>(new float[16] {
			d(0, 0), d(0, 1), d(0, 2), 0,
			d(1, 0), d(1, 1), d(1, 2), 0,
			d(2, 0), d(2, 1), d(2, 2), 0,
			0, 0, 0, 1 });
	}

    static math::Matrix<4, 4> translate3(float x, float y, float z) { // Create a translation matrix. Can't be 3 by 3, as those could only rotate & scale 3d space
        return Matrix<4, 4>(new float[16] {
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1 });
    }
}

#endif /* Matrix_hpp */
