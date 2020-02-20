#ifndef SCENE_H_
#define SCENE_H_

#include "light.h"
#include "object.h"
#include "triple.h"

#include <vector>

// Forward declerations
class Ray;
class Image;

class Scene
{
    std::vector<ObjectPtr> objects;
    std::vector<LightPtr> lights;   // no ptr needed, but kept for consistency
    Point eye;

    public:

        // trace a ray into the scene and return the color
        Color trace(Ray const &ray);

        // render the scene to the given image
        void render(Image &img);

        void addObject(ObjectPtr obj);
        void addLight(Light const &light);
        void setEye(Triple const &position);

        unsigned getNumObject();
        unsigned getNumLights();

    private:
        Color getPhongIlluminationColor(Vector N, Point hit, Material material);
        Vector getNormalizedLightVectorFromPosition(Point position, Light light);
        double calculateDiffuseComponent(Vector normal, Point hit, Light light);
        double calculateSpecularComponent(Vector normal, Point hit, double p, Light light);
};

#endif
