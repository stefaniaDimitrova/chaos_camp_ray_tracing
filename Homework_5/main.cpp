#include <fstream>
#include "camera.hpp"
#include "triangle.hpp"
#include "crt_ray.hpp"
#include "picture.hpp"

static const int imageWidth = 1920;
static const int imageHeight = 1080;
static const int maxColorComponent = 255;



CRTRay generatedRay (int x, int y, CRTCamera &camera)
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
    cameraRay.origin = camera.getPosition();
    cameraRay.direction = normalizedDirection;

    return cameraRay;
}

void generateTriangle(CRTTriangle &triangle, Picture& result, int red , int green, int blue, CRTCamera &camera)
{
    // Generate the normal vector for the triangle
    // This is done once and reused for all pixels
    CRTVector normal = triangle.generateNormalVector(triangle.v0(), triangle.v1(), triangle.v2());

    result.pixels.resize(imageWidth, std::vector<CRTPixel>(imageHeight));

    int bRed = 0;
    int bGreen = 0;
    int bBlue = 0;

    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            // Generate the ray for the current pixel
            CRTRay cameraRay = generatedRay(x, y, camera);

            //check if ray is not parallel to triangle plain
            if (normal.dotProduct(cameraRay.direction) != 0)
            {
                float rpDist = (triangle.v0()-cameraRay.origin).dotProduct(normal);
                float rProj = cameraRay.direction.dotProduct(normal);
                float t = rpDist/ rProj;

                CRTVector intersectionPoint = cameraRay.origin + (cameraRay.direction * t);

                CRTVector e0 = triangle.v1() - triangle.v0();
                CRTVector e1 = triangle.v2() - triangle.v1();
                CRTVector e2 = triangle.v0() - triangle.v2();

                CRTVector v0p = intersectionPoint - triangle.v0();
                CRTVector v1p = intersectionPoint - triangle.v1();
                CRTVector v2p = intersectionPoint - triangle.v2();

                bool leftOfEdge0 = normal.dotProduct(e0.cross(v0p)) > 0;
                bool leftOfEdge1 = normal.dotProduct(e1.cross(v1p)) > 0;
                bool leftOfEdge2 = normal.dotProduct(e2.cross(v2p)) > 0;

                if (leftOfEdge0 && leftOfEdge1 && leftOfEdge2)
                {
                    // The intersection point is inside the triangle
                    result.pixels[x][y].red = red;
                    result.pixels[x][y].green = green;
                    result.pixels[x][y].blue = blue;
                }   
            }    
       } 
   }  
}

void generatePicture(Picture &picture, std::ofstream &fileStream)
{
    for (int y = 0; y < imageHeight; ++y)
    {
        for (int x = 0; x < imageWidth; ++x)
        {
            CRTPixel pixel = picture.pixels[x][y];
            fileStream << pixel.red << ' ' 
                       << pixel.green << ' ' 
                       << pixel.blue << '\t';
        }
    }
}

int main()
{
    std::ofstream ppmFileStream("triangle_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";
    // std::cout << "HERE" << std::endl;

    Picture result(imageWidth, imageHeight);

    CRTCamera camera(CRTVector(0,0,0));

    CRTVector D( 0.0f,  1.0f, -3.0f);   // apex
    CRTVector A( 0.0f, -1.0f, -3.0f);   // front-base vertex (directly underneath the apex)
    CRTVector B(-1.0f, -1.0f, -5.0f);   // back-left
    CRTVector C( 1.0f, -1.0f, -5.0f);   // back-right

    // Two faces sharing the edge D→A:
    CRTTriangle leftFace  { D, A, B };
    CRTTriangle rightFace { D, C, A };
    
    generateTriangle(leftFace,result, 255,0,255, camera);
    generateTriangle(rightFace, result, 255,0,0, camera);
    generatePicture(result, ppmFileStream);

    ppmFileStream.close();
  

}