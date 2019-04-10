//
//  Vector.hpp
//  Dimension++
//
//  Created by Aaron Hodel on 29.01.19.
//  Copyright © 2019 Aaron Hodel. All rights reserved.
//

#ifndef Vector_hpp
#define Vector_hpp

#include <cmath>
#include <stdio.h>
#include <string>

namespace math {
    
    // Just tell vector there exists a matrix class out there somewhere
    template<int M, int N>
    class Matrix;
    
    template<int D>
    class Vector {
    private:
        float *values = nullptr;

    public:
        Vector();
        Vector(float v);
        Vector(float *v);
        Vector(Vector<D> const& other) { values = (float*) malloc(sizeof(float) * D); for(int i = 0; i < D; i++) values[i] = other.values[i]; }
        ~Vector() { free(values); }
        
        inline float &operator[](int index) { return values[index]; }
        
        float dot(Vector<D> other);
        inline float operator *(Vector<D> other) { return dot(other); }
        
        inline Vector<D> operator =(Vector<D> other) { for(int i = 0; i < D; i++) values[i] = other[i]; return *this; }

        inline Vector<D> operator +(Vector<D> other) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] + other[i]; return result; }
        inline Vector<D> operator -(Vector<D> other) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] - other[i]; return result; }
        inline Vector<D> operator +=(Vector<D> other) { for(int i = 0; i < D; i++) values[i] += other[i]; return *this; }
        inline Vector<D> operator -=(Vector<D> other) { for(int i = 0; i < D; i++) values[i] -= other[i]; return *this; }

        inline Vector<D> operator *(float scalar) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] * scalar; return result; }
        inline Vector<D> operator /(float scalar) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] / scalar; return result; }
        inline Vector<D> operator *=(float scalar) { for(int i = 0; i < D; i++) values[i] *= scalar; return *this; }
        inline Vector<D> operator /=(float scalar) { for(int i = 0; i < D; i++) values[i] /= scalar; return *this; }

		// Elementwise operators
		inline Vector<D> mult(Vector<D> other) { Vector<D> result; for (int i = 0; i < D; i++) result[i] = values[i] * other[i]; return result; }
		inline Vector<D> div(Vector<D> other) { Vector<D> result; for (int i = 0; i < D; i++) result[i] = values[i] / other[i]; return result; }

        inline Vector<D> operator +(float vec) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] + vec; return result; }
        inline Vector<D> operator -(float vec) { Vector<D> result; for(int i = 0; i < D; i++) result[i] = values[i] - vec; return result; }
        inline Vector<D> operator +=(float vec) { for(int i = 0; i < D; i++) values[i] += vec; return *this; }
        inline Vector<D> operator -=(float vec) { for(int i = 0; i < D; i++) values[i] -= vec; return *this; }
            
		inline Vector<D> copy() { return Vector<D>(values); } // Copy the vector and return it
        
        inline Vector<D> normalize() { float len = 0; for(int i = 0; i < D; i++) len += values[i] * values[i]; operator/=(1.0f / sqrt(len)); return *this; }
        inline float len() { float len = 0; for(int i = 0; i < D; i++) len += values[i] * values[i]; return sqrt(len); }
        inline float lensq() { float len = 0; for(int i = 0; i < D; i++) len += values[i] * values[i]; return len; }
    };
}

template <int D>
math::Vector<D>::Vector() {
    values = (float*) malloc(sizeof(float) * D);
    for(int i = 0; i < D; i++) values[i] = 0;
}

template <int D>
math::Vector<D>::Vector(float v) {
    values = (float*) malloc(sizeof(float) * D);
    for(int i = 0; i < D; i++) values[i] = v;
}

template <int D>
math::Vector<D>::Vector(float *v) {
    values = (float*) malloc(sizeof(float) * D);
    for(int i = 0; i < D; i++) values[i] = v[i];
}

template <int D>
float math::Vector<D>::dot(Vector<D> other) {
    float result = 0.0;
    for(int i = 0; i < D; i++) result += other[i] * values[i];
    return result;
}

namespace math {
 
    static math::Vector<3> cross(Vector<3> a, Vector<3> b) {
        math::Vector<3> c;
        c[0] = a[1] * b[2] - a[2] * b[1];
        c[1] = a[2] * b[0] - a[0] * b[2];
        c[2] = a[0] * b[1] - a[1] * b[0];
        return c;
    }
    
    static math::Vector<3> getNormal(Vector<3> &a, Vector<3> &b, Vector<3> &c) {
        math::Vector<3> d;
        float m0 = (b[0] - a[0]);
        float m1 = (b[1] - a[1]);
        float m2 = (b[2] - a[2]);
        float n0 = (c[0] - a[0]);
        float n1 = (c[1] - a[1]);
        float n2 = (c[2] - a[2]);

        c[0] = m1 * n2 - m2 * n1;
        c[1] = m2 * n0 - m0 * n2;
        c[2] = m0 * n1 - m1 * n0;
        return d;
    }
    
}

#endif /* Vector_hpp */
