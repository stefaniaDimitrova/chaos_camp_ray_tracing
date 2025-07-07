#include "crt_vector.hpp"

int main()
{
    CRTVector A1(3.5, 0, 0);
    CRTVector B1(1.75,3.5,0);

    CRTVector C1 = A1.cross(B1);

    std::cout << "Cross product of A1 and B1: (" 
              << C1.getX() << ", " 
              << C1.getY() << ", " 
              << C1.getZ() << ")\n";


    CRTVector A2(3,-3,1);
    CRTVector B2(4,9,3);

    CRTVector C2 = A2.cross(B2);

    std::cout << "Cross product of A2 and B2: (" 
              << C2.getX() << ", " 
              << C2.getY() << ", " 
              << C2.getZ() << ")\n";

    
    CRTVector A3(3,-3,1);
    CRTVector B3(4,9,3);

    CRTVector C3 = A3.cross(B3);
    float lengthC3 = C3.length();
    std::cout << "Area of parallelogram formed by vectors A3 and B3: " 
              << lengthC3 << "\n";

    CRTVector A4(3,-3,1);
    CRTVector B4(-12,12,-4);
    CRTVector C4 = A4.cross(B4);
    float lengthC4 = C4.length();
    std::cout << "Area of parallelogram formed by vectors A4 and B4: " 
              << lengthC4 << "\n";

    return 0;
}