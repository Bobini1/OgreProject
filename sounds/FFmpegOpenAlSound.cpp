//
// Created by bobini on 14.01.23.
//

#include <stdexcept>
#include <memory>
#include "FFmpegOpenAlSound.h"

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}
#include <AL/alc.h>
#include <AL/alext.h>

sounds::FFmpegOpenALSound::FFmpegOpenALSound(const char* filename)
{
    auto* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, filename, nullptr, nullptr)) {
        throw std::runtime_error("Could not open file " +
                                 std::string(filename));
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        throw std::runtime_error("Could not find stream info");
    }
    const auto audioStreamIndex = av_find_best_stream(
      formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (audioStreamIndex < 0) {
        throw std::runtime_error("Could not find audio stream");
    }
    const auto* audioStream = formatContext->streams[audioStreamIndex];
    const auto* codecParameters = audioStream->codecpar;
    const auto* codec = avcodec_find_decoder(codecParameters->codec_id);
    if (codec == nullptr) {
        throw std::runtime_error("Unsupported codec");
    }
    AVCodecContext* codecContext = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(codecContext, codecParameters) < 0) {
        throw std::runtime_error("Couldn't copy codec context");
    }
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        throw std::runtime_error("Could not open codec");
    }
    auto format = setSampleFormat(*codecContext, *codec);

    auto samples = decodeFile(*formatContext, *audioStream, *codecContext);
    alcMakeContextCurrent(getALContext());
    // load samples into openal
    ALuint buffer{};
    alGenBuffers(1, &buffer);
    sampleBuffer = std::shared_ptr<ALuint>(new ALuint(buffer), deleteBuffer);

    alBufferData(buffer,
                 getALFormat(format, codecContext->channels),
                 samples.data(),
                 static_cast<ALsizei>(samples.size()),
                 codecContext->sample_rate);

    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
}
sounds::FFmpegOpenALSound::FFmpegOpenALSound(std::shared_ptr<SoundObject>& soundObject, float x, float y, float z){
    ALuint buffer = soundObject->getBuffer();
    sampleBuffer = soundObject->getSampleBuffer();
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, x, y, z);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
}
sounds::FFmpegOpenALSound::~FFmpegOpenALSound()
{
    alDeleteSources(1, &source);
}
auto
sounds::FFmpegOpenALSound::isPlaying() const -> bool
{
    ALint state{};
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

sounds::FFmpegOpenALSound::FFmpegOpenALSound(
  const sounds::FFmpegOpenALSound& other)
  : sampleBuffer(other.sampleBuffer)
{
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, static_cast<ALint>(*sampleBuffer));
}
sounds::FFmpegOpenALSound::FFmpegOpenALSound(
  sounds::FFmpegOpenALSound&& other) noexcept
  : source(other.source)
  , sampleBuffer(std::move(other.sampleBuffer))
{
    other.source = 0;
}
auto
sounds::FFmpegOpenALSound::operator=(const sounds::FFmpegOpenALSound& other)
  -> sounds::FFmpegOpenALSound&
{
    if (this != &other) {
        alDeleteSources(1, &source);
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, static_cast<ALint>(*sampleBuffer));
    }
    return *this;
}
auto
sounds::FFmpegOpenALSound::operator=(sounds::FFmpegOpenALSound&& other) noexcept
  -> sounds::FFmpegOpenALSound&
{
    if (this != &other) {
        alDeleteSources(1, &source);
        source = other.source;
        sampleBuffer = std::move(other.sampleBuffer);
        other.source = 0;
    }
    return *this;
}
#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-make-member-function-const"
void
sounds::FFmpegOpenALSound::pause()
{
    alSourcePause(source);
}
void
sounds::FFmpegOpenALSound::stop()
{
    alSourceStop(source);
}
void
sounds::FFmpegOpenALSound::play()
{
    alSourcePlay(source);
}
void
sounds::FFmpegOpenALSound::setVolume(float volume)
{
    alSourcef(source, AL_GAIN, volume);
}
auto
sounds::FFmpegOpenALSound::isPaused() const -> bool
{
    ALint state{};
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}
auto
sounds::FFmpegOpenALSound::isStopped() const -> bool
{
    ALint state{};
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}
auto
sounds::FFmpegOpenALSound::getVolume() const -> float
{
    float volume{};
    alGetSourcef(source, AL_GAIN, &volume);
    return volume;
}
auto
sounds::FFmpegOpenALSound::getDuration() const -> std::chrono::nanoseconds
{
    ALint size{};
    alGetBufferi(*sampleBuffer, AL_SIZE, &size);
    ALint frequency{};
    alGetBufferi(*sampleBuffer, AL_FREQUENCY, &frequency);
    ALint channels{};
    alGetBufferi(*sampleBuffer, AL_CHANNELS, &channels);
    ALint bits{};
    alGetBufferi(*sampleBuffer, AL_BITS, &bits);
    return std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(
      size * 8 / (frequency * channels * bits)));
}
void
sounds::FFmpegOpenALSound::setIsLooping(bool looping)
{
    alSourcei(source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}
void
sounds::FFmpegOpenALSound::setRate(float rate)
{
    alSourcef(source, AL_PITCH, rate);
}
void
sounds::FFmpegOpenALSound::setTimePoint(std::chrono::nanoseconds offset)
{
    const auto frequency = getFrequency();
    const auto samples = offset.count() * frequency / 1'000'000'000;
    alSourcei(source, AL_SAMPLE_OFFSET, static_cast<ALint>(samples));
    // todo: check if this requires the number of channels
}
auto
sounds::FFmpegOpenALSound::getFrequency() const -> int
{
    ALint frequency{};
    alGetBufferi(*sampleBuffer, AL_FREQUENCY, &frequency);
    return frequency;
}
auto
sounds::FFmpegOpenALSound::getRate() const -> float
{
    float rate{};
    alGetSourcef(source, AL_PITCH, &rate);
    return rate;
}
auto
sounds::FFmpegOpenALSound::getIsLooping() const -> bool
{
    ALint looping{};
    alGetSourcei(source, AL_LOOPING, &looping);
    return looping == AL_TRUE;
}
auto
sounds::FFmpegOpenALSound::getChannels() const -> int
{
    ALint channels{};
    alGetBufferi(*sampleBuffer, AL_CHANNELS, &channels);
    return channels;
}
auto
sounds::FFmpegOpenALSound::getTimePoint() const -> std::chrono::nanoseconds
{
    ALint samples{};
    alGetSourcei(source, AL_SAMPLE_OFFSET, &samples);
    return std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(
      samples * 1'000'000'000 / getFrequency()));
}
#pragma clang diagnostic pop
