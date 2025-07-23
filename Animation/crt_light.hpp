#pragma once
#include "crt_vector.hpp"
#include "crt_color.hpp"
#include "crt_ray.hpp"

class CRTLight{
public:
    CRTLight() : position(CRTVector(0,0,0)) {}

    CRTLight(const CRTVector& position)
        : position(position) {}

    const CRTVector& getPosition() const {
        return position;
    }

    void setIntensity(float intensity) {
        this->intensity = intensity;
    }

    float getIntensity() const {
        return intensity;
    }

private:
    CRTVector position; // Position of the light in 3D space
    float intensity;
};