#pragma once
#include "crt_vector.hpp"
#include "crt_matrix.hpp"

class CRTCamera{
public:
CRTCamera (CRTVector position) : position(position) {}

CRTVector getPosition() const {
        return position;
    }

CRTMatrix getRotationMatrix() const {
        return rotationMatrix;
    }

void dolly (float distance) {
        position.setZ(position.getZ() + distance);
    }

void truck (float distance){
    position.setX(position.getX() + distance);
    }

void pedestal (float distance) {
        position.setY(position.getY() + distance);
    }


//rotate the camera around the Y-axis
void pan (float angle) {
        // Assuming angle is in radians
        rotationMatrix.setM(0,0,std::cos(angle));
        rotationMatrix.setM(0,2,-std::sin(angle));
        rotationMatrix.setM(2,0,std::sin(angle));
        rotationMatrix.setM(2,2,std::cos(angle));
    }

void tilt (float angle) {
        // Assuming angle is in radians
        rotationMatrix.setM(1,1,std::cos(angle));
        rotationMatrix.setM(1,2,-std::sin(angle));
        rotationMatrix.setM(2,1,std::sin(angle));
        rotationMatrix.setM(2,2,std::cos(angle));
    }

void roll (float angle) {
        // Assuming angle is in radians
        rotationMatrix.setM(0,0,std::cos(angle));
        rotationMatrix.setM(0,1,-std::cos(angle));
        rotationMatrix.setM(1,0,std::sin(angle));
        rotationMatrix.setM(1,1,std::cos(angle));
    }

void reset() {
        rotationMatrix = CRTMatrix(); // Reset to identity matrix
        position = CRTVector(0.0f, 0.0f, 0.0f); // Reset position to origin
    }

private:
    CRTMatrix rotationMatrix;
    CRTVector position; //In 3D space
    };