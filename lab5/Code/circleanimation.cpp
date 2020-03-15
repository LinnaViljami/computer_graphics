#include "circleanimation.h"
#include <cmath>

CircleAnimation::CircleAnimation()
{

}

CircleAnimation::CircleAnimation(int centerX, int centerY, int centerZ, double circleRadius, double animationStepAngle, bool rotateClockwise, ImportedObject* animatedObject) :
    x(centerX),
    y(centerY),
    z(centerZ),
    radius(circleRadius),
    stepAngle(animationStepAngle),
    clockwise(rotateClockwise),
    currentPhase(0),
    object(animatedObject)
{

}

void CircleAnimation::update()
{
    rotateObject();
    translateObject();


}

void CircleAnimation::rotateObject()
{
    double angle;
    angle = currentPhase + stepAngle;
    if(angle >= 360){
        angle -= 360;
    }
    currentPhase = angle;
    object->setRotation(0, static_cast<int>(currentPhase), 0);
}

void CircleAnimation::translateObject()
{
    double radAngle = (currentPhase / 360.0) * 2 * M_PI;
    float translateX = x + static_cast<float>(sin(radAngle) * radius);
    float translateZ = z + static_cast<float>(cos(radAngle) * radius);
    object->setTranslation(translateX, y,translateZ);
}

