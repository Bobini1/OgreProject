#include "SoundObject.h"
#include <stdexcept>
#include <memory>

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
#include <AL/alc.h>
#include <AL/alext.h>
#include "sounds/HelperFunctions.h"

#define MAX_AUDIO_FRAME_SIZE 192000

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
    //AL_FORMAT_MONO_FLOAT32
    alBufferData(m_buffer,
                 AL_FORMAT_MONO_FLOAT32,
                 samples.data(),
                 static_cast<ALsizei>(samples.size()),
                 codecContext->sample_rate);

    std::cout << "echoE" << std::endl;
    if (alcIsExtensionPresent(getALDevice(), "ALC_EXT_EFX") == AL_FALSE){
        std::cout << "wrong" << std::endl;
    }
    m_effect = 0;
    ALenum err;

    /* Create the effect object and set the echo type. */
    myAlGenEffects(1, &m_effect);
    myAlEffecti(m_effect, AL_EFFECT_TYPE, AL_EFFECT_ECHO);

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL error: %s\n", alGetString(err));
        if(myAlIsEffect(m_effect))
            myAlDeleteEffects(1, &m_effect);
    }

    m_slot = 0;
    myAlGenAuxiliaryEffectSlots(1, &m_slot);
    /* Tell the effect slot to use the loaded effect object. Note that the this
     * effectively copies the effect properties. You can modify or delete the
     * effect object afterward without affecting the effect slot.
     */
    myAlAuxiliaryEffectSloti(m_slot, AL_EFFECTSLOT_EFFECT, (ALint)m_effect);
    assert(alGetError()==AL_NO_ERROR && "Failed to set effect slot");

    std::cout << "echoE end" << std::endl;

    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
}