#include "crt_scene.hpp"

int main()
{
    CRTScene scene("scene6.crtscene");

    scene.renderScene();

    std::cout << "Scene rendered successfully." << std::endl;

    return 0;
}