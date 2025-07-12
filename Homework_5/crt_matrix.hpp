#pragma once
#include "crt_vector.hpp"

class CRTMatrix {
public:
friend CRTMatrix operator*(const CRTMatrix& lhs, const CRTMatrix& rhs) {
        CRTMatrix result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] +
                                 lhs.m[i][1] * rhs.m[1][j] +
                                 lhs.m[i][2] * rhs.m[2][j];
            }
        }
        return result;
    }

friend CRTVector operator*( const CRTVector& vector,const CRTMatrix& matrix) {
        return CRTVector(
            matrix.m[0][0] * vector.getX() + matrix.m[0][1] * vector.getY() + matrix.m[0][2] * vector.getZ(),
            matrix.m[1][0] * vector.getX() + matrix.m[1][1] * vector.getY() + matrix.m[1][2] * vector.getZ(),
            matrix.m[2][0] * vector.getX() + matrix.m[2][1] * vector.getY() + matrix.m[2][2] * vector.getZ()
        );
    }

float getM(int row, int col) const {
        return m[row][col];
    }

void setM(int row, int col, float value) {
        m[row][col] = value;
    }

private:
    float m[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    }; // Identity matrix by default
};