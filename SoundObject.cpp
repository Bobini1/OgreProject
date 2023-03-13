#include "SoundObject.h"
#include <stdexcept>
#include <memory>

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}
#include <AL/alc.h>
#include <AL/alext.h>
#include "sounds/HelperFunctions.h"

SoundObject::SoundObject(const char* filePath, const char* name)
    : m_name(name)
{
    auto* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, filePath, nullptr, nullptr)) {
        throw std::runtime_error("Could not open file " +
                                 std::string(filePath));
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
    alGenBuffers(1, &m_buffer);
    sampleBuffer = std::shared_ptr<ALuint>(new ALuint(m_buffer), deleteBuffer);

    alBufferData(m_buffer,
                 getALFormat(format, codecContext->channels),
                 samples.data(),
                 static_cast<ALsizei>(samples.size()),
                 codecContext->sample_rate);

    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
}