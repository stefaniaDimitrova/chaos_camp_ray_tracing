#include <iostream>
#include <fstream>

static const int imageWidth = 1920;
static const int imageHeight = 1080;
static const int maxColorComponent = 255;

int main()
{
    std::ofstream ppmFileStream("crt_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

    int red = 0;
    int green = 0;
    int blue = 0;

    for (int row = 0; row < imageHeight; ++row)
    {
        for (int col = 0; col < imageWidth; ++col)
        {
            if ((row <= 360 && col <= 480) || (row > 360 && row <= 720 && col > 960 && col <= 1440))
            {
                red =  rand() % 256;   // Random red value
                green = 0;
                blue = 0;

                ppmFileStream << red << ' ' << green << ' ' << blue << ' ' << '\t';

            }
            else if ((row <= 360 && col > 480 && col <= 960) || 
                    (row > 360 && row <= 720 && col > 1440) ||
                    (row > 720 && row <= 1080 && col <= 480))
                    {
                        red = 0;   // Random red value
                        green = rand() % 256;
                        blue = 0;

                        ppmFileStream << red << ' ' << green << ' ' << blue << ' ' << '\t';

                    }

            else if((row <= 360 && col > 960 && col <= 1440) || 
                    (row > 720 && col > 480 && col <= 960))
                    {
                        red = 0;   // Random red value
                        green = 0;
                        blue = rand() % 256;

                        ppmFileStream << red << ' ' << green << ' ' << blue << ' ' << '\t';
                    }

            else if((row <= 360 && col > 1440) ||
                    (row >360 && row <= 720 && col > 480 && col <= 960) ||
                    (row > 720 && col > 1440))
                    {
                        red = rand() % 256;   // Random red value
                        green = 0;
                        blue = rand() % 256;

                        ppmFileStream << red << ' ' << green << ' ' << blue << ' ' << '\t';

                        
                    }
            else 
            {
                red = rand() % 256;   // Random red value
                green = red;
                blue = red;

                ppmFileStream << red << ' ' << green << ' ' << blue << ' ' << '\t';
            }
            
        }
    }
}