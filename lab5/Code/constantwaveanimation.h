#ifndef CONSTANTWAVEANIMATION_H
#define CONSTANTWAVEANIMATION_H

#include "animation.h"
#include "wave.h"
#include <vector>


class ConstantWaveAnimation : public Animation
{
public:
    ConstantWaveAnimation();
    ConstantWaveAnimation(std::vector<Wave> animationWaves, float stepAngle);
    std::vector<Wave> waves;
    float phase;
    float step;

    float getPhaseRad();
    // Animation interface
public:
    void update() override;
};

#endif // CONSTANTWAVEANIMATION_H
