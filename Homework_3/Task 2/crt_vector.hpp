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

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    CRTVector operator+(const CRTVector& other) const {
        return CRTVector(x + other.x, y + other.y, z + other.z);
    }

    CRTVector operator-(const CRTVector& other) const {
        return CRTVector(x - other.x, y - other.y, z - other.z);
    }   

    CRTVector cross(const CRTVector& other) const {
        return CRTVector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }


   private:
   float x, y, z;
};