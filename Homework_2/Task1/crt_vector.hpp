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


   private:
   float x, y, z;
};