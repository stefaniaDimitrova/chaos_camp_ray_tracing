#pragma once

#include "crt_vector.hpp"

static const int vertsInTriangle = 3;

class CRTTriangle {
public:
CRTTriangle (CRTVector A, CRTVector B, CRTVector C) {
        vertices[0] = A;
        vertices[1] = B;
        vertices[2] = C;
        normal = generateNormalVector(A,B,C);
    }

CRTVector getNormal() const {
        return normal;
    }

CRTVector v0() const {
        return vertices[0];
    }

CRTVector v1() const {
    return vertices[1];
}

CRTVector v2() const{
    return vertices[2];
}

CRTVector generateNormalVector(CRTVector A, CRTVector B, CRTVector C) {
    CRTVector AB = B - A;
    CRTVector AC = C - A;
    CRTVector normal = AB.cross(AC);
    return normal.normalize(); // Normalize the normal vector
}
private:
    CRTVector vertices[vertsInTriangle];  // Vertices of the triangle
    CRTVector normal = generateNormalVector(vertices[0], vertices[1], vertices[2]);

};

