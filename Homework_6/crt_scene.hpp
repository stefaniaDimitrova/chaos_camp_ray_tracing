#pragma once
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include "crt_vector.hpp"
#include "camera.hpp"
#include "crt_mesh.hpp"
#include "crt_settings.hpp"
#include "triangle.hpp"
#include "picture.hpp"
#include "crt_ray.hpp"

using namespace rapidjson;

class CRTScene{
public:
    CRTScene(const std::string& sceneFileName)
    {
        std::cout << "Loading scene from: " << sceneFileName << std::endl;
        // Initialize the document
        initRapidJSON(sceneFileName);

        parseRapidJSON(document, *this);
    }

    void renderScene()
    {
        int trianglesCounter = 1;
        int red = rand() % 256;
        int green = rand() % 256;
        int blue = rand() % 256;

        Picture result(settings.getImageWidth(), settings.getImageHeight(), settings.getBackgroundColor());
        int counter = 0;
        for (size_t i = 0; i < geometryObjects.size(); i++)
        {
            for (size_t j = 0; j < geometryObjects[i].getTriangleVertIndices().size(); j+=3)
            {
                int v1Index = geometryObjects[i].getTriangleVertIndices()[j];
                int v2Index = geometryObjects[i].getTriangleVertIndices()[j+1];
                int v3Index = geometryObjects[i].getTriangleVertIndices()[j+2];

                CRTVector v1 = geometryObjects[i].getVertices()[v1Index];
                CRTVector v2 = geometryObjects[i].getVertices()[v2Index];
                CRTVector v3 = geometryObjects[i].getVertices()[v3Index];

                CRTTriangle triangle(v1, v2, v3);
                int red = rand() % 256;
                int green = rand() % 256;
                int blue = rand() % 256;
                generateTriangle(triangle, result, red, green, blue, camera); // Red color for the triangle

                std::cout << "Triangle " << trianglesCounter << " rendered successfully!\n";
                trianglesCounter++;
            }
            
        }

        std::ofstream output = generateStream("output_image.ppm");
        generatePicture(result, output);
        
    }


private:
    std::vector <CRTMesh> geometryObjects;
    CRTCamera camera;
    CRTSettings settings;
    Document document;

    void initRapidJSON(std::string fileName)
    {
        std::ifstream ifs(fileName);
        assert(ifs.is_open());

        IStreamWrapper isw(ifs);
        this->document.ParseStream(isw);
        std::cout << "Document parsed successfully.\n";
    }

    void parseRapidJSON(Document& doc, CRTScene &scene)
    {
        //getting the settings
        if (!doc.HasMember("settings")) return;

        const Value& setVal = doc["settings"];

        //getting background color
        if (!setVal.HasMember("background_color")) {
            std::cerr << "No background_color found in settings\n";
            return;
        }
        const Value& bgVal = setVal["background_color"];
        float r = bgVal[0].GetFloat();
        float g = bgVal[1].GetFloat();
        float b = bgVal[2].GetFloat();
        CRTColor backgroundColor(r, g, b);
        
        //getting image width and height
        if (!setVal.HasMember("image_settings"))
        {
            std::cerr << "No image_width or image_height found in settings\n";
            return;
        }
        
        const Value& imgSet = setVal["image_settings"];
        int imageWidth = imgSet["width"].GetInt();
        int imageHeight = imgSet["height"].GetInt();
        
        CRTSettings settings(imageWidth,imageHeight,backgroundColor);
        this->settings = settings;


        //getting the camera
        if(!doc.HasMember("camera")) return;
    
        const Value& camVal = doc["camera"];
        if(!camVal.HasMember("matrix"))
        {
            std::cerr << "No camera matrix found\n";
            return;
        }

        const Value& matrixVal = camVal["matrix"];
        CRTMatrix cameraMatrix;
        
        int counter = 0;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                // if(!matrixVal[i].IsArray() || matrixVal[i].Size() != 9)
                // {
                //     std::cerr << "Invalid camera matrix format\n";
                //     return;
                // }
                cameraMatrix.setM(i, j, matrixVal[counter].GetFloat());
                counter++;
            }
        }

        if(!camVal.HasMember("position"))
        {
            std::cerr << "No camera position found\n";
            return;
        }
        const Value& posVal = camVal["position"];
        CRTVector cameraPosition;
        cameraPosition.setX(posVal[0].GetFloat());
        cameraPosition.setY(posVal[1].GetFloat());
        cameraPosition.setZ(posVal[2].GetFloat());

        CRTCamera camera(cameraPosition);
        camera.setRotationMatrix(cameraMatrix);

        this->camera = camera;


        //getting meshes
        if (!doc.HasMember("objects")) return;
        const Value& objectsVal = doc["objects"];

        if (!objectsVal.IsArray()) {
            std::cerr << "Objects should be an array\n";
            return;
        }

        for (SizeType i = 0; i < objectsVal.Size(); ++i) 
        {
            const Value& object = objectsVal[i];
            if (!object.HasMember("vertices") || !object.HasMember("triangles")) {
                std::cerr << "Object " << i << " is missing vertices or triangles\n";
                continue;
            }

            const Value& verticesVal = object["vertices"];
            const Value& trianglesVal = object["triangles"];

           std::vector<CRTVector> vertices;
            for (SizeType k = 0; k + 2 < verticesVal.Size(); k += 3) {
                float x = verticesVal[k + 0].GetFloat();
                float y = verticesVal[k + 1].GetFloat();
                float z = verticesVal[k + 2].GetFloat();
                vertices.emplace_back(x, y, z);
                    }

            std::vector<int> triangleVertIndices;
        for (SizeType k = 0; k + 2 < trianglesVal.Size(); k += 3) {
            triangleVertIndices.push_back(trianglesVal[k + 0].GetInt());
            triangleVertIndices.push_back(trianglesVal[k + 1].GetInt());
            triangleVertIndices.push_back(trianglesVal[k + 2].GetInt());
            }

            CRTMesh mesh(vertices, triangleVertIndices);
            this->geometryObjects.push_back(mesh);
        }

        // if(!objectsVal.HasMember("vertices"))
        // {
        //     std::cerr << "No vertices found in objects\n";
        //     return;
        // }

        // std::vector<CRTVector> vertices;
        // const Value& verticesVal = objectsVal["vertices"];
        // for (SizeType i = 0; i < verticesVal.Size(); ++i) 
        // {
        //     float x = verticesVal[i][0].GetFloat();
        //     float y = verticesVal[i][1].GetFloat();
        //     float z = verticesVal[i][2].GetFloat();
        //     vertices.push_back(CRTVector(x, y, z));
        // }

        // if(!objectsVal.HasMember("triangles"))
        // {
        //     std::cerr << "No triangles found in objects\n";
        //     return;
        // }

        // std::vector<int> triangleVertIndices;
        // const Value& trianglesVal = objectsVal["triangles"];
        
        // for (SizeType i = 0; i < trianglesVal.Size(); ++i) 
        // {
        //     triangleVertIndices.push_back(trianglesVal[i][0].GetInt());
        //     triangleVertIndices.push_back(trianglesVal[i][1].GetInt());
        //     triangleVertIndices.push_back(trianglesVal[i][2].GetInt());
        // }
        // CRTMesh mesh(vertices, triangleVertIndices);
        // this->geometryObjects.push_back(mesh);
    }

    void generateTriangle(CRTTriangle &triangle, Picture& result, int red , int green, int blue, CRTCamera &camera)
{
    // Generate the normal vector for the triangle
    // This is done once and reused for all pixels
    CRTVector normal = triangle.generateNormalVector(triangle.v0(), triangle.v1(), triangle.v2());

    result.pixels.resize(settings.getImageWidth(), std::vector<CRTPixel>(settings.getImageHeight()));

    int bRed = 0;
    int bGreen = 0;
    int bBlue = 0;

    for (size_t y = 0; y < settings.getImageHeight(); y++)
    {
        for (size_t x = 0; x < settings.getImageWidth(); x++)
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

CRTRay generatedRay (int x, int y, CRTCamera &camera)
{
    //finding the center of each pixel and turning it to NDC
    float ndcX = (x + 0.5f) / settings.getImageWidth();
    float ndcY = (y + 0.5f) / settings.getImageHeight();

    //converting to screen space
    float screenX = (ndcX * 2.0f - 1.0f) * settings.getImageWidth() / settings.getImageHeight();
    float screenY = (1.0f - ndcY * 2.0f );
    float screenZ = -1.0f; // Assuming a fixed depth for the ray

    //normalizing the ray direction
    CRTVector rayDirection(screenX, screenY, screenZ);
    CRTVector rRayDirection = rayDirection*camera.getRotationMatrix(); // Apply camera rotation

    float length = rRayDirection.length();
    CRTVector normalizedDirection(rRayDirection.getX() / length,rRayDirection.getY() / length,rRayDirection.getZ() / length);

    //creating the ray
    CRTRay cameraRay;
    cameraRay.origin = camera.getPosition();
    cameraRay.direction = normalizedDirection;

    return cameraRay;
}

void generatePicture(Picture &picture, std::ofstream &fileStream)
{
    for (int y = 0; y < settings.getImageHeight(); ++y)
    {
        for (int x = 0; x < settings.getImageWidth(); ++x)
        {
            CRTPixel pixel = picture.pixels[x][y];
            fileStream << pixel.red << ' ' 
                       << pixel.green << ' ' 
                       << pixel.blue << '\t';
        }
    }
}

std::ofstream generateStream(std::string fileName)
{
    std::ofstream ppmFileStream(fileName, std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n" << settings.getImageWidth() << " " << settings.getImageHeight() << "\n" << 255 << "\n";

    return ppmFileStream;
}

};