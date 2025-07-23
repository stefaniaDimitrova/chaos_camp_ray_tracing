#pragma once
#include "pixel.hpp"
#include <vector>
#include "crt_color.hpp"

struct Picture{
    int imageWidth;
    int imageHeight;

    std::vector<std::vector<CRTPixel>> pixels;

    Picture(int width, int height, CRTColor backgroundColor) : imageWidth(width), imageHeight(height) 
    {
        // std::cout << backgroundColor.green << std::endl;
        pixels.resize(imageWidth, std::vector<CRTPixel>(imageHeight, CRTPixel(backgroundColor.red, backgroundColor.green, backgroundColor.blue)));
    }

    void clear() {
        pixels.clear();
        pixels.resize(imageWidth, std::vector<CRTPixel>(imageHeight));
    }
};