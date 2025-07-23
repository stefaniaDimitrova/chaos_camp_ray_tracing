#include "crt_scene.hpp"

int main()
{
    CRTScene scene("scene3.crtscene");

    scene.renderScene();

    std::cout << "Scene rendered successfully." << std::endl;

    return 0;
}