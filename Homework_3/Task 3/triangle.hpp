#include "crt_vector.hpp"

static const int vertsInTriangle = 3;

class CRTTriangle {
public:
CRTTriangle (CRTVector A, CRTVector B, CRTVector C) {
        vertices[0] = A;
        vertices[1] = B;
        vertices[2] = C;
    }
private:
    CRTVector vertices[vertsInTriangle];  // Vertices of the triangle
};

