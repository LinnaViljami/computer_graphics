#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <limits>

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

Color Scene::getReflectionColor(Point hitPosition, Vector I, Vector N, unsigned currentDepth) {
    // Calculate reflection direction
    Vector R = I - 2 * N.dot(I) * N;  

    hitPosition = offsetPoint(hitPosition, N);
    Ray reflectionRay(hitPosition, R);

    return trace(reflectionRay, currentDepth - 1);
}

Color Scene::refract(Point hitPosition, Vector D, Vector N, double ni, double nt, unsigned currentDepth) {
    // These variable names are named after the components in the lecture slides.
    double redComponent = 1 - (ni*ni * (1 - pow(D.dot(N), 2))) / (nt*nt);
    Vector rightHandSide = N * sqrt(redComponent);
    Vector leftHandSide = (ni * (D - (D.dot(N)) * N)) / nt;
    Vector T = leftHandSide - rightHandSide;

    Ray refractionRay(hitPosition, T);
    return trace(refractionRay, currentDepth - 1);
}

Color Scene::trace(Ray const &ray, unsigned depth)
{
    pair<ObjectPtr, Hit> mainhit = castRay(ray);
    ObjectPtr obj = mainhit.first;
    Hit min_hit = mainhit.second;

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material const &material = obj->material;
    Point hit = ray.at(min_hit.t);
    Vector V = -ray.D;

    // Pre-condition: For closed objects, N points outwards.
    Vector N = min_hit.N;

    // The shading normal always points in the direction of the view,
    // as required by the Phong illumination model.
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
        Vector L = (light->position - hit).normalized();


        if (renderShadows && isShadow(hit, L, light->position, shadingN)) {
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

    if (depth > 0 and material.isTransparent)
    {
        // The object is transparent, and thus refracts and reflects light.
        // Use Schlick's approximation to determine the ratio between the two.
        
        double ni, nt;
        if (ray.D.dot(shadingN) < 0) {
            // The angle between the ray and the normal is larger than 90
            // degrees, so this ray goes from the outside into the material.
            ni = 1.0;
            nt = material.nt;
        } else {
            // The ray comes from within a material and goes out.
            ni = material.nt;
            nt = 1.0;
        }

        double kr0 = pow((ni-nt)/(ni+nt),2);

        Point P = hit;
        Vector D = ray.D;
        Vector N = shadingN;
        double cosThetaI = D.normalized().dot(N.normalized());

        double kr = kr0 + (1-kr0)*pow((1-cosThetaI),5.0);
        double kt = 1 - kr;


        refract(hit, ray.D, shadingN, ni, material.nt, depth);
    }
    else if (depth > 0 and material.ks > 0.0)
    {
        // The object is not transparent, but opaque.
        Color reflectionColor = getReflectionColor(hit, ray.D, shadingN, depth);
        color += material.ks * reflectionColor;
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
            Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
            Ray ray(eye, (pixel - eye).normalized());
            Color col = trace(ray, recursionDepth);
            col.clamp();
            img(x, y) = col;
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
