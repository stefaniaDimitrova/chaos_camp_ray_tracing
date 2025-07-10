#pragma once
#include "pixel.hpp"
#include <vector>

struct Picture{
    int imageWidth;
    int imageHeight;

    std::vector<std::vector<CRTPixel>> pixels;

    Picture(int width, int height) : imageWidth(width), imageHeight(height) {}
};