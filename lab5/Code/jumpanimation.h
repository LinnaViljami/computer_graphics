#ifndef JUMPANIMATION_H
#define JUMPANIMATION_H

#include "animation.h"
#include "object.h"

class JumpAnimation : public Animation
{
public:
    JumpAnimation();
    JumpAnimation(int centerX, int centerY, int centerZ, double animationStepAngle, ImportedObject* animatedObject);
    void update() override;

private:
    int x;
    int y;
    int z;
    double stepAngle;
    double currentPhase;
    ImportedObject* object;

};

#endif // JUMPANIMATION_H
