//
// Created by bobini on 10.03.23.
//

#ifndef OGREPROJECT_SOUNDEFFECT_H
#define OGREPROJECT_SOUNDEFFECT_H


#include "IPickupEffect.h"
#include "sounds/FFmpegOpenAlSound.h"

class SoundEffect : public IPickupEffect {
public:
    SoundEffect(sounds::FFmpegOpenALSound sound);

    void run() override;

    void update(float delta_time) override;

    bool isRunning() override;

    bool isFinished() override;
private:
    sounds::FFmpegOpenALSound sound_effect_;
};


#endif //OGREPROJECT_SOUNDEFFECT_H
