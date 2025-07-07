#include "crt_vector.hpp"

int main()
{
    CRTVector A1(-1.75, -1.75, -3);
    CRTVector B1(1.75,-1.75,-3);
    CRTVector C1(0,1.75,-3);

    CRTVector A1B1 = B1 - A1;
    CRTVector A1C1 = C1 - A1;

    CRTVector normal1 = A1B1.cross(A1C1);
    normal1 = normal1.normalize(); // Normalize the normal vector

    std::cout << "Normal vector of triangle 1: (" 
              << normal1.getX() << ", " 
              << normal1.getY() << ", " 
              << normal1.getZ() << ")\n";

    

    CRTVector A2(0,0,-1 );
    CRTVector B2(1,0,1);
    CRTVector C2(-1,0,1);

    CRTVector A2B2 = B2 - A2;
    CRTVector A2C2 = C2 - A2;
    CRTVector normal2 = A2B2.cross(A2C2);
    normal2 = normal2.normalize(); // Normalize the normal vector
    std::cout << "Normal vector of triangle 2: (" 
              << normal2.getX() << ", " 
              << normal2.getY() << ", " 
              << normal2.getZ() << ")\n";

            
    CRTVector A3(0.56,1.11,1.23);
    CRTVector B3(0.44,-2.368,-0.54);
    CRTVector C3(-1.56,0.15,-1.92);

    CRTVector A3B3 = B3 - A3;
    CRTVector A3C3 = C3 - A3;
    CRTVector normal3 = A3B3.cross(A3C3);
    normal3 = normal3.normalize(); // Normalize the normal vector   
    std::cout << "Normal vector of triangle 3: (" 
              << normal3.getX() << ", " 
              << normal3.getY() << ", " 
              << normal3.getZ() << ")\n";

    return 0;
}
