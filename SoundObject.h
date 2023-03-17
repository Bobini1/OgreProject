#pragma once
#include <AL/al.h>
#include <string>

class SoundObject {
    public:
        SoundObject(const char* filePath, const char* name);
        const std::string& getName() { return m_name; };
        ALuint& getBuffer() { return m_buffer; };
        ALuint& getEffect() { return m_effect; };
        ALuint& getSlot() { return m_slot; };
        std::shared_ptr<const ALuint>& getSampleBuffer() { return sampleBuffer; };
    private:
        std::string m_name;
        ALuint m_effect;
        ALuint m_buffer;
        ALuint m_slot;
        std::shared_ptr<const ALuint> sampleBuffer;
};