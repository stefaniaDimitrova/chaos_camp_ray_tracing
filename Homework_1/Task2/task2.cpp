#include <iostream>
#include <fstream>

static const int imageWidth = 1920;
static const int imageHeight = 1080;
static const int maxColorComponent = 255;

int main()
{
    std::ofstream ppmFileStream("task2_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

    int red = 0;
    int green = 0; 
    int blue = 0;

    const int centerX = imageWidth / 2;
    const int centerY = imageHeight / 2;
    const int radius = 300;

    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= radius * radius)
            {
                // Inside the circle
                red = 0;   // Random red value
                green = 180; // Random green value
                blue = 0;  // Random blue value
            }
            else
            {
                // Outside the circle, set to black
                red = 180;
                green = 180;
                blue = 180;
            }

            ppmFileStream << red << ' ' << green << ' ' << blue << '\t'; 
        }
        
    }
    
}