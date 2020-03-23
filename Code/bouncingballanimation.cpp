#include "bouncingballanimation.h"
#include <cmath>

BouncingBallAnimation::BouncingBallAnimation()
{

}

BouncingBallAnimation::BouncingBallAnimation(int centerX, int centerY, int centerZ, double animationStepAngle, double bounceHeight, ImportedObject *animatedObject) :
    x(centerX),
    y(centerY),
    z(centerZ),
    stepAngle(animationStepAngle),
    height(bounceHeight),
    object(animatedObject)
{
    object->setTranslation(x, y, z);

}

void BouncingBallAnimation::update()
{
    double angle;
    angle = currentPhase + stepAngle;
    if(angle >= 360){
        angle -= 360;
    }
    currentPhase = angle;
    double radAngle = (currentPhase / 360.0) * 2 * M_PI;
    object->setTranslation(x, y+ static_cast<float>(height *abs(sin(radAngle))), z);
}
