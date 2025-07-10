#include "crt_vector.hpp"

class CRTCamera{
public:
CRTCamera (CRTVector position) : position(position) {}

CRTVector getPosition() const {
        return position;
    }
private:
    CRTVector position; //In 3D space
    };