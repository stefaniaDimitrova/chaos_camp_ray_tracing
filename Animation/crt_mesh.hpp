#pragma once
#include <vector>
#include "crt_vector.hpp"

class CRTMesh
{
public:
    CRTMesh(std::vector<CRTVector> vertices, std::vector<int> triangleVertIndices)
        : vertices(vertices), triangleVertIndices(triangleVertIndices) {}

    std::vector<CRTVector> getVertices() const {
        return vertices;
    }
    std::vector<int> getTriangleVertIndices() const {
        return triangleVertIndices;
    }

private:
    std::vector<CRTVector> vertices; //3D coorginates for the whole mesh
    std::vector<int> triangleVertIndices; // Each triple forms a triangle vertices
};