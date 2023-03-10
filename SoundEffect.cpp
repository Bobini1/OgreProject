//
// Created by bobini on 10.03.23.
//

#include "SoundEffect.h"

void SoundEffect::run() {
    sound_effect_.play();
}

void SoundEffect::update(float delta_time) {
}

bool SoundEffect::isRunning() {
    return sound_effect_.isPlaying();
}

bool SoundEffect::isFinished() {
    return !sound_effect_.isPlaying();
}

SoundEffect::SoundEffect(sounds::FFmpegOpenALSound sound)
: sound_effect_(std::move(sound))
{
}
