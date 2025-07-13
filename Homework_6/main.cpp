#include "crt_scene.hpp"

int main()
{
    CRTScene scene("scene4.crtscene");

    scene.renderScene();

    std::cout << "Scene rendered successfully." << std::endl;

    return 0;
}