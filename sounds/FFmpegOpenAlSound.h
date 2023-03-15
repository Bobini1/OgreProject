//
// Created by bobini on 14.01.23.
//

#ifndef OGREPROJECT_FFMPEGOPENALSOUND_H
#define OGREPROJECT_FFMPEGOPENALSOUND_H
#include "../SoundObject.h"
#include "HelperFunctions.h"
#include <AL/al.h>
#include <vector>
#include <optional>
#include <span>

namespace sounds {
class FFmpegOpenALSound
{
    ALuint source{};
    std::shared_ptr<const ALuint> sampleBuffer;

  public:
    explicit FFmpegOpenALSound(const char* filename);
    FFmpegOpenALSound(std::shared_ptr<SoundObject>& soundObject, float x, float y, float z);
    FFmpegOpenALSound(const FFmpegOpenALSound& other);
    FFmpegOpenALSound(FFmpegOpenALSound&& other) noexcept;
    auto operator=(const FFmpegOpenALSound& other) -> FFmpegOpenALSound&;
    auto operator=(FFmpegOpenALSound&& other) noexcept -> FFmpegOpenALSound&;
    ~FFmpegOpenALSound();

    void play();
    void stop();
    void pause();
    void setVolume(float volume);
    void setIsLooping(bool looping);
    void setRate(float rate);
    void setTimePoint(std::chrono::nanoseconds offset);

    auto isPlaying() const -> bool;
    auto isPaused() const -> bool;
    auto isStopped() const -> bool;
    auto getVolume() const -> float;
    auto getDuration() const -> std::chrono::nanoseconds;
    auto getRate() const -> float;
    auto getIsLooping() const -> bool;
    auto getFrequency() const -> int;
    auto getChannels() const -> int;
    auto getTimePoint() const -> std::chrono::nanoseconds;
};
} // namespace sounds
#endif // OGREPROJECT_FFMPEGOPENALSOUND_H
