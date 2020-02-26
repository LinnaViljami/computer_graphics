#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

Color Scene::trace(Ray const &ray)
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

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          // the hit objects material
    Point hit = ray.at(min_hit.t);              // the hit point
    Vector N = min_hit.N;                       // the normal at hit point
    //Vector V = -ray.D;                          // the view vector

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector + Vector    vector sum
    *        Vector - Vector    vector difference
    *        Point - Point      yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double * Color     scales each color component (r,g,b)
    *        Color * Color      ditto
    *        pow(a,b)           a to the power of b
    ****************************************************/

//    double Ia = 1;

//    double Id = calculateDiffuseComponent(N, hit,);

//    double Is = calculateSpecularComponent(N, hit, material.n);

//    // Determine color based on Phon illuminatin model
//    Color color =
//            Ia * material.ka * material.color +
//            Id * material.kd * lights.at(0)->color * material.color +
//            Is * material.ks * lights.at(0)->color;

////    Color color = (N + 1) / 2;   // Use this color instead of the Phong color to visualize normal vectors
    Color color = getPhongIlluminationColor(N, hit, material);
    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            Point pixel(x + 0.5, h - 1 - y + 0.5, 0);
            Ray ray(eye, (pixel - eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x, y) = col;
        }
    }
}

// --- Misc functions ----------------------------------------------------------

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

Color Scene::getPhongIlluminationColor(Vector N, Point hit, Material material)
{
    double Ia = 1;
    Color color = Ia * material.ka * material.color;
    for (auto light_ptr : lights){
        color += calculateDiffuseComponent(N, hit, *light_ptr) * material.kd * light_ptr->color * material.color;
        color += calculateSpecularComponent(N, hit, material.n, *light_ptr) * material.ks * light_ptr->color;
    }

    //    Color color = (N + 1) / 2;   // Use this color instead of the Phong color to visualize normal vectors

    return color;

}

Vector Scene::getNormalizedLightVectorFromPosition(Point position, Light light)
{
    Vector lightVector = light.position - position;
    return lightVector.normalized();
}

double Scene::calculateDiffuseComponent(Vector normal, Point hit, Light light)
{
    Vector lightVector = getNormalizedLightVectorFromPosition(hit, light);

    double diffuseComponent = normal.dot(lightVector);
    if (diffuseComponent < 0) {
        diffuseComponent = 0;
    }
    return diffuseComponent;
}

double Scene::calculateSpecularComponent(Vector normal, Point hit, double p, Light light)
{
    Vector lightVector = getNormalizedLightVectorFromPosition(hit, light);

    Vector R = 2 * (normal.dot(lightVector)) * normal - lightVector;

    Vector viewAngle = (eye - hit).normalized();

    double specularComponent = R.dot(viewAngle);
    if (specularComponent < 0) {
        specularComponent = 0;
    }
    return pow(specularComponent, p);
}
