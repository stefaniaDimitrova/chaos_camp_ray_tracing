#pragma once
#include "crt_color.hpp"

class CRTSettings
{
    public:
    CRTSettings(int imageWidth, int imageHeight, CRTColor backgroundColor) : 
            imageWidth(imageWidth), imageHeight(imageHeight), backgroundColor(backgroundColor) {}

    CRTSettings()
    {
        imageWidth = 1920; // Default width
        imageHeight = 1080; // Default height  
        backgroundColor = CRTColor(0.0f, 0.0f, 0.0f); // Default background color (black)
    }

    int getImageWidth() const {
        return imageWidth;
    }
    int getImageHeight() const {
        return imageHeight;
    }

    CRTColor getBackgroundColor() const {
        return backgroundColor;
    }
    private:
    int imageWidth;
    int imageHeight;
    CRTColor backgroundColor;
};
