#include "jumpanimation.h"
#include <cmath>

JumpAnimation::JumpAnimation()
{

}

JumpAnimation::JumpAnimation(int centerX, int centerY, int centerZ, double jumpHeight, double animationStepAngle, ImportedObject *animatedObject) :
    x(centerX),
    y(centerY),
    z(centerZ),
    height(jumpHeight),
    stepAngle(animationStepAngle),
    currentPhase(0),
    object(animatedObject)
{
    object->setTranslation(x, y, z);

}

void JumpAnimation::update()
{
    double angle;
    angle = currentPhase + stepAngle;
    if(angle >= 360){
        angle -= 360;
    }
    currentPhase = angle;
    double radAngle = (currentPhase / 360.0) * 2 * M_PI;
    object->setRotation(0, 0, static_cast<float>(-sin(radAngle - M_PI)*50));
    object->setTranslation(x, y + static_cast<float>(height *(1 + cos(radAngle - M_PI))), z);

}
