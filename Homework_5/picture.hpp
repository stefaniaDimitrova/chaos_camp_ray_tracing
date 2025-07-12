#pragma once
#include "pixel.hpp"
#include <vector>

struct Picture{
    int imageWidth;
    int imageHeight;

    std::vector<std::vector<CRTPixel>> pixels;

    Picture(int width, int height) : imageWidth(width), imageHeight(height) {}

    void clear() {
        pixels.clear();
        pixels.resize(imageWidth, std::vector<CRTPixel>(imageHeight));
    }
};