#ifndef BOUNCINGBALLANIMATION_H
#define BOUNCINGBALLANIMATION_H

#include "animation.h"
#include "object.h"

class BouncingBallAnimation : public Animation
{
public:
    BouncingBallAnimation();
    BouncingBallAnimation(int centerX, int centerY, int centerZ, double animationStepAngle, double bounceHeight, ImportedObject* animatedObject);
    void update() override;
private:
    int x;
    int y;
    int z;
    double stepAngle;
    double currentPhase;
    double height;
    ImportedObject* object;
};

#endif // BOUNCINGBALLANIMATION_H
