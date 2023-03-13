#pragma once
#include <AL/al.h>
#include <string>

class SoundObject {
    public:
        SoundObject(const char* filePath, const char* name);
        const std::string& getName() { return m_name; };
        ALuint& getBuffer() { return m_buffer; };
        std::shared_ptr<const ALuint>& getSampleBuffer() { return sampleBuffer; };
    private:
        std::string m_name;
        ALuint m_buffer;
        std::shared_ptr<const ALuint> sampleBuffer;
};