#ifndef TINYMATH_H
#define TINYMATH_H

#include <cmath>

// Forward references of each of the structs
struct Vector3D;
struct Matrix3D;

/*!
 * \brief mathematical 3D vector structure
 * Vector3D performs vector operations with 3-dimensions
 * The purpose of this class is primarily for 3D graphics
 * applications.
 */
struct Vector3D {
    // Note: x,y,z are a convention
    // x,y,z could be position, but also any 3-component value.
    float x, y, z;

    // Default conostrutcor
    // 'why default?' https://stackoverflow.com/questions/20828907/the-new-keyword-default-in-c11
    Vector3D() = default;

    // The "Real" constructor we want to use.
    // This initializes the values x,y,z
    Vector3D(float a, float b, float c) : x(a), y(b), z(c) {
    }

    // The "Real" constructor we want to use.
    // This initializes the values x,y,z
    Vector3D(Vector3D const &v) : x(v.x), y(v.y), z(v.z) {
    }

    // Index operator, allowing us to access the individual
    // x,y,z components of our vector.
    float &operator[](int i) {
        // Whenever our vector is initialized all variables contained in it are added to memory in order
        // Therefore all pointers are in order as well and thus can be referenced like this.
        // This may not work if the stack is multi-threaded this may not be thread-safe
        return ((&x)[i]);
    }

    // Index operator, allowing us to access the individual
    // x,y,z components of our vector.
    const float &operator[](int i) const {
        // this is passing the direct pointer reference as a const and therefore cannot be changed.
        return ((&x)[i]);
    }

    // Multiplication Operator
    // Multiply vector by a uniform-scalar.
    Vector3D &operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return (*this);
    }

    // Division Operator
    Vector3D &operator/=(float s) {
        if (s != 0) {
            x /= s;
            y /= s;
            z /= s;
        }

        return (*this);
    }

    // Addition operator
    Vector3D &operator+=(const Vector3D &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }

    // Subtraction operator
    Vector3D &operator-=(const Vector3D &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }

};

// Vector comparison
inline bool operator==(const Vector3D &v1, const Vector3D &v2) {
    return (v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]);
}

// Compute the dot product of a Vector3D
inline float Dot(const Vector3D &a, const Vector3D &b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// Multiplication of a vector by a scalar values
inline Vector3D operator*(const Vector3D &v, float s) {
    Vector3D vec(v.x * s, v.y * s, v.z * s);
    return vec;
}

// Multiplication of a vector by a vector is by default a dot product
inline float operator*(const Vector3D &a, const Vector3D &b) {
    return Dot(a, b);
}

// Division of a vector by a scalar value.
inline Vector3D operator/(const Vector3D &v, float s) {
    if (s != 0) {
        Vector3D vec(v.x / s, v.y / s, v.z / s);
        return vec;
    } else {
        return v;
    }
}

// Negation of a vector
// Use Case: Sometimes it is handy to apply a force in an opposite direction
inline Vector3D operator-(const Vector3D &v) {
    Vector3D vec(-v.x, -v.y, -v.z);
    return vec;
}

// Return the magnitude of a vector
inline float Magnitude(const Vector3D &v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Return the sqr magnitude of a vector
inline float MagnitudeSqr(const Vector3D &v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

// Add two vectors together
inline Vector3D operator+(const Vector3D &a, const Vector3D &b) {
    Vector3D vec(a.x + b.x, a.y + b.y, a.z + b.z);
    return vec;
}

// Subtract two vectors
inline Vector3D operator-(const Vector3D &a, const Vector3D &b) {
    Vector3D vec(a.x - b.x, a.y - b.y, a.z - b.z);
    return vec;
}

// Vector Projection
inline Vector3D Project(const Vector3D &a, const Vector3D &b) {
    return b * (a * (b / Magnitude(b)));
}

// Set a vectors magnitude to 1
// Note: This is NOT generating a normal vector
inline Vector3D Normalize(const Vector3D &v) {
    float mag = Magnitude(v);
    if (mag != 0) {
        return v / mag;
    } else {
        return v;
    }
}

inline void Print(const Vector3D &v) {
    printf("Vector[x=%.2f, y=%.2f, z=%.2f]\n", v.x, v.y, v.z);
}

// Find the closest point on a line segment to another point v.
inline Vector3D PointToLine(const Vector3D &v, const Vector3D &a, const Vector3D &b) {

    Vector3D p = b - a;
    Vector3D p2 = v - a;

    float sqrDist = MagnitudeSqr(p);
    float u = Dot(p, p2) / sqrDist;


    if (u > 1.0f) {
        u = 1.0f;
    } else if (u < 0.0f) {
        u = 0.0f;
    }

    return a + (p * u);
}


// a x b (read: 'a crossed b')
// Produces a new vector perpendicular to a and b.
// (So long as a and b are not parallel which returns zero vector)
inline Vector3D CrossProduct(const Vector3D &a, const Vector3D &b) {
    Vector3D vec((a.y * b.z) - (a.z * b.y),
                 (a.z * b.x) - (a.x * b.z),
                 (a.x * b.y) - (a.y * b.x));
    return vec;
}


/*!
 * \brief Matrix 3D represents 3x3 matrices in Math
 * Assists in mathematical operations involving matrices.
 */
struct Matrix3D {
private:
    float n[3][3];  // Store each value of the matrix

public:
    Matrix3D() = default;

    // Row major order this is so values can be read let to right and index by [x][y] values
    // Matrix constructor with 9 scalar values.
    Matrix3D(float n00, float n01, float n02,
             float n10, float n11, float n12,
             float n20, float n21, float n22) {

        n[0][0] = n00;
        n[0][1] = n10;
        n[0][2] = n20;
        n[1][0] = n01;
        n[1][1] = n11;
        n[1][2] = n21;
        n[2][0] = n02;
        n[2][1] = n12;
        n[2][2] = n22;
    }

    // Matrix constructor from three vectors.
    Matrix3D(const Vector3D &a, const Vector3D &b, const Vector3D &c) {
        n[0][0] = a.x;
        n[0][1] = a.y;
        n[0][2] = a.z;
        n[1][0] = b.x;
        n[1][1] = b.y;
        n[1][2] = b.z;
        n[2][0] = c.x;
        n[2][1] = c.y;
        n[2][2] = c.z;
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    float &operator()(int i, int j) {
        return (n[j][i]);
    }

    // Index operator with two dimensions
    // Example: M(1,1) returns row 1 and column 1 of matrix M.
    const float &operator()(int i, int j) const {
        return (n[j][i]);
    }

    // Return a row from a matrix as a vector.
    Vector3D &operator[](int j) {
        return (*reinterpret_cast<Vector3D *>(n[j]));
    }

    // Return a row from a matrix as a vector.
    const Vector3D &operator[](int j) const {
        return (*reinterpret_cast<const Vector3D *>(n[j]));
    }

};
// matrix comparison
inline bool operator==(const Matrix3D &m1, const Matrix3D &m2) {
    return (m1[0][0] == m2[0][0]) && (m1[0][1] == m2[0][1]) && (m1[0][2] == m2[0][2])
           && (m1[1][0] == m2[1][0]) && (m1[1][1] == m2[1][1]) && (m1[1][2] == m2[1][2])
           && (m1[2][0] == m2[2][0]) && (m1[2][1] == m2[2][1]) && (m1[2][2] == m2[2][2]);
}


// Matrix Multiplication
inline Matrix3D operator*(const Matrix3D &A, const Matrix3D &B) {
    float n00 = (A[0][0] * B[0][0]) + (A[0][1] * B[1][0]) + (A[0][2] * B[2][0]);
    float n10 = (A[0][0] * B[0][1]) + (A[0][1] * B[1][1]) + (A[0][2] * B[2][1]);
    float n20 = (A[0][0] * B[0][2]) + (A[0][1] * B[1][2]) + (A[0][2] * B[2][2]);

    float n01 = (A[1][0] * B[0][0]) + (A[1][1] * B[1][0]) + (A[1][2] * B[2][0]);
    float n11 = (A[1][0] * B[0][1]) + (A[1][1] * B[1][1]) + (A[1][2] * B[2][1]);
    float n21 = (A[1][0] * B[0][2]) + (A[1][1] * B[1][2]) + (A[1][2] * B[2][2]);

    float n02 = (A[2][0] * B[0][0]) + (A[2][1] * B[1][0]) + (A[2][2] * B[2][0]);
    float n12 = (A[2][0] * B[0][1]) + (A[2][1] * B[1][1]) + (A[2][2] * B[2][1]);
    float n22 = (A[2][0] * B[0][2]) + (A[2][1] * B[1][2]) + (A[2][2] * B[2][2]);

    Matrix3D mat3D(n00, n01, n02, n10, n11, n12, n20, n21, n22);
    return mat3D;
}

// Matrix multiply by a vector

inline Vector3D operator*(const Matrix3D &M, const Vector3D &v) {
    float n00 = (M[0][0] * v[0]) + (M[1][0] * v[1]) + (M[2][0] * v[2]);
    float n10 = (M[0][1] * v[0]) + (M[1][1] * v[1]) + (M[2][1] * v[2]);
    float n20 = (M[0][2] * v[0]) + (M[1][2] * v[1]) + (M[2][2] * v[2]);

    Vector3D vec(n00, n10, n20);
    return vec;
}


#endif
