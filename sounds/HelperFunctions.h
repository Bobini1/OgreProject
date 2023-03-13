#pragma once
extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}
#include <AL/alc.h>
#include <AL/alext.h>

auto getALFormat(AVSampleFormat AVFormat, int channels) -> ALenum;
auto setSampleFormat(AVCodecContext& context, const AVCodec& codec) -> AVSampleFormat;
void deleteBuffer(ALuint* buffer);
void deletePacket(AVPacket* packet);
void deleteFrame(AVFrame* frame);
auto decodeFile(AVFormatContext& formatContext, const AVStream& audioStream, AVCodecContext& codecContext) -> std::vector<unsigned char>;
auto getALDevice() -> ALCdevice*;
auto getALContext() -> ALCcontext*;