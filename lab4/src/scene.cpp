#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <iostream>

using namespace std;

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    return pair<ObjectPtr, Hit>(obj, min_hit);
}

// Returns a point with a small ofset relative to the input in the
// direction of the normal. This can be used to prevent shadow acne
// and similar issues.
Point Scene::offsetPoint(Point hit, Vector normal) {
    return hit + epsilon * normal;
}

bool Scene::isShadow(Point hit, Vector L, Point lightPosition, Vector shadingN) {
    // Use a small offset to prevent shadow acne.
    hit = offsetPoint(hit, shadingN);

    Ray shadowRay(hit, L);
    pair<ObjectPtr, Hit> shadowHit = castRay(shadowRay);
    ObjectPtr obj = shadowHit.first;
    
    Hit min_hit = shadowHit.second;
    Point shadowHitPoint = shadowRay.at(min_hit.t);

    if (!obj) {
        // No object was hit by the shadow ray.
        return false;
    }

    const double distanceToLight = (hit - lightPosition).length();
    const double distanceToShadowRayHit = (hit - shadowHitPoint).length();

    if (distanceToShadowRayHit > distanceToLight) {
        // The shadow ray hit an object, but after reaching the light. The 
        // light is in between the object and the hit. This is not a shadow.
        return false;
    }

    return true;
}

Color Scene::traceReflection(Point hitPosition, Vector I, Vector N, bool fromInside, unsigned currentDepth) {
    if (fromInside) {
        hitPosition = offsetPoint(hitPosition, -N);
        N = -N;
    } else {
        hitPosition = offsetPoint(hitPosition, N);
    }

    // Calculate reflection direction
    Vector R = I - 2 * N.dot(I) * N;  

    Ray reflectionRay(hitPosition, R);
    return trace(reflectionRay, currentDepth - 1);
}

Color Scene::traceRefraction(Point hitPosition, Vector D, Vector N, double ni, double nt, double redComponent, bool fromInside, unsigned currentDepth) {    
    if (fromInside) {
        hitPosition = offsetPoint(hitPosition, N);
        N = -N;
    } else {
        hitPosition = offsetPoint(hitPosition, -N);
    }
    
    // These variable names are named after the components in the lecture slides.
    Vector rightHandSide = N * sqrt(redComponent);
    Vector leftHandSide = (ni * (D - (D.dot(N)) * N)) / nt;
    Vector T = leftHandSide - rightHandSide;

    
    Ray refractionRay(hitPosition, T);



    return trace(refractionRay, currentDepth - 1);
}

Color Scene::getPhongIlluminationColor(Material material, Point hitLocation, Vector surfaceNormal, Vector rayDirection)
{
    Vector N = surfaceNormal.normalized();
    Vector V = -rayDirection.normalized();

    Vector shadingN;
    if (N.dot(V) >= 0.0)
        shadingN = N;
    else
        shadingN = -N;

    Color matColor = material.color;

    // Add ambient once, regardless of the number of lights.
    Color color = material.ka * matColor;

    // Add diffuse and specular components.
    for (auto const &light : lights)
    {
        Vector L = (light->position - hitLocation).normalized();


        if (renderShadows && isShadow(hitLocation, L, light->position, shadingN)) {
            // Don't add lighting if the hit is in the shadow of this light.
            continue;
        }

        // Add diffuse.
        double diffuse = std::max(shadingN.dot(L), 0.0);
        color += diffuse * material.kd * light->color * matColor;

        // Add specular.
        Vector reflectDir = reflect(-L, shadingN);
        double specAngle = std::max(reflectDir.dot(V), 0.0);
        double specular = std::pow(specAngle, material.n);

        color += specular * material.ks * light->color;
    }
    return color;
}

double Scene::getReflectionIntensityFactorApproximation(double ni, double nt, bool fromInside, Vector N, Vector D)
{
    N.normalize();
    D.normalize();
    double kr0 = pow((ni-nt)/(ni+nt),2);


    double cosThetaI;
    if (fromInside) {
        cosThetaI = (-D).dot(-N);
    } else {
        cosThetaI = (-D).dot(N);
    }

    return kr0 + (1-kr0)*pow((1-cosThetaI),5.0);

}

double Scene::getRedComponent(double ni, double nt, bool fromInside,  Vector N, Vector D)
{
    Vector normalTowardsRay;
    if(fromInside){
        normalTowardsRay = (-N);
    }
    else{
        normalTowardsRay = N;
    }

    return 1 - (((ni*ni) * (1-(pow((D.dot(normalTowardsRay)),2.0))))     /     (nt*nt));


}

Color Scene::trace(Ray const &ray, unsigned depth)
{
    //background color
    Color color = Color(0.0,0.0,0.0);

    // extract hit information
    pair<ObjectPtr, Hit> hitDetectionResult = castRay(ray);
    ObjectPtr objectToHit = hitDetectionResult.first;
    Hit hit = hitDetectionResult.second;

    // If no  hit just return background color.
    if (!objectToHit)
        return color;


    Material const &material = objectToHit->material;
    Point hitLocation = ray.at(hit.t);


    // Pre-condition: For closed objects, N points outwards.
    Vector N = hit.N.normalized();
    Vector D = ray.D.normalized();

    // Add phong illumination color
    color += getPhongIlluminationColor(material,hitLocation,N,D);

    // track possible reflections and reftractions for transparent object
    if (depth > 0 && material.isTransparent)
    {
        // The object is transparent, and thus refracts and reflects light.
        // Use Schlick's approximation to determine the ratio between the two.
        
        double ni, nt;
        bool fromInside;

        // lambda function definition
        auto isAngleOver90Degree = [](Vector vec1, Vector vec2){ return ((vec1).dot(vec2) > 0);};

        if (isAngleOver90Degree(-D, N)) {
            // ray comes from outside of the sphere
            fromInside = false;
            ni = 1.0;
            nt = material.nt;
        } else {
            // The ray comes from within a material and goes out.
            fromInside = true;
            ni = material.nt;
            nt = 1.0;
        }


        // calculate reflection vs refraction intensity factors
        double kr = getReflectionIntensityFactorApproximation(ni, nt, fromInside,N,D);
        double kt = 1 - kr;


        double redComponent = getRedComponent(ni, nt, fromInside, N, D);
        Point P = hitLocation;

        // if redComponent is negative total internal reflection happens
        if (redComponent < 0) {
            color += traceReflection(P, D, N, fromInside, depth);
        } else {
            color += kr * traceReflection(P, D, N, fromInside, depth);
            color += kt * traceRefraction(P, D, N, ni, nt, redComponent, fromInside, depth);
        }
    }
    // if non transparent but reflective object
    else if (depth > 0 and material.ks > 0.0)
    {
        color += material.ks * traceReflection(hitLocation, ray.D, N, false, depth);
    }
    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();

    for (unsigned y = 0; y < h; ++y)
        for (unsigned x = 0; x < w; ++x)
        {
            Point pixelCorner(x, h - 1 - y, 0);
            std::vector<Point> subPixels = getSuperSamplingPixels(pixelCorner);
            std::vector<Color> subPixelColors = getPixelColors(subPixels);
//            Color averageColor = Color(0.0, 0.0, 0.0);
//            for (auto color : subPixelColors){
//                averageColor += color;
//                averageColor = averageColor/2;
//            }
            Color averageColor = std::accumulate(subPixelColors.begin(), subPixelColors.end(),Color(0.0,0.0,0.0))/subPixelColors.size();
            img(x, y) = averageColor;
        }
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene()
:
    objects(),
    lights(),
    eye(),
    renderShadows(false),
    recursionDepth(0),
    supersamplingFactor(1)
{}

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}

std::vector<Point> Scene::getSuperSamplingPixels(Point pixelCorner)
{
    if(supersamplingFactor <= 1){
        return {Point(pixelCorner.x + 0.5, pixelCorner.y + 0.5, pixelCorner.z)};
    }
    else{
        std::vector<Point> subPixels = {};
        double subPixelSize = 1.0/static_cast<double>(supersamplingFactor);
        for (unsigned i=0; i<supersamplingFactor; i++){
//            std::cout << "i=" << i << " ,korkein " << supersamplingFactor << std::endl;
            double deltaX = subPixelSize/2.0 + subPixelSize*i;
            for (unsigned j=0; j<supersamplingFactor; j++){
//                std::cout << "j=" << j << " ,korkein " << supersamplingFactor << std::endl;
                double deltaY = subPixelSize/2.0 + subPixelSize*j;
                subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y + deltaY, pixelCorner.z));
            }
        }
        return subPixels;
    }
//    else if(supersamplingFactor % 2 == 0){
//        for (unsigned i=1; i<=(supersamplingFactor/2); i++){
//            double deltaX = static_cast<double>(i)/static_cast<double>(supersamplingFactor+1);
//            for (unsigned j=1; j<=(supersamplingFactor/2); j++){
//                double deltaY = static_cast<double>(j)/static_cast<double>(supersamplingFactor+1);
//                subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y + deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y - deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x - deltaX, pixelCorner.y + deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x - deltaX, pixelCorner.y - deltaY, pixelCorner.z));
//            }
//        }
//    }
//    else{
//        // add pixels in vertical centerline of subpixel grid
//        subPixels.push_back(pixelCorner);
//        for (unsigned j=1; j<=((supersamplingFactor-1)/2); j++){
//            double deltaY = static_cast<double>(j)/static_cast<double>(supersamplingFactor+1);
//            subPixels.push_back(Point(pixelCorner.x, pixelCorner.y + deltaY, pixelCorner.z));
//            subPixels.push_back(Point(pixelCorner.x, pixelCorner.y - deltaY, pixelCorner.z));
//        }

//        // add pixels outside of the vertical centerline row by row
//        for (unsigned i=1; i<=((supersamplingFactor-1)/2); i++){
//            double deltaX = static_cast<double>(i)/static_cast<double>(supersamplingFactor+1);
//            for (unsigned j=1; j<=((supersamplingFactor-1)/2); j++){
//                // add row pixels outride of the horizontal centreline
//                double deltaY = static_cast<double>(j)/static_cast<double>(supersamplingFactor+1);
//                subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y + deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y - deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x - deltaX, pixelCorner.y + deltaY, pixelCorner.z));
//                subPixels.push_back(Point(pixelCorner.x - deltaX, pixelCorner.y - deltaY, pixelCorner.z));
//            }
//            // add pixels in  this row horizontal centerline
//            subPixels.push_back(Point(pixelCorner.x - deltaX, pixelCorner.y, pixelCorner.z));
//            subPixels.push_back(Point(pixelCorner.x + deltaX, pixelCorner.y, pixelCorner.z));

//        }
//    }

//    return subPixels;
}

std::vector<Color> Scene::getPixelColors(std::vector<Point> pixels)
{
    std::vector<Color> colors = {};
    for (auto pixel : pixels){
        Ray ray(eye, (pixel - eye).normalized());
        Color col = trace(ray, recursionDepth);
        col.clamp();
        colors.push_back(col);
    }
    return colors;
}

void Scene::setRenderShadows(bool shadows)
{
    renderShadows = shadows;
}

void Scene::setRecursionDepth(unsigned depth)
{
    recursionDepth = depth;
}

void Scene::setSuperSample(unsigned factor)
{
    supersamplingFactor = factor;
}
