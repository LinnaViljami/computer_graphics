#ifndef CIRCLEANIMATION_H
#define CIRCLEANIMATION_H

#include "animation.h"
#include "object.h"

class CircleAnimation : public Animation
{
public:
    CircleAnimation();
    CircleAnimation(double circleRadius, double animationStepAngle, bool rotateClockwise, ImportedObject* animatedObject);


    // Animation interface
public:
    virtual void update() override;


private:
    double radius;
    double stepAngle;
    bool clockwise;
    double currentPhase;
    ImportedObject* object;
    void rotateObject();
    void translateObject();
};

#endif // CIRCLEANIMATION_H
