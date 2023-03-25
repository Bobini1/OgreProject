#pragma once
#include <AL/al.h>
#include <string>

enum class Effect{
    NONE,
    EAXREVERB,
    REVERB,
    CHORUS,
    DISTORTION,
    ECHO,
    FLANGER,
    FREQUENCY_SHIFTER,
    VOCAL_MORPHER,
    PITCH_SHIFTER,
    RING_MODULATOR,
    AUTOWAH,
    COMPRESSOR,
    EQUALIZER
};

class SoundObject {
    public:
        SoundObject(const char* filePath, const char* name, bool isEffect, Effect effect = Effect::NONE);
        const std::string& getName() { return m_name; };
        ALuint& getBuffer() { return m_buffer; };
        ALuint& getEffect() { return m_effect; };
        ALuint& getSlot() { return m_slot; };
        bool getIsEffect() { return m_isEffect; };
        std::shared_ptr<const ALuint>& getSampleBuffer() { return sampleBuffer; };
    private:
        std::string m_name;
        ALuint m_effect;
        ALuint m_buffer;
        ALuint m_slot;
        bool m_isEffect;
        std::shared_ptr<const ALuint> sampleBuffer;
};