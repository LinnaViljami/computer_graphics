#include "pushanimation.h"
#include <cmath>

PushAnimation::PushAnimation()
{

}

PushAnimation::PushAnimation(int centerX, int centerY, int centerZ, double animationSpeed, ImportedObject *animatedObject) :
    x(centerX),
    y(centerY),
    z(centerZ),
    stepSize(animationSpeed),
    object(animatedObject)
{
    object->setTranslation(x, y, z);

}

void PushAnimation::update()
{
    updatePhase();
    rotateObject();




}

void PushAnimation::updatePhase()
{

    currentPhase += stepSize*3;
    if(currentPhase >= 360*3){
        currentPhase -= 360*3;
    }

}

void PushAnimation::rotateObject()
{
    if(currentPhase > 180){
        object->setRotation(0,0,0);
    }else{
        double radAngle = (currentPhase / 360.0) * 2 * M_PI;
        object->setRotation(0, 0, static_cast<float>(-pow(sin(radAngle - M_PI),2)*20));
    }
}
