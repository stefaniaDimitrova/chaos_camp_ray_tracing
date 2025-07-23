#pragma once
#include <fstream>
#include <cmath>
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
#include "crt_light.hpp"

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
    std::vector <CRTLight> lights;

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
        std::cout << (this->settings).getBackgroundColor().green << std::endl;

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
            for (SizeType k = 0; k + 2 < verticesVal.Size(); k += 3)
            {
                float x = verticesVal[k + 0].GetFloat();
                float y = verticesVal[k + 1].GetFloat();
                float z = verticesVal[k + 2].GetFloat();
                vertices.emplace_back(x, y, z);
            }

            std::vector<int> triangleVertIndices;
            for (SizeType k = 0; k + 2 < trianglesVal.Size(); k += 3)
            {
                triangleVertIndices.push_back(trianglesVal[k + 0].GetInt());
                triangleVertIndices.push_back(trianglesVal[k + 1].GetInt());
                triangleVertIndices.push_back(trianglesVal[k + 2].GetInt());
            }

            CRTMesh mesh(vertices, triangleVertIndices);
            this->geometryObjects.push_back(mesh);
        }

        // --- inside parseRapidJSON(...), replace your single‐light code with this ---

if (!doc.HasMember("lights")) return;

const Value& lightsArray = doc["lights"];
if (!lightsArray.IsArray()) {
    std::cerr << "Lights must be an array\n";
    return;
}

// clear any existing lights
this->lights.clear();

for (SizeType i = 0; i < lightsArray.Size(); ++i) {
    const Value& lightObj = lightsArray[i];

    // read position
    if (!lightObj.HasMember("position") || !lightObj["position"].IsArray() || lightObj["position"].Size() != 3) {
        std::cerr << "Light " << i << " has no valid position\n";
        continue;
    }
    const Value& posVal = lightObj["position"];
    CRTVector pos;
    pos.setX(posVal[0].GetFloat());
    pos.setY(posVal[1].GetFloat());
    pos.setZ(posVal[2].GetFloat());

    // read intensity
    if (!lightObj.HasMember("intensity")) {
        std::cerr << "Light " << i << " has no intensity\n";
        continue;
    }
    float intensity = lightObj["intensity"].GetFloat();

    // construct and store
    CRTLight L(pos);
    L.setIntensity(intensity);
    this->lights.push_back(L);
}

// now this->lights contains one CRTLight per JSON entry

    }

    void generateTriangle(CRTTriangle &triangle, Picture& result, int red , int green, int blue, CRTCamera &camera)
    {
        // Generate the normal vector for the triangle
        // This is done once and reused for all pixels
        CRTVector normal = triangle.generateNormalVector(triangle.v0(), triangle.v1(), triangle.v2());

        result.pixels.resize(settings.getImageWidth(), std::vector<CRTPixel>(settings.getImageHeight()));

        int bRed = settings.getBackgroundColor().red;
        int bGreen = settings.getBackgroundColor().green;
        int bBlue = settings.getBackgroundColor().blue;

        for (size_t y = 0; y < settings.getImageHeight(); y++)
        {
            for (size_t x = 0; x < settings.getImageWidth(); x++)
            {
                // Generate the ray for the current pixel
                CRTRay cameraRay = generatedRay(x,y,camera);

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
                        CRTColor shaded = shade(intersectionPoint,triangle);
                        result.pixels[x][y].red = shaded.red;
                        result.pixels[x][y].green = shaded.green;
                        result.pixels[x][y].blue = shaded.blue;

                        // std::cout << "Red: "<< shaded.red << "Green: " << shaded.green << "Blue: " << shaded.blue << std::endl;
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

CRTColor shade(const CRTVector &P, const CRTTriangle &triangle) {
    // P        = point on surface
    // triangle = the triangle we're shading (so we can get its normal)
    CRTColor result(0,0,0);
    // surface normal
    CRTVector N = triangle.getNormal();

    // for each light in the scene
    for (std::size_t li = 0; li < lights.size(); ++li) {
        CRTLight &Lgt = lights[li];
        // vector from P to light
        CRTVector Lvec = Lgt.getPosition() - P;
        float dist2 = Lvec.dotProduct(Lvec);
        float dist  = std::sqrt(dist2);
        // normalized light direction
        CRTVector Ldir(
            Lvec.getX() / dist,
            Lvec.getY() / dist,
            Lvec.getZ() / dist
        );

        // shadow‐ray with a small bias along Ldir so we don't self‐intersect
        CRTRay shadowRay;
        const float eps = 1e-4f;
        shadowRay.origin = CRTVector(
            P.getX() + Ldir.getX()*eps,
            P.getY() + Ldir.getY()*eps,
            P.getZ() + Ldir.getZ()*eps
        );
        shadowRay.direction = Ldir;

        // test shadow‐ray against every triangle in every mesh
        bool blocked = false;
        for (std::size_t mi = 0; mi < geometryObjects.size() && !blocked; ++mi) {
            CRTMesh &mesh = geometryObjects[mi];
            const std::vector<CRTVector> &V = mesh.getVertices();
            const std::vector<int>    &I = mesh.getTriangleVertIndices();

            for (std::size_t ti = 0; ti + 2 < I.size() && !blocked; ti += 3) {
                const CRTVector &v0 = V[I[ti+0]];
                const CRTVector &v1 = V[I[ti+1]];
                const CRTVector &v2 = V[I[ti+2]];

                // triangle's local normal
                CRTVector e0 = v1 - v0;
                CRTVector e1 = v2 - v0;
                CRTVector triN = e0.cross(e1).normalize();

                // plane intersection
                float denom = triN.dotProduct(shadowRay.direction);
                if (std::fabs(denom) < 1e-6f) 
                    continue;  // parallel

                float tHit = (v0 - shadowRay.origin).dotProduct(triN) / denom;
                if (tHit < eps || tHit > dist) 
                    continue;  // behind origin or beyond light

                // intersection point
                CRTVector Phit(
                    shadowRay.origin.getX() + shadowRay.direction.getX()*tHit,
                    shadowRay.origin.getY() + shadowRay.direction.getY()*tHit,
                    shadowRay.origin.getZ() + shadowRay.direction.getZ()*tHit
                );

                // inside‐triangle test (edge‐cross‐product / same‐side)
                CRTVector c0 = (v1 - v0).cross(Phit - v0);
                CRTVector c1 = (v2 - v1).cross(Phit - v1);
                CRTVector c2 = (v0 - v2).cross(Phit - v2);

                if (triN.dotProduct(c0) > 0 &&
                    triN.dotProduct(c1) > 0 &&
                    triN.dotProduct(c2) > 0)
                {
                    blocked = true;
                }
            }
        }

        if (blocked)
            continue;  // this light contributes nothing

        // Lambertian term
        float cosTheta = std::max(0.0f, N.dotProduct(Ldir));
        float intensity = Lgt.getIntensity();
        CRTColor albedo(100, 255, 0);
        float attenuation = intensity / (4.0f * static_cast<float>(3.14) * dist2);

        result.red   += attenuation * albedo.red   * cosTheta;
        result.green += attenuation * albedo.green * cosTheta;
        result.blue  += attenuation * albedo.blue  * cosTheta;
    }

    // clamp to [0,255]
    if (result.red   > 255.0f) result.red   = 255.0f;
    if (result.green > 255.0f) result.green = 255.0f;
    if (result.blue  > 255.0f) result.blue  = 255.0f;

    return result;
}

    // CRTColor shade(CRTVector & intersection, CRTTriangle& triangle)
    // {
    //     CRTColor finalColor = CRTColor(0,0,0);

       
    //     CRTVector lightDir = light.getPosition() - intersection;
    //     // std::cout << lightDir.getX() <<"," << lightDir.getY() << "," << lightDir.getZ() << std::endl;
    //     // std::cout << intersection.getX() <<"," << intersection.getY() << "," << intersection.getZ() << std::endl;

    //     lightDir = lightDir.normalize();
    //     // std::cout << lightDir.getX() <<"," << lightDir.getY() << "," << lightDir.getZ() << std::endl;

    //     float sr = lightDir.length();
    //     // std::cout << "sr: " << sr << std::endl;
    //     // std::cout << "Triangle Normal: " <<  triangle.getNormal().getX() << "," << triangle.getNormal().getY() << "," << triangle.getNormal().getZ() << std::endl;

    //     float cosLaw = std::max(0.0f, lightDir.dotProduct(triangle.getNormal()));
    //     // std::cout << "dotProduct: " << lightDir.dotProduct(triangle.getNormal()) << std::endl;
    //     // std::cout << "cosLaw: " << cosLaw << std::endl;
    //     float intensity = light.getIntensity();
    //     // std::cout << "Intensity: " << intensity << std::endl;
    //     CRTColor albedo = CRTColor(100,255,0);
    //     float sa = 4 * 3.14 * sr * sr;

    //     CRTRay shadowRay;
    //     shadowRay.origin = intersection;
    //     shadowRay.direction = lightDir;

    //     bool intersects = false;

        

        


    //     CRTColor color = CRTColor(intensity/sa * albedo.red * cosLaw,intensity/sa * albedo.green * cosLaw,intensity/sa * albedo.blue * cosLaw);
    //     // std::cout << "Red: " << color.red << std::endl;
        

    //     return color;
    // }
};