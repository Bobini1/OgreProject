#include "HelperFunctions.h"

auto
getALFormat(AVSampleFormat AVFormat, int channels) -> ALenum
{
    switch (channels) {
        case 1:
            switch (AVFormat) {
                case AV_SAMPLE_FMT_U8:
                    return AL_FORMAT_MONO8;
                case AV_SAMPLE_FMT_S16:
                    return AL_FORMAT_MONO16;
                case AV_SAMPLE_FMT_FLT:
                    return AL_FORMAT_MONO_FLOAT32;
                case AV_SAMPLE_FMT_DBL:
                    return AL_FORMAT_MONO_DOUBLE_EXT;
                default:
                    throw std::runtime_error("Unsupported format");
            }
        case 2:
            switch (AVFormat) {
                case AV_SAMPLE_FMT_U8:
                    return AL_FORMAT_STEREO8;
                case AV_SAMPLE_FMT_S16:
                    return AL_FORMAT_STEREO16;
                case AV_SAMPLE_FMT_FLT:
                    return AL_FORMAT_STEREO_FLOAT32;
                case AV_SAMPLE_FMT_DBL:
                    return AL_FORMAT_STEREO_DOUBLE_EXT;
                default:
                    throw std::runtime_error("Unsupported format");
            }
        default:
            throw std::runtime_error("Unsupported format");
    }
}

auto
setSampleFormat(AVCodecContext& context, const AVCodec& codec) -> AVSampleFormat
{
    if (codec.type != AVMEDIA_TYPE_AUDIO || codec.sample_fmts == nullptr) {
        throw std::runtime_error("Could not decode audio");
    }
    const AVSampleFormat* fmt{};

    context.request_sample_fmt = AV_SAMPLE_FMT_NONE;

    for (fmt = codec.sample_fmts; *fmt != AV_SAMPLE_FMT_NONE; fmt++) {
        if (!av_sample_fmt_is_planar(*fmt) && *fmt != AV_SAMPLE_FMT_S32) {
            context.request_sample_fmt = *fmt;
            break;
        }
    }

    if (context.request_sample_fmt == AV_SAMPLE_FMT_NONE) {
        throw std::runtime_error("Could not decode audio");
    }
    context.sample_fmt = context.request_sample_fmt;
    return context.request_sample_fmt;
}

void
deleteBuffer(ALuint* buffer)
{
    alDeleteBuffers(1, buffer);
    delete buffer;
}

void
deletePacket(AVPacket* packet)
{
    av_packet_free(&packet);
}

void
deleteFrame(AVFrame* frame)
{
    av_frame_free(&frame);
}

auto
decodeFile(AVFormatContext& formatContext,
           const AVStream& audioStream,
           AVCodecContext& codecContext) -> std::vector<unsigned char>
{
    size_t sampleCount = 0;
    const auto bytesPerSample =
      static_cast<size_t>(av_get_bytes_per_sample(codecContext.sample_fmt));
    const auto channels = static_cast<size_t>(codecContext.channels);

    std::vector<unsigned char> samples;
    std::unique_ptr<AVPacket, decltype(&deletePacket)> packet = {
        av_packet_alloc(), &deletePacket
    };
    std::unique_ptr<AVFrame, decltype(&deleteFrame)> frame = { av_frame_alloc(),
                                                               &deleteFrame };
    while (av_read_frame(&formatContext, packet.get()) >= 0) {
        if (packet->stream_index == audioStream.index) {
            avcodec_send_packet(&codecContext, packet.get());
            while (avcodec_receive_frame(&codecContext, frame.get()) == 0) {
                const auto frameSampleCount =
                  static_cast<size_t>(frame->nb_samples);
                sampleCount += frameSampleCount * channels;
                samples.resize(sampleCount * bytesPerSample);
                auto frameSize = frameSampleCount * channels * bytesPerSample;
                std::copy(frame->data[0],
                          frame->data[0] + frameSize,
                          samples.end() - frameSize);
            }
        }
        av_packet_unref(packet.get());
    }
    return samples;
}

auto
getALDevice() -> ALCdevice*
{
    static auto device = std::unique_ptr<ALCdevice, decltype(&alcCloseDevice)>(
      alcOpenDevice(nullptr), &alcCloseDevice);
    return device.get();
}
auto
getALContext() -> ALCcontext*
{
    static auto context =
      std::unique_ptr<ALCcontext, decltype(&alcDestroyContext)>(
        alcCreateContext(getALDevice(), nullptr), &alcDestroyContext);
    return context.get();
}