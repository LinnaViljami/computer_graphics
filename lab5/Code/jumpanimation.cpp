#include "jumpanimation.h"
#include <cmath>

JumpAnimation::JumpAnimation()
{

}

JumpAnimation::JumpAnimation(int centerX, int centerY, int centerZ, double animationStepAngle, ImportedObject *animatedObject) :
    x(centerX),
    y(centerY),
    z(centerZ),
    stepAngle(animationStepAngle),
    currentPhase(0),
    object(animatedObject)
{


}

void JumpAnimation::update()
{
    double angle;
    angle = currentPhase + stepAngle;
    if(angle >= 360){
        angle -= 360;
    }
    currentPhase = angle;
    object->setRotation(0, static_cast<int>(-sin(currentPhase)), 0);
    object->setTranslation(0, static_cast<float>(cos(currentPhase)), 0);

}
