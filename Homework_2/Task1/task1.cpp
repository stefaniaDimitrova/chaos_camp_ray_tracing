#include <fstream>
#include "crt_ray.hpp"

static const int imageWidth = 1920;
static const int imageHeight = 1080;
static const int maxColorComponent = 255;

int main()
{
    std::ofstream ppmFileStream("rays_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

    int red = 0;
    int green = 0;
    int blue = 0;

    //camera ray algorithm
    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            //finding the center of each pixel and turning it to NDC
            float ndcX = (x + 0.5f) / imageWidth;
            float ndcY = (y + 0.5f) / imageHeight;

            //converting to screen space
            float screenX = (ndcX * 2.0f - 1.0f) * imageWidth / imageHeight;
            float screenY = (1.0f - ndcY * 2.0f );
            float screenZ = -1.0f; // Assuming a fixed depth for the ray

            //normalizing the ray direction
            CRTVector rayDirection(screenX, screenY, screenZ);
            float length = rayDirection.length();
            CRTVector normalizedDirection(screenX / length,screenY / length,screenZ / length);

            //creating the ray
            CRTRay cameraRay;
            cameraRay.origin = CRTVector(0.0f, 0.0f, 0.0f); // Assuming the camera is at the origin
            cameraRay.direction = normalizedDirection;

            //coloring the pixel
            int red = static_cast<int>(((cameraRay.direction.getX() + 1.0f) / 2.0f) * 255.0f);
            int green = static_cast<int>(((cameraRay.direction.getY() + 1.0f) / 2.0f) * 255.0f);
            int blue = static_cast<int>(((cameraRay.direction.getZ() + 1.0f) / 2.0f) * 255.0f);

            ppmFileStream << red << ' ' << green << ' ' << blue << '\t';

        }
        
    }
    
}