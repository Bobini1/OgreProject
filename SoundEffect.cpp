//
// Created by bobini on 10.03.23.
//

#include "SoundEffect.h"

void SoundEffect::run() {
    sound_effect_.play();
}

void SoundEffect::pause() {
    sound_effect_.pause();
}

void SoundEffect::update(float delta_time) {
}

bool SoundEffect::isRunning() {
    return sound_effect_.isPlaying();
}

bool SoundEffect::isFinished() {
    return !sound_effect_.isPlaying();
}

void SoundEffect::updatePosition(float x, float y, float z){
    sound_effect_.updatePosition(x,y,z);
}

SoundEffect::SoundEffect(sounds::FFmpegOpenALSound sound)
: sound_effect_(std::move(sound))
{
}
