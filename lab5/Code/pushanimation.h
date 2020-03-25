#ifndef PUSHANIMATION_H
#define PUSHANIMATION_H

#include "animation.h"
#include "object.h"

class PushAnimation : public Animation
{
public:
    PushAnimation();
    PushAnimation(int centerX, int centerY, int centerZ, double animationSpeed, ImportedObject* animatedObject);
    void update() override;
private:
    int x;
    int y;
    int z;
    double stepSize;
    double currentPhase;
    ImportedObject* object;
    void updatePhase();
    void rotateObject();
};

#endif // PUSHANIMATION_H
