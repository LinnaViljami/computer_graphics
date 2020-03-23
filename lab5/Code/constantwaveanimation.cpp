#include "constantwaveanimation.h"
#include <math.h>
ConstantWaveAnimation::ConstantWaveAnimation()
{

}

ConstantWaveAnimation::ConstantWaveAnimation(std::vector<Wave> animationWaves, float stepAngle) :

    waves(animationWaves),
    phase(0.0f),
    step(stepAngle)
{

}

float ConstantWaveAnimation::getPhaseRad()
{
    return phase/360.0f * 2.0f * static_cast<float>(M_PI);
}

void ConstantWaveAnimation::update()
{
    phase = phase + step;
    if(phase > 360.0f){
        phase -= 360.0f;
    }
}
