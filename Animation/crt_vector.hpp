#pragma once

#include <iostream>
#include <cmath>

class CRTVector {
public:
    CRTVector(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z) {}

    float getX() const {
        return x;
    }
    float getY() const {
        return y;
    }
    float getZ() const {
        return z;
    }
    void setX(float x) {
        this->x = x;
    }
    void setY(float y) {
        this->y = y;
    }
    void setZ(float z) {
        this->z = z;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    CRTVector operator+(const CRTVector& other) const {
        return CRTVector(x + other.x, y + other.y, z + other.z);
    }

    CRTVector operator-(const CRTVector& other) const {
        return CRTVector(x - other.x, y - other.y, z - other.z);
    }   

    CRTVector operator*(float scalar) const {
        return CRTVector(x * scalar, y * scalar, z * scalar);
    }

    CRTVector cross(const CRTVector& other) const {
        return CRTVector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    CRTVector normalize() const {
        float len = length();
        if (len == 0) return CRTVector(0, 0, 0); // Avoid division by zero
        return CRTVector(x / len, y / len, z / len);
    }

    float dotProduct(const CRTVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }


   private:
   float x, y, z;
};